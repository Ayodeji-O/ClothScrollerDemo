// ScrollerManager.cpp - Contains the implementation for the singleton class that manages the
//                       position of the cloth scroller text, and coordinates communication
//                       between clients and the required scroller servers.

#include "ScrollerManager.h"




bool CScrollerManager::InitializeScrollerManager(const std::string& messageFile,
												 const QuantityType scrollerImageWidth,
												 const QuantityType scrollerImageHeight,
												 const QuantityType scrollRate)
{
	bool							bManagerInitializedSuccessfully = false;


	// Image color planes (usually/always for 32-bit images).
	const QuantityType				kColorPlanes = 1;

	// Initialize the text stream server..
	const CTextStreamInitialize		textStreamInitialize(messageFile, true);
	CScrollerManager::pTextStreamServer = new CTextStreamServer(textStreamInitialize);

	if (pTextStreamServer && pTextStreamServer->IsValid()) {

		const CScrollerImageStreamInitialize
									scrollerStreamInitialize(scrollerImageWidth,
									scrollerImageHeight, CScrollerManager::mkImageBitDepth,
									kColorPlanes);

		// Initialize the scroller image stream server
		CScrollerManager::pImageStreamServer =
			new CScrollerImageStreamServer(scrollerStreamInitialize);

		if (CScrollerManager::pImageStreamServer) {
			// Set the scroller scroll rate.
			CScrollerManager::mScrollRate = scrollRate;

			// Store the image width and the image height.
			CScrollerManager::mImageWidth = scrollerImageWidth;
			CScrollerManager::mImageHeight = scrollerImageHeight;

			// Add scroller lead-in spacing...
			CScrollerManager::ApplyLeadInPixelOffset();

			// ...Indicate a success condition.
			bManagerInitializedSuccessfully = true;
		}
	}

	return(bManagerInitializedSuccessfully);
}


bool CScrollerManager::ConcludeScrollerManager()
{
	bool							bScrollerManagerConcludedSuccessfully = false;

	// Release any memory allocated for the stream servers.
	bScrollerManagerConcludedSuccessfully = CScrollerManager::IsValid();

	if (CScrollerManager::pTextStreamServer) {
		delete(CScrollerManager::pTextStreamServer);
	}

	if (CScrollerManager::pImageStreamServer) {
		delete(CScrollerManager::pImageStreamServer);
	}


	return(bScrollerManagerConcludedSuccessfully);
}


bool CScrollerManager::IncrementScroller()
{
	bool							bIncrementedSuccessfully = false;

	if (CScrollerManager::IsValid()) {
		// Increment the scroller position within the lead character...
		mCurrentScrollerPixelOffset += (RelativeQuantityType)CScrollerManager::mScrollRate;

		// ...Determine if the lead character index requires incrementing, and
		// perform the incrementing as appropriate.
		CScrollerManager::AdjustScrollerIndex();

		bIncrementedSuccessfully = true;
	}

	return(bIncrementedSuccessfully);
}


bool CScrollerManager::GetScrollerImageDataPointer(void*& pScrollerImageData)
{
	bool							bImagePointerRetrievedSuccessfully = false;

	std::string						scrollerString;

	// Retrieve the current scroller string..
	if (CScrollerManager::IsValid() && GetScrollerString(
		CScrollerManager::mCurrentScrollerTextIndex,
		CScrollerManager::mkScrollerStringSegmentLength, scrollerString))
	{
		// Retrieve a scroller image packet...
		CScrollerImageDataStreamRequest
									scrollerImageDataStreamRequest(scrollerString,
									CScrollerManager::mCurrentScrollerPixelOffset,
									CScrollerManager::mBackgroundColor,
									CScrollerManager::mScrollerTextColor);

		CDataStreamPacket			imageStreamPacket;
		CScrollerManager::pImageStreamServer->GetDataStreamPacket(scrollerImageDataStreamRequest,
			imageStreamPacket);

		// Retrieve a pointer to the image data.
		void*						pImageData = NULL;
		MemSizeType					imageDataSize = 0;
		imageStreamPacket.GetDataStreamPacketData(pImageData, imageDataSize);

		if (pImageData && imageDataSize > 0) {
			pScrollerImageData = pImageData;
			bImagePointerRetrievedSuccessfully = true;
		}
	}

	return(bImagePointerRetrievedSuccessfully);
}


// Returns the scroller image format (width, height, color depth).
bool CScrollerManager::GetScrollerImageFormat(QuantityType& imageWidth,
	QuantityType& imageHeight, QuantityType& bitDepth, QuantityType& imageDataSize)
{
	bool							bImageFormatRetrieved = false;

	const QuantityType				kBitsPerByte = 8;

	if (CScrollerManager::IsValid()) {
		// Return the image parameters.
		imageWidth = CScrollerManager::mImageWidth;
		imageHeight = CScrollerManager::mImageHeight;
		bitDepth = CScrollerManager::mkImageBitDepth;

		imageDataSize = CScrollerManager::mImageWidth * CScrollerManager::mImageHeight *
			(CScrollerManager::mkImageBitDepth / kBitsPerByte);

		bImageFormatRetrieved = true;
	}

	return(bImageFormatRetrieved);
}

void CScrollerManager::SetScrollerTextColor(const CFloatColor& scrollerTextColor)
{
	// Set the scroller text color.
	CScrollerManager::mScrollerTextColor = scrollerTextColor;
}


void CScrollerManager::SetScrollerBackgroundColor(const CFloatColor& backgroundColor)
{
	// Set the scroller background color.
	CScrollerManager::mBackgroundColor = backgroundColor;
}

bool CScrollerManager::IsValid()
{
	// Ensure that the stream servers have been created properly.
	return(CScrollerManager::pTextStreamServer && CScrollerManager::pImageStreamServer &&
		CScrollerManager::pTextStreamServer->IsValid() &&
		CScrollerManager::pImageStreamServer->IsValid());
}


void CScrollerManager::AdjustScrollerIndex()
{
	std::string						leadCharacterString;

	if (CScrollerManager::IsValid() &&
		CScrollerManager::GetScrollerString(CScrollerManager::mCurrentScrollerTextIndex,
		1, leadCharacterString))
	{
		// Determine the width of the lead character (pixels).
		const QuantityType			leadCharacterWidth =
			CScrollerManager::pImageStreamServer->GetScrollerTextWidth(leadCharacterString);

		// ...If the current pixel offset exceeds the bounds of the lead character, increment.
		// the lead character.
		if (mCurrentScrollerPixelOffset > (RelativeQuantityType)leadCharacterWidth) {
			mCurrentScrollerPixelOffset -= (RelativeQuantityType)leadCharacterWidth;
			CScrollerManager::mCurrentScrollerTextIndex++;

			// Adjust in the event that the current pixel offset exceeds the bounds of one
			// or more characters...
			CScrollerManager::AdjustScrollerIndex();
		}
	}
	else {
		// ...If no character could be retrieved, start the scroller from the beginning.
		CScrollerManager::mCurrentScrollerTextIndex = 0;
		CScrollerManager::ApplyLeadInPixelOffset();
	}
}


bool CScrollerManager::GetScrollerString(const MemIndexType stringIndex,
										 const MemSizeType stringLength,
										 std::string& scrollerString)
{
	bool							bStringRetrievedSuccessfully = false;

	if (CScrollerManager::IsValid()) {
		// Create a request object for the scroller string...
		CTextDataStreamRequest		textDataStreamRequest(
			stringIndex, stringLength);

		//Request the scroller string
		CDataStreamPacket			textDataStreamPacket;
		if (CScrollerManager::pTextStreamServer->GetDataStreamPacket(textDataStreamRequest,
			textDataStreamPacket))
		{
			MemSizeType				textStringSize = 0;
			void*					pPacketData = NULL;
			if (textDataStreamPacket.GetDataStreamPacketData(pPacketData, textStringSize)) {
				// Compose a string using requested text data.
				scrollerString.assign((const char*)pPacketData, textStringSize);

				bStringRetrievedSuccessfully = !scrollerString.empty();
			}
		}
	}

	return(bStringRetrievedSuccessfully);
}


void CScrollerManager::ApplyLeadInPixelOffset()
{
	// Default, minimal lead-in is the width of the image (additional padding can be added
	// as well).
	CScrollerManager::mCurrentScrollerPixelOffset =
		-(CScrollerManager::mImageWidth + CScrollerManager::mTextLeadInPadding);
}

CTextStreamServer*					CScrollerManager::pTextStreamServer = NULL;

CScrollerImageStreamServer*			CScrollerManager::pImageStreamServer = NULL;

MemSizeType							CScrollerManager::mCurrentScrollerTextIndex = 0;
	
RelativeQuantityType				CScrollerManager::mCurrentScrollerPixelOffset = 0;

QuantityType						CScrollerManager::mScrollRate;

const MemSizeType					CScrollerManager::mkScrollerStringSegmentLength = 25;

QuantityType						CScrollerManager::mImageWidth = 0;

QuantityType						CScrollerManager::mImageHeight = 0;

const QuantityType					CScrollerManager::mkImageBitDepth = 32;

CFloatColor							CScrollerManager::mScrollerTextColor = CFloatColor(0.0, 0.0, 0.0);

CFloatColor							CScrollerManager::mBackgroundColor = CFloatColor(0.0, 0.0, 0.0);

const QuantityType					CScrollerManager::mTextLeadInPadding = 100;