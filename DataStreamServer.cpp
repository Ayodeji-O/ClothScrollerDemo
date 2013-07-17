// DataStreamServer.cpp

#include "DataStreamServer.h"




/////
// CDataStreamInitialize class
/////
CDataStreamInitialize::CDataStreamInitialize(const EDataStreamType dataStreamType) :
	mDataStreamType(dataStreamType)
{

}

CDataStreamInitialize::~CDataStreamInitialize()
{

}

 
EDataStreamType	CDataStreamInitialize::GetDataStreamType() const
{
	// Return the assigned data stream type (derived class should pass this value to
	// the superclass upon construction).
	return(this->mDataStreamType);
}


/////
// CTextStreamInitialize class
/////

CTextStreamInitialize::CTextStreamInitialize(std::string fileSpec, const bool bUseHash) :
	CDataStreamInitialize(eDataStreamText), mFileSpec(fileSpec), mbValidateWithHash(bUseHash)
{

}

CTextStreamInitialize::~CTextStreamInitialize()
{

}

bool CTextStreamInitialize::IsValid() const
{
	// The file specification string must not be a NULL-length string.
	return(this->mFileSpec.length() > 0);
}

std::string	 CTextStreamInitialize::GetFileSpec() const
{
	// Return the [full] file specification.
	return(this->mFileSpec);
}


// Determines whether or not to validate the string with the postfixed hash.
bool CTextStreamInitialize::ValidateWithHash() const
{
	// Return the flag that indicates whether or not the string should be validated against a
	// hash value (if the validation fails, the server will not return data).
	return(this->mbValidateWithHash);
}


/////
// CScrollerImageStreamInitialize class
/////

CScrollerImageStreamInitialize::CScrollerImageStreamInitialize(const QuantityType imageWidth,
															   const QuantityType imageHeight,
															   const QuantityType bitDepth,
															   const QuantityType colorPlanes) :
	CDataStreamInitialize(eDataStreamScollerImage), mImageWidth(imageWidth), mImageHeight(imageHeight), mBitDepth(bitDepth),
		mColorPlanes(colorPlanes)
{

}

CScrollerImageStreamInitialize::~CScrollerImageStreamInitialize()
{

}

bool CScrollerImageStreamInitialize::IsValid() const
{
	// Image parameters must result in the creation of an image with
	// a finte size.
	return((this->GetImageWidth() > 0) && (this->GetImageHeight() > 0) &&
		(this->GetBitDepth() > 0) && (this->GetColorPlanes() > 0));

}

QuantityType CScrollerImageStreamInitialize::GetImageWidth() const
{
	// Return the image width.
	return(this->mImageWidth);
}

QuantityType CScrollerImageStreamInitialize::GetImageHeight() const
{
	// Return the image height.
	return(this->mImageHeight);
}

QuantityType CScrollerImageStreamInitialize::GetBitDepth() const
{
	// Return the bit depth.
	return(this->mBitDepth);
}

	
QuantityType CScrollerImageStreamInitialize::GetColorPlanes() const
{
	// Return the color planes.
	return(this->mColorPlanes);
}


/////
// CDataStreamRequest class
/////

CDataStreamRequest::CDataStreamRequest(const EDataStreamType dataStreamType) :
	mDataStreamRequestType(dataStreamType)
{

}

CDataStreamRequest::~CDataStreamRequest()
{

}


EDataStreamType	CDataStreamRequest::GetDataStreamType() const
{
	// Return the type that corresponds to the data stream request.
	return(this->mDataStreamRequestType);
}


/////
// CTextDataStreamRequest class
/////

CTextDataStreamRequest::CTextDataStreamRequest(const MemIndexType startIndex,
											   const MemSizeType stringSize) :
	CDataStreamRequest(eDataStreamText), mStartCharacterIndex(startIndex),
		mStringSize(stringSize)
{

}

CTextDataStreamRequest::~CTextDataStreamRequest()
{

}


bool CTextDataStreamRequest::IsValid() const
{
	// The text string size must be greater than zero.
	return(this->mStringSize > 0);
}

// Retrieves the data that describes the text stream request.
bool CTextDataStreamRequest::GetTextStreamRequestInfo(MemIndexType& startCharacterIndex,
													  MemSizeType& stringLength) const
{
	bool							bInfoRetrievedSuccessfully = false;

	if (this->IsValid()) {
		// Return the requested information.
		startCharacterIndex = this->mStartCharacterIndex;
		stringLength = this->mStringSize;

		bInfoRetrievedSuccessfully = true;
	}

	return(bInfoRetrievedSuccessfully);
}


/////
// CScrollerImageDataStreamRequest class
/////

CScrollerImageDataStreamRequest::CScrollerImageDataStreamRequest(const std::string& scrollerText,
																 const RelativeQuantityType pixelOffset,
																 const CFloatColor& backgroundColor,
																 const CFloatColor& textColor) :
	CDataStreamRequest(eDataStreamScollerImage), mScrollerTextString(scrollerText), mPixelOffset(pixelOffset),
		mBackgroundColor(backgroundColor), mTextColor(textColor)
{

}

CScrollerImageDataStreamRequest::~CScrollerImageDataStreamRequest()
{

}

bool CScrollerImageDataStreamRequest::IsValid() const
{
	// Ensure that the length of the string is greater than zero.
	return(this->mScrollerTextString.size() > 0);
}

bool CScrollerImageDataStreamRequest::GetScrollerImageRequestInfo(std::string& scrollerTextString,
																  RelativeQuantityType& pixelOffset,
																  CFloatColor& backgroundColor,
																  CFloatColor& textColor) const
{
	bool							bInfoRetrievedSuccessfully = false;

	if (this->IsValid()) {
		// Return the requested information.
		scrollerTextString = this->mScrollerTextString;
		pixelOffset = this->mPixelOffset;
		backgroundColor = this->mBackgroundColor;
		textColor = this->mTextColor;

		bInfoRetrievedSuccessfully = true;
	}

	return(bInfoRetrievedSuccessfully);
}


/////
// CDataStreamDataTransfer
/////

CDataStreamDataTransfer::CDataStreamDataTransfer()
{

}

CDataStreamDataTransfer::~CDataStreamDataTransfer()
{

}


CDataStreamDataTransfer::CGenericDataContainer::CGenericDataContainer() :
	mGenericDataSize(0), mDataStreamType(eDataStreamUndefined)
{

}

CDataStreamDataTransfer::CGenericDataContainer::CGenericDataContainer(void* pGenericData,
																	  const MemSizeType
																	  genericDataSize,
																	  const EDataStreamType
																	  dataStreamType) :
	mpGenericData(pGenericData), mGenericDataSize(genericDataSize),
		mDataStreamType(dataStreamType)
{

}

CDataStreamDataTransfer::CGenericDataContainer::~CGenericDataContainer()
{

}


void* CDataStreamDataTransfer::CGenericDataContainer::GetGenericDataPointer()
{
	// Return the data pointer.
	return(this->mpGenericData);
}

MemSizeType CDataStreamDataTransfer::CGenericDataContainer::GetGenericDataSize() const
{
	// Return the data size.
	return(this->mGenericDataSize);
}

EDataStreamType CDataStreamDataTransfer::CGenericDataContainer::GetDataStreamType() const
{
	// Return the data stream type.
	return(this->mDataStreamType);
}


/////
// CDataStreamPacket class
/////

CDataStreamPacket::CDataStreamPacket() : mbMemoryOwnedByPacket(false), mpDataStreamPacketData(NULL),
	mDataStreamPacketDataSize(0)
{

}


CDataStreamPacket::~CDataStreamPacket()
{
	// Release memory allocated by the packet object as necessary.
	if (this->mbMemoryOwnedByPacket) {
		this->FreeMemory();
	}
}


bool CDataStreamPacket::GetDataStreamPacketData(void*& pDataStreamPacketData,
		 MemSizeType& dataStreamPacketDataSize)
{
	bool							bDataRetrievedSuccessfully = false;

	if (this->IsValid()) {
		// Return the packet data size and pointer.
		pDataStreamPacketData = this->mpDataStreamPacketData;
		dataStreamPacketDataSize = this->mDataStreamPacketDataSize;

		bDataRetrievedSuccessfully = true;
	}

	return(bDataRetrievedSuccessfully);
}


bool CDataStreamPacket::SetDataStreamPacketData(CGenericDataContainer&
	genericDataContainer, const bool bUseLocalCopy)
{
	bool							bStreamDataSetSuccessfully = false;

	// If allocated memory exists, de-allocate the memory first.
	if ((this->mbMemoryOwnedByPacket && this->mpDataStreamPacketData && this->FreeMemory()) ||
		!this->mbMemoryOwnedByPacket)
	{
		// Allocate memory for local storage if specified...
		if (bUseLocalCopy && this->AllocateMemory(genericDataContainer.GetGenericDataSize())) {
			const void*					pSourceMemoryPointer =
				genericDataContainer.GetGenericDataPointer();
			
			if (pSourceMemoryPointer) {
				// ...Copy the memory from the data stream into the local storage area.
				::memcpy(this->mpDataStreamPacketData, pSourceMemoryPointer,
					genericDataContainer.GetGenericDataSize());


				bStreamDataSetSuccessfully = true;
			}
		}
		else {
			// Copy the packet data pointer to local memory.
			this->mpDataStreamPacketData = genericDataContainer.GetGenericDataPointer();
			
			bStreamDataSetSuccessfully = (this->mpDataStreamPacketData != NULL);
		}

		if (bStreamDataSetSuccessfully) {
			// Store the size of the data.
			this->mDataStreamPacketDataSize = genericDataContainer.GetGenericDataSize();
		}
		else {
			// Indicate that no data is available.
			this->mDataStreamPacketDataSize = 0;
		}
	}


	return(bStreamDataSetSuccessfully);
}



bool CDataStreamPacket::IsValid() const
{
	// The data stream packet data pointer must be valid, and the size of the data must
	// be greater than zero.
	return(this->mpDataStreamPacketData && (this->mDataStreamPacketDataSize > 0));
}


bool CDataStreamPacket::AllocateMemory(const MemSizeType& requiredMemory)
{
	bool							bAllocatedSuccessfully = false;

	if (!this->mpDataStreamPacketData) {
		// Attempt to allocate the requested amount of memory.
		void*						pMemory = ::malloc(requiredMemory);

		if (pMemory) {
			this->mpDataStreamPacketData = pMemory;
			bAllocatedSuccessfully = true;
		}
	}

	return(bAllocatedSuccessfully);
}

bool CDataStreamPacket::FreeMemory()
{
	bool							bMemoryFreedSuccessfully = false;

	if (this->mpDataStreamPacketData && this->mbMemoryOwnedByPacket) {
		// Release the allocated memory;
		::free(this->mpDataStreamPacketData);
		this->mpDataStreamPacketData = NULL;

		bMemoryFreedSuccessfully = true;
	}

	return(bMemoryFreedSuccessfully);
}

/////
// CDataStreamServer
/////

CDataStreamServer::CDataStreamServer(const EDataStreamType dataStreamServerType,
									 const CDataStreamInitialize& initializationData) :
	mDataStreamServerType(dataStreamServerType)
{


}


CDataStreamServer::~CDataStreamServer()
{

}

EDataStreamType CDataStreamServer::GetDataStreamType() const
{
	// Return the data stream server type.
	return(this->mDataStreamServerType);
}


bool CDataStreamServer::VerifyDataStreamRequestType(const CDataStreamRequest&
													dataStreamRequest) const
{
	// Ensure that the data stream request type matches the server type.
	return((dataStreamRequest.GetDataStreamType() == this->GetDataStreamType()) &&
		dataStreamRequest.IsValid());
}

/////
// CTextStreamServer class
/////

CTextStreamServer::CTextStreamServer(const CDataStreamInitialize& initializationData) :
		CDataStreamServer(eDataStreamText, initializationData), mpFileAccess(NULL),
			mpFileBuffer(NULL), mCurrentBufferIndexInFile(0), mTrueBufferLength(0),
			mbUsingHash(false), mbMessageValidated(false)
{
	// Initialize the text stream server.
	::AssertExpression(this->InitializeDataStreamServer(initializationData));
}

CTextStreamServer::~CTextStreamServer()
{
	// Release the buffer memory...
	this->FreeFileBuffer();

	// Close the open file handle.
	if (this->mpFileAccess)
	{
		::fclose(this->mpFileAccess);
		this->mpFileAccess = NULL;
	}
}


bool CTextStreamServer::IsValid() const
{
	// Ensure that a file access object pointer exists, and that the data buffer size
	// is greater than zero.
	return(this->mpFileAccess && this->mpFileBuffer && (CTextStreamServer::mkDataBufferSize > 0));
}

bool CTextStreamServer::GetDataStreamPacket(const CDataStreamRequest&
											dataStreamRequest, CDataStreamPacket& dataStreamPacket)
{
	bool							bStreamPacketRetrieved = false;

	// Ensure that the packet type is a text stream request; also ensure that the message
	// has been validated if a hash is in use...
	if (this->VerifyDataStreamRequestType(dataStreamRequest) && (!this->IsUsingHash() ||
		this->mbMessageValidated))
	{
		bool						bRequestedTextAvailable = false;

		CTextDataStreamRequest&		textStreamRequest = (CTextDataStreamRequest&)dataStreamRequest;

		// If a hash is in use, the hash is present at the begining of the file - offset the 
		// file pointer by an amount equivalent to the size of the hash to prevent reading
		// the hash as part of the message text.
		const QuantityType			fileIndexOffset = this->IsUsingHash() ? 
			CTextStreamServer::mkHashSize : 0;

		// Determine the amount of text data requested, in addition to the starting index into the
		// file
		MemIndexType				startCharacterIndex = 0;
		MemIndexType				requestLength = 0;
		MemIndexType				actualLength = 0;
		if (textStreamRequest.GetTextStreamRequestInfo(startCharacterIndex, requestLength) &&
			this->mpFileBuffer && (requestLength < CTextStreamServer::mkDataBufferSize))
		{
			// Attempt to determine if the requested text string is already in the file buffer;
			// in this case, reading from the file will not need to occur.
			if ((startCharacterIndex >= this->mCurrentBufferIndexInFile) &&
				((startCharacterIndex + requestLength) <
				(this->mCurrentBufferIndexInFile + this->mTrueBufferLength)))
			{
				actualLength = requestLength;
				bRequestedTextAvailable = true;
			} 
			else if (this->mpFileAccess && 
				(::fseek(this->mpFileAccess, (startCharacterIndex + fileIndexOffset), SEEK_SET) == 0))
			{
				// ...Request either does not exist in the buffer, or is not present in 
				// the buffer in its entirety. Load the data into the buffer.
				this->mTrueBufferLength = ::fread(this->mpFileBuffer, 1,
					CTextStreamServer::mkDataBufferSize, this->mpFileAccess);

				if (this->mTrueBufferLength > 0)
				{
					// Store the current index that corresponds to the buffer offset within the
					// file.
					this->mCurrentBufferIndexInFile = startCharacterIndex;

					actualLength = this->mTrueBufferLength;

					bRequestedTextAvailable = true;
				}
			}
		}
		else {
			// Request size exceeded buffer size.
		}


		if (bRequestedTextAvailable) {
			// Request suceeded - return the  packet data.
			CDataStreamDataTransfer::CGenericDataContainer
								textContainer(((char*)this->mpFileBuffer + 
								(startCharacterIndex - this->mCurrentBufferIndexInFile)),
								actualLength, eDataStreamText);

			bStreamPacketRetrieved = dataStreamPacket.SetDataStreamPacketData(textContainer, true);
		}
	}

	return(bStreamPacketRetrieved);
}


bool CTextStreamServer::InitializeDataStreamServer(const CDataStreamInitialize&
												   initializationData)
{
	bool							bServerInitializedSuccessfully = false;

	// Ensure that the object is a text stream initialization object..
	const bool						bValidTextStreamInitObject =
		initializationData.IsValid() &&
		(initializationData.GetDataStreamType() == eDataStreamText) &&
		dynamic_cast<const CTextStreamInitialize*>(&initializationData);

	if (bValidTextStreamInitObject) {
		// Initialize the text stream by creating a file, and storing the
		// file access object.
		const CTextStreamInitialize&
									textStreamInitialize =
									(const CTextStreamInitialize&)initializationData;						

		// Open the file.
		const std::string			kFileAccessMode = "rb";
		std::string					fileSpecification = textStreamInitialize.GetFileSpec();
		this->mpFileAccess = ::fopen(fileSpecification.c_str(), kFileAccessMode.c_str());

		// Allocate file read buffer memory.
		bServerInitializedSuccessfully	= (this->mpFileAccess != NULL) &&
			this->AllocateFileBuffer();

		if (bServerInitializedSuccessfully && textStreamInitialize.ValidateWithHash()) {
			this->mbUsingHash = textStreamInitialize.ValidateWithHash();
			// ... If a hash is being used to validate the message file, perform the validation
			// here.
			this->ValidateMessageFile();
		}
	}


	return(bServerInitializedSuccessfully);
}



bool CTextStreamServer::AllocateFileBuffer()
{
	// Allocate memory for the file read buffer...
	this->mpFileBuffer = ::malloc(CTextStreamServer::mkDataBufferSize);

	return(this->mpFileBuffer != NULL);
}

	// Frees memory used for the file buffer.
bool CTextStreamServer::FreeFileBuffer()
{
	// Release the memory that was used for the file buffer.
	::free(this->mpFileBuffer);
	this->mpFileBuffer = NULL;

	return(true);
}


// Creates a simple hash value from two bytes.
char CTextStreamServer::SimpleLocalHash(char targetByte, char neighborByte)
{
	// http://www.burtleburtle.net/bob/hash/
	// (Knuth's "The Art of Computer Programming", volume 3 "Sorting and Searching", chapter 6.4)
	char							localHash = 0;

	const unsigned int				firstShift = 2;
	const unsigned int				secondShift = 3;	// Originally 27

	localHash = ((targetByte << firstShift) ^ (targetByte >> secondShift)) ^ (neighborByte++);

	return(localHash);
}

// Performs a hash "round" on a 16-byte data block.
bool CTextStreamServer::HashRound(unsigned long hashIntermediates[NUM_HASH_INTERMEDIATES], const void* pDataBlock,
								  const QuantityType multiplier)
{
	bool							bRoundComputedSuccessfully = false;

	if (pDataBlock && (multiplier != 0)) {

		const char*					pInData = (const char*)pDataBlock;

		// Hash "round" (16 bytes) - Adapted from
		// http://www.cs.technion.ac.il/~biham/Reports/Tiger/y
		const unsigned long			primeTable[16] = { 
			// Prime numbers obtained from:
			// http://www.prime-numbers.org
			9257,		1855519,	2958503,	1001750039,				
			1000000007, 1000003519, 2000004359, 2010751433,
			2010759071, 3000253777, 3000261653, 3501650023,
			3802051789, 4002954971, 4102703677, 4202950013
		};


		hashIntermediates[3] ^= (unsigned long)(pInData[0] ^ pInData[2] ^ pInData[4] ^ pInData[6]) ^
			primeTable[pInData[0] & 0x0f] ^ primeTable[pInData[2] & 0x0f] ^
			primeTable[pInData[4] & 0x0f] ^ primeTable[pInData[6] & 0x0f];
		hashIntermediates[0] -= (unsigned long)(pInData[1] ^ pInData[3] ^ pInData[5] ^ pInData[7]) *
			(primeTable[pInData[1] & 0x0f] ^ primeTable[pInData[3] & 0x0f] ^
			primeTable[pInData[5] & 0x0f] ^ primeTable[pInData[7] & 0x0f] ^
			SimpleLocalHash(primeTable[pInData[2] & 0x0f], primeTable[pInData[6] & 0x0f]));
		hashIntermediates[1] += (unsigned long)(pInData[8] ^ pInData[10] ^ pInData[12] ^ pInData[14]) *
			(primeTable[pInData[8] & 0x0f] ^ primeTable[pInData[10] & 0x0f] ^
			primeTable[pInData[12] & 0x0f] ^ primeTable[pInData[14] & 0x0f] ^
			primeTable[((unsigned long)SimpleLocalHash(pInData[8], pInData[10]) +
			(unsigned long)SimpleLocalHash(pInData[14], pInData[12])) & 0x0f]);
		hashIntermediates[2] ^= ((unsigned long)(pInData[9] ^ pInData[11] ^ pInData[13] ^ pInData[15]) ^
			primeTable[pInData[9] & 0x0f] ^ primeTable[pInData[11] & 0x0f] ^
			primeTable[pInData[13] & 0x0f] ^ primeTable[pInData[15] & 0x0f] ^
			primeTable[pInData[14] & 0x0f]) * multiplier;

		bRoundComputedSuccessfully = true;
	}

	return(bRoundComputedSuccessfully);
}

// Computes the hash for the text file.
bool CTextStreamServer::ComputeFileHash(void* pHashBuffer)
{
	bool							bHashComputedSuccessfully = false;

	// Buffer size - 64 bytes.
	const unsigned long				hashBufferSize = 64;

	// "Sub-buffer" size - 16 bytes.
	const unsigned long				hashRoundSubBufferSize = 16;

	// Iterations required to process an entire buffer.
	const unsigned long				bufferIterations = hashBufferSize /
		hashRoundSubBufferSize;

	// Buffer initialization constant = digits of Euler's number.
	const unsigned long				bufferInitData[16] = {
		// Euler's number (e) digits obtained from:
		// http://antwrp.gsfc.nasa.gov/htmltest/gifcity/e.2mil
		2718281828,	459045235,	3602874713,	526624977,
		572470936,	999595749,	669676277,	2407663035,
		3547594571,	3821785251,	664274274,	6639193,
		200305992,  181741,		359662904,	357290033
	};

	const const QuantityType		multiplier = 17;

	// "Intermediate" hash values - these values will ultimately
	// be used to determine the final hash value.
	unsigned long hashIntermediates[NUM_HASH_INTERMEDIATES] = {
		1700003491,
		2117061241,
		3333300367,
		4127628919
	};

	// Seek to the beginning of the file, just beyond the hash...
	if (::AssertExpression(hashBufferSize == sizeof(bufferInitData)) && pHashBuffer &&
		this->IsValid() &&
		(::fseek(this->mpFileAccess, CTextStreamServer::mkHashSize, SEEK_SET) == 0))
	{
		void*						pFileBlockBuffer = ::malloc(hashBufferSize);

		if (pFileBlockBuffer)
		{
			::memcpy(pFileBlockBuffer, bufferInitData, sizeof(bufferInitData));
			while (::fread(pFileBlockBuffer, 1, hashBufferSize, this->mpFileAccess) > 0) {

				for (unsigned int readBlockLoop = 0; readBlockLoop < bufferIterations;
					readBlockLoop++)
				{
					// Perform four hashing rounds, "rotating" the list of intermediates 
					// during each round.
					this->HashRound(hashIntermediates,
						(char*)pFileBlockBuffer + (readBlockLoop * hashRoundSubBufferSize),
						multiplier);
					this->RotateIntermediatesRight(hashIntermediates);
					this->HashRound(hashIntermediates,
						(char*)pFileBlockBuffer + (readBlockLoop * hashRoundSubBufferSize),
						multiplier);
					this->RotateIntermediatesRight(hashIntermediates);
					this->HashRound(hashIntermediates,
						(char*)pFileBlockBuffer + (readBlockLoop * hashRoundSubBufferSize),
						multiplier);
					this->RotateIntermediatesRight(hashIntermediates);
					this->HashRound(hashIntermediates,
						(char*)pFileBlockBuffer + (readBlockLoop * hashRoundSubBufferSize),
						multiplier);
					this->RotateIntermediatesRight(hashIntermediates);
				}

				::memcpy(pFileBlockBuffer, bufferInitData, sizeof(bufferInitData));
			}

			// Compute the final hash.
			unsigned long hashHigh = hashIntermediates[0] ^ hashIntermediates[3];
			unsigned long hashLow = hashIntermediates[1] ^ hashIntermediates[2];

			// Store the hash.
			((char*)pHashBuffer)[0] = (hashHigh >> 24) & 0xff;
			((char*)pHashBuffer)[1] = (hashHigh >> 16) & 0xff;
			((char*)pHashBuffer)[2] = (hashHigh >> 8) & 0xff;
			((char*)pHashBuffer)[3] = hashHigh & 0xff;
			((char*)pHashBuffer)[4] = (hashLow >> 24) & 0xff;
			((char*)pHashBuffer)[5] = (hashLow >> 16) & 0xff;
			((char*)pHashBuffer)[6] = (hashLow >> 8) & 0xff;
			((char*)pHashBuffer)[7] = hashLow & 0xff;

			bHashComputedSuccessfully = true;

			// Release memory allocated for the file buffer.
			::free(pFileBlockBuffer);
		}

	}

	return(bHashComputedSuccessfully);
}


void CTextStreamServer::RotateIntermediatesRight(unsigned long hashIntermediates[NUM_HASH_INTERMEDIATES])
{
	unsigned long tempIntermediate = hashIntermediates[NUM_HASH_INTERMEDIATES - 1];

	// Shift all intermediate values right...
	for(long intermediateIndex = (NUM_HASH_INTERMEDIATES - 1); intermediateIndex > 0;
		intermediateIndex--)
	{
		hashIntermediates[intermediateIndex] = hashIntermediates[intermediateIndex - 1];
	}

	hashIntermediates[0] = tempIntermediate;


}

// Validates the message file against the hash resident in the file.
bool CTextStreamServer::ValidateMessageFile()
{
	bool							bMessageFileValidated = true;

	char*							pStoredFileHashBuffer = 
		new char[CTextStreamServer::mkHashSize];
	char*							pTrueFileHashBuffer = 
		new char[CTextStreamServer::mkHashSize];

	if (pStoredFileHashBuffer && pTrueFileHashBuffer && this->IsValid() &&
		this->ComputeFileHash(pTrueFileHashBuffer) && (::fseek(this->mpFileAccess, 0, SEEK_SET) == 0))
	{
		::fread(pStoredFileHashBuffer, 1, CTextStreamServer::mkHashSize, this->mpFileAccess);

		// Compare the hash stored in the file with the computed hash - if the values
		// are equivalent, the initialization was successful.
		bMessageFileValidated = (::memcmp(pStoredFileHashBuffer, pTrueFileHashBuffer,
			CTextStreamServer::mkHashSize) == 0);

		this->mbMessageValidated = bMessageFileValidated;
	}

	

	return(bMessageFileValidated);
}


bool CTextStreamServer::IsUsingHash() const
{
	// Return the hash usage flag (when set to true, a valid hash must prefix the
	// file; otherwise, the message file cannot be used).
	return(this->mbUsingHash);
}

const QuantityType					CTextStreamServer::mkDataBufferSize = 256;
	// 8-byte hash.
const QuantityType					CTextStreamServer::mkHashSize = 8;

/////
// CScrollerImageStreamServer class
/////

CScrollerImageStreamServer::CScrollerImageStreamServer(const CDataStreamInitialize&
													   initializationData) :
	CDataStreamServer(eDataStreamScollerImage, initializationData), 
	mhScrollerImageDC(NULL), mpScrollerImageDIBData(NULL), mhScrollerImageBitmap(NULL),
		mhOldDCBitmap(NULL), mhScrollerFont(NULL), mhOldDCFont(NULL), mImageWidth(NULL),
		mImageHeight(NULL)
{
	::AssertExpression(this->InitializeDataStreamServer(initializationData));
}


CScrollerImageStreamServer::~CScrollerImageStreamServer()
{

	if (this->mhScrollerImageDC) {
		// Restore the state of the device context...
		HGDIOBJ							hScrollerImage =
			::SelectObject(this->mhScrollerImageDC, this->mhOldDCBitmap);
		HGDIOBJ							hScrollerFont = 
			::SelectObject(this->mhScrollerImageDC, this->mhOldDCFont);

		// Delete previously-created device context objects...
		::DeleteObject(hScrollerImage);
		::DeleteObject(hScrollerFont);


		// ...Delete the device context.
		::DeleteDC(this->mhScrollerImageDC);
	}
}

bool CScrollerImageStreamServer::IsValid() const
{

	return(this->mpScrollerImageDIBData && this->mhScrollerFont && this->mhScrollerImageBitmap &&
		this->mhScrollerImageDC && (this->mImageHeight > 0) && (this->mImageWidth > 0));
}

bool CScrollerImageStreamServer::GetDataStreamPacket(const CDataStreamRequest&
		dataStreamRequest, CDataStreamPacket& dataStreamPacket)
{
	bool							bStreamPacketRetrieved = false;

	// Maximum value of a GDI color component.
	const QuantityType				kMaxGDIColorComponent = 255;

	// Number of bits in one byte.
	const QuantityType				kBitsPerByte = 32;

	if (this->VerifyDataStreamRequestType(dataStreamRequest)) {
		// Retrieve the image generation request parameters...
		CScrollerImageDataStreamRequest&
									scrollerImageDataStreamRequest =
									(CScrollerImageDataStreamRequest&)dataStreamRequest;

		std::string					scrollerTextString;
		RelativeQuantityType		pixelOffset;
		CFloatColor					backgroundFloatColor(0.0, 0.0, 0.0);
		CFloatColor					textFloatColor(0.0, 0.0, 0.0);

		if (scrollerImageDataStreamRequest.GetScrollerImageRequestInfo(scrollerTextString,
			pixelOffset, backgroundFloatColor, textFloatColor))
		{
			RECT					textRect = { -pixelOffset, 0, (this->mImageWidth - 1),
				(this->mImageHeight - 1) };
			RECT					textBackgroundRect = { 0, 0, (this->mImageWidth - 1),
				(this->mImageHeight - 1) };
		
			// Convert the floating point color intensities to GDI-compatible values.

			// Background color.
			const COLORREF			backgroundColor = RGB(
				(QuantityType)(backgroundFloatColor.GetRedValue() *
				(ScalarType)kMaxGDIColorComponent),
				(QuantityType)(backgroundFloatColor.GetGreenValue() *
				(ScalarType)kMaxGDIColorComponent),
				(QuantityType)(backgroundFloatColor.GetBlueValue() *
				(ScalarType)kMaxGDIColorComponent));
				
			// Text color.
			const COLORREF			textColor = RGB(
				(QuantityType)(textFloatColor.GetRedValue() *
				(ScalarType)kMaxGDIColorComponent),
				(QuantityType)(textFloatColor.GetGreenValue() *
				(ScalarType)kMaxGDIColorComponent),
				(QuantityType)(textFloatColor.GetBlueValue() *
				(ScalarType)kMaxGDIColorComponent));

			// Draw a solid-colored background...
			HBRUSH					hBackgroundBrush = ::CreateSolidBrush(backgroundColor);

			if (hBackgroundBrush) {
				::FillRect(this->mhScrollerImageDC, &textBackgroundRect, hBackgroundBrush);
				::DeleteObject((HGDIOBJ)hBackgroundBrush);
			}

			// Draw the scroller text onto the image.
			::SetBkMode(this->mhScrollerImageDC, TRANSPARENT);
			::SetTextColor(this->mhScrollerImageDC, textColor);
			::DrawText(this->mhScrollerImageDC, scrollerTextString.c_str(),
				scrollerTextString.length(), &textRect, DT_SINGLELINE);

			// ...Return a pointer to the image data.
			CDataStreamDataTransfer::CGenericDataContainer imageContainer(
				this->mpScrollerImageDIBData, this->mImageWidth * this->mImageHeight *
				(this->mImageBitDepth / kBitsPerByte), eDataStreamScollerImage);
	
			dataStreamPacket.SetDataStreamPacketData(imageContainer, false);
		}
	}


	return(bStreamPacketRetrieved);
}

QuantityType CScrollerImageStreamServer::GetScrollerTextWidth(const std::string&
															  scrollerText) const
{
	QuantityType					textWidth = 0;

	// Will contain the required text dimensions.
	RECT							textOutputRect = {0, 0, 0, 0};

	if (this->IsValid()) {
		// Determine the dimensions (pixels) required to draw the specified string in its
		// entirety.
		if (::DrawText(this->mhScrollerImageDC, scrollerText.c_str(), scrollerText.length(),
			&textOutputRect, DT_CALCRECT) > 0)
		{
			// If the dimensions have been determined successfully, compute the required
			// string width.
			textWidth = textOutputRect.right - textOutputRect.left;
		}
	}

	return(textWidth);
}

bool CScrollerImageStreamServer::InitializeDataStreamServer(const CDataStreamInitialize&
															initializationData)
{
	bool							bServerInitializedSuccessfully = false;

	// Ensure that the object is a scroller image stream initialization object.
	const bool						bValidScrollerStreamInitObject =
		initializationData.IsValid() &&
		(initializationData.GetDataStreamType() == eDataStreamScollerImage) &&
		dynamic_cast<const CScrollerImageStreamInitialize*>(&initializationData);

	if (bValidScrollerStreamInitObject) {

		const CScrollerImageStreamInitialize&
									scrollerStreamInitialize =
									(const CScrollerImageStreamInitialize&)initializationData;

		// Store the image dimensions/parameters.
		this->mImageWidth = scrollerStreamInitialize.GetImageWidth();
		this->mImageHeight = scrollerStreamInitialize.GetImageHeight();
		this->mImageBitDepth = scrollerStreamInitialize.GetBitDepth();

		// Create the scroller image device context.
		HDC							hScrollerDC = ::CreateCompatibleDC(NULL);

		if (hScrollerDC) {

			const int				kBitsPerByte = 8;

			this->mhScrollerImageDC = hScrollerDC;

			// Create a bitmap that will be used to access the scroller text image
			// data.
			BITMAPINFO					scrollerBitmapInfo;
			::memset(&scrollerBitmapInfo, 0, sizeof(scrollerBitmapInfo));

			scrollerBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			scrollerBitmapInfo.bmiHeader.biWidth = scrollerStreamInitialize.GetImageWidth();
			scrollerBitmapInfo.bmiHeader.biHeight = scrollerStreamInitialize.GetImageHeight();
			scrollerBitmapInfo.bmiHeader.biPlanes = scrollerStreamInitialize.GetColorPlanes();
			scrollerBitmapInfo.bmiHeader.biBitCount = scrollerStreamInitialize.GetBitDepth();
			scrollerBitmapInfo.bmiHeader.biCompression = BI_RGB;
			scrollerBitmapInfo.bmiHeader.biSizeImage = scrollerStreamInitialize.GetImageWidth() *
				scrollerStreamInitialize.GetImageHeight() *
				(scrollerStreamInitialize.GetBitDepth() / kBitsPerByte);

			void*					pScrollerImageBits = NULL;

			HBITMAP					hScrollerBitmap = 
				::CreateDIBSection(this->mhScrollerImageDC, &scrollerBitmapInfo, DIB_RGB_COLORS,
				&pScrollerImageBits, NULL, 0);

			if (hScrollerBitmap && pScrollerImageBits) {
				// Store the bitmap handle, and the pointer to the image data...
				this->mhScrollerImageBitmap = hScrollerBitmap;
				this->mpScrollerImageDIBData = pScrollerImageBits;

				this->mhOldDCBitmap = (HBITMAP)::SelectObject(this->mhScrollerImageDC,
					hScrollerBitmap);

				// Create the font that will be used to draw the scroller text.
				::SetMapMode(this->mhScrollerImageDC, MM_TEXT);

				LOGFONT				scrollerFont;
				::memset(&scrollerFont, 0, sizeof(scrollerFont));

				const QuantityType	kCharactersPerLine = 15;

				scrollerFont.lfHeight = scrollerStreamInitialize.GetImageHeight();
				scrollerFont.lfWidth = scrollerStreamInitialize.GetImageWidth() / kCharactersPerLine;
				scrollerFont.lfWeight = FW_NORMAL;
				scrollerFont.lfCharSet = DEFAULT_CHARSET;
				scrollerFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
				scrollerFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
				scrollerFont.lfQuality = ANTIALIASED_QUALITY;
				scrollerFont.lfPitchAndFamily = DEFAULT_PITCH | FF_MODERN;

				const char			kFaceName[] = "Arial";

				::strcpy(scrollerFont.lfFaceName, kFaceName);

				HFONT				hScrollerFont = ::CreateFontIndirect(&scrollerFont);

				if (hScrollerFont) {
					// Store the font handle.
					this->mhScrollerFont = hScrollerFont;
					this->mhOldDCFont = (HFONT)::SelectObject(this->mhScrollerImageDC,
						this->mhScrollerFont);

					bServerInitializedSuccessfully = true;
				}
			}
			else {
				::DeleteObject(hScrollerBitmap);
			}
		}
	}


	return(bServerInitializedSuccessfully);
}

