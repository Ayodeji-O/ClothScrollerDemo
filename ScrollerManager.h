// ScrollerManager.h - Singleton class that manages the position of the cloth scroller text, and
//                     coordinates communication between clients and the required scroller
//                     servers.


#ifndef _SCROLLERMANAGER_H
#define _SCROLLERMANAGER_H 1

#include "DataStreamServer.h"



class CScrollerManager
{
public:

	// Initializes the scroller manager - the function must be called before the scroller manager
	// services are used.
	static bool						InitializeScrollerManager(const std::string& messageFile,
		const QuantityType scrollerImageWidth, const QuantityType scrollerImageHeight,
		const QuantityType scrollRate);

	// Performs any actions required to conclude usage of the scroller manager.
	static bool						ConcludeScrollerManager();

	// Performs one scroller scroll step.
	static bool						IncrementScroller();

	// Retrieves a pointer to the current scroller image data (RGB data).
	static bool						GetScrollerImageDataPointer(void*& pScrollerImageData);

	// Returns the scroller image format (width, height, color depth).
	static bool						GetScrollerImageFormat(QuantityType& imageWidth,
		QuantityType& imageHeight, QuantityType& bitDepth, QuantityType& imageDataSize);

	// Sets the scroller text color.
	static void						SetScrollerTextColor(const CFloatColor& scrollerTextColor);

	// Sets the scroller background color.
	static void						SetScrollerBackgroundColor(const CFloatColor& backgroundColor);

protected:
	// Protected constructor/destructor to enforce existence as a singleton class.
	CScrollerManager();
	~CScrollerManager();

	// Verifies the validity of the class (including stream server instance pointer check).
	static bool						IsValid();

	// Adjusts the scroller index and pixel offset such that the proper character is
	// referenced during scroller incrementing.
	static void						AdjustScrollerIndex();

	// Utility function - obtains a scroller string from the message server.
	static bool						GetScrollerString(const MemIndexType stringIndex,
		const MemSizeType stringLength, std::string& scrollerString);

	// Sets the current pixel offset for scroller "lead-in".
	static void						ApplyLeadInPixelOffset();

	// Text stream server - provides text to clients from a provided source file.
	static CTextStreamServer*		pTextStreamServer;

	// Image stream server - provides scroller images to clients.
	static CScrollerImageStreamServer*
									pImageStreamServer;
	// Current index within the scroller string.
	static MemSizeType				mCurrentScrollerTextIndex;

	// Current pixel offset with the foremost scroller character.
	static RelativeQuantityType		mCurrentScrollerPixelOffset;

	// Scrolling rate (pixels per scroller step).
	static QuantityType				mScrollRate;

	// Length of one visible/drawn scroller text segment.
	static const MemSizeType		mkScrollerStringSegmentLength;

	// Scroller image width.
	static QuantityType				mImageWidth;

	// Scroller image height.
	static QuantityType				mImageHeight;

	// Default Image color depth.
	static const QuantityType		mkImageBitDepth;

	// Scroller text color.
	static CFloatColor				mScrollerTextColor;

	// Scroller background color.
	static CFloatColor				mBackgroundColor;

	// "Padding" for "lead-in" (pixels - produces an additional delay before the text
	// initially appears).
	const static QuantityType		mTextLeadInPadding;
};


#endif // #ifndef _SCROLLERMANAGER_H
