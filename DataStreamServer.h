// DataStreamServer.h - Contains declarations pertinent to providing streams of data
//                      to clients through the use of a Singleton class.

#ifndef _DATASTREAMSERVER_H
#define _DATASTREAMSERVER_H	1


#include "MemoryUtility.h"
#include "PrimitiveSupport.h"
#include <string>


typedef enum
{
	eDataStreamUndefined,
	eDataStreamText,
	eDataStreamScollerImage
} EDataStreamType;


class CDataStreamInitialize
{
public:
	CDataStreamInitialize(const EDataStreamType dataStreamType);
	virtual ~CDataStreamInitialize();

	 // Returns the type of data stream that this object will initialize
	EDataStreamType					GetDataStreamType() const;

	virtual bool					IsValid() const = 0;

protected:

	const EDataStreamType			mDataStreamType;
};

class CTextStreamInitialize : public CDataStreamInitialize
{
public:
	CTextStreamInitialize(std::string fileSpec, const bool bUseHash = false);
	~CTextStreamInitialize();

	// Determines the validity of the text stream initialization object.
	bool							IsValid() const;

	// Retrieves the file [full] path name.
	std::string						GetFileSpec() const;

	// Determines whether or not to validate the string with the postfixed hash.
	bool							ValidateWithHash() const;
protected:
	// Full file path name.
	const std::string				mFileSpec;

	// When set to true, the string is validated against a postfixed hash.
	const bool						mbValidateWithHash;
};

class CScrollerImageStreamInitialize : public CDataStreamInitialize
{
public:
	CScrollerImageStreamInitialize(const QuantityType imageWidth, const QuantityType imageHeight,
		const QuantityType bitDepth, const QuantityType colorPlanes);
	~CScrollerImageStreamInitialize();

	// Determines the validity of the image stream initialization object.
	bool							IsValid() const;

	// Retrieves the image width.
	QuantityType					GetImageWidth() const;

	// Retrieves the image height.
	QuantityType					GetImageHeight() const;

	// Retrieves the image bit depth (bits per pixel).
	QuantityType					GetBitDepth() const;

	// Retrieves the color planes used for the image.
	QuantityType					GetColorPlanes() const;

protected:
	// Image width
	const QuantityType				mImageWidth;
	// Image height
	const QuantityType				mImageHeight;
	// Color Depth (bits per pixel)
	const QuantityType				mBitDepth;
	// Color planes (usually equivalent to one)
	const QuantityType				mColorPlanes;
};


class CDataStreamRequest
{
public:
	CDataStreamRequest(const EDataStreamType dataStreamType);
	virtual ~CDataStreamRequest() = 0;

	// Returns the type that corresponds to the data stream request.
	EDataStreamType					GetDataStreamType() const;

	// Determines if the data stream request object is valid.
	virtual bool					IsValid() const = 0;

protected:
	// Type of the data stream request.
	const EDataStreamType			mDataStreamRequestType;
};


class CTextDataStreamRequest : public CDataStreamRequest
{
public:
     CTextDataStreamRequest(const MemIndexType startIndex,
		 const MemSizeType stringSize);
     ~CTextDataStreamRequest();

	// Determines if the data stream request object is valid.
	bool							IsValid() const;

	// Retrieves the data that describes the text stream request.
	bool							GetTextStreamRequestInfo(MemIndexType& startCharacterIndex,
		MemSizeType& stringLength) const;

protected:
	// Starting index of the text within the source pool.
	const MemIndexType				mStartCharacterIndex;
	// Length of the string.
	const MemSizeType				mStringSize;
};

class CScrollerImageDataStreamRequest : public CDataStreamRequest
{
public:
	CScrollerImageDataStreamRequest(const std::string& scrollerText,
		const RelativeQuantityType pixelOffset, const CFloatColor& backgroundColor,
		const CFloatColor& textColor);
	~CScrollerImageDataStreamRequest();

	// Determines if the data stream request object is valid.
	bool							IsValid() const;

	// Retrieves the data that describes the image stream request.
	bool							GetScrollerImageRequestInfo(std::string& scrollerTextString,
		 RelativeQuantityType& pixelOffset, CFloatColor& backgroundColor, CFloatColor& textColor) const;

protected:
	// String to draw.
	const std::string				mScrollerTextString;

	// Distance (in pixels) by which to offset the text (left).
	const RelativeQuantityType		mPixelOffset;

	// Background color
	const CFloatColor				mBackgroundColor;

	// Color of scroller text
	const CFloatColor				mTextColor;
};

class CDataStreamDataTransfer
{
public:
     CDataStreamDataTransfer();
     ~CDataStreamDataTransfer();


protected:
	// Protected interface that should only be exposed to stream server and packet objects -
	// prevents clients from altering packet data.
	class CGenericDataContainer
	{
	public:
		CGenericDataContainer();
		CGenericDataContainer(void* pGenericData, const MemSizeType genericDataSize,
			const EDataStreamType DataStreamType);
		~CGenericDataContainer();

		void*						GetGenericDataPointer();

		MemSizeType					GetGenericDataSize() const;

		EDataStreamType				GetDataStreamType() const;

	protected:
		// Data stream pointer.
		void*						mpGenericData;
		// Size of the data stream.
		const MemSizeType			mGenericDataSize;

		// Type of memory stream data
		const EDataStreamType		mDataStreamType;
	};
};

class CDataStreamPacket : public CDataStreamDataTransfer
{
public:

     CDataStreamPacket();

     // Free memory here, if owned by packet.
     ~CDataStreamPacket();

     bool							GetDataStreamPacketData(void*& pDataStreamPacketData,
		 MemSizeType& dataStreamPacketDataSize);
     bool							SetDataStreamPacketData(CGenericDataContainer& 
		 genericDataContainer, const bool bUseLocalCopy = true);


protected:

	// Determines if the data stream packet object is valid.
	bool							IsValid() const;

	// Allocates memory required for data storage.
	bool							AllocateMemory(const MemSizeType& requiredMemory);
	// De-allocates memory allocated for data storage.
	bool							FreeMemory();

	// Will be true if the memory was allocated by the stream packet object.
	bool							mbMemoryOwnedByPacket;

	// Pointer to the packet data.
	void*							mpDataStreamPacketData;

	// Size of the packet data.
	MemSizeType						mDataStreamPacketDataSize;
};

class CDataStreamServer : public CDataStreamDataTransfer
{
public:
	CDataStreamServer(const EDataStreamType dataStreamServerType,
		const CDataStreamInitialize& initializationData);
	virtual ~CDataStreamServer();

	// Verifies the validity of the stream server object.
	virtual bool					IsValid() const = 0;

	// Retrieves a stream packet, given a provided data stream request.
	virtual bool					GetDataStreamPacket(const CDataStreamRequest&
		dataStreamRequest, CDataStreamPacket& dataStreamPacket) = 0;

	// Retrieves the data stream type.
	EDataStreamType					GetDataStreamType() const;

protected:
	// Initialize the data stream server.
	virtual bool					InitializeDataStreamServer(const CDataStreamInitialize&
		initializationData) = 0;

	// Ensures that the request is valid with respect to the host server (i.e.
	// request type matches server type).
	virtual bool					VerifyDataStreamRequestType(const CDataStreamRequest&
		dataStreamRequest) const;

	// Data stream server type.
	const EDataStreamType			mDataStreamServerType;
};

class CTextStreamServer : public CDataStreamServer
{
public:
	CTextStreamServer(const CDataStreamInitialize& initializationData);
	~CTextStreamServer();

	// Verifies the validity of the stream server object.
	bool							IsValid() const;

	// Retrieves a text stream packet.
	bool							GetDataStreamPacket(const CDataStreamRequest&
		dataStreamRequest, CDataStreamPacket& dataStreamPacket);

protected:

	#define							NUM_HASH_INTERMEDIATES 4

	// Initializes the text data stream server.
	bool							InitializeDataStreamServer(const CDataStreamInitialize&
		 initializationData);

	// Allocates memory for the file buffer.
	bool							AllocateFileBuffer();

	// Frees memory used for the file buffer.
	bool							FreeFileBuffer();

	// Creates a simple hash value from two bytes.
	char SimpleLocalHash(char targetByte, char neighborByte);

	// Performs a hash "round" on a 16-byte data block.
	bool HashRound(unsigned long hashIntermediates[NUM_HASH_INTERMEDIATES], const void* pDataBlock,
		const QuantityType multiplier);

	// "Rotates" the hash intermediates once towareds the right.
	void RotateIntermediatesRight(unsigned long hashIntermediates[NUM_HASH_INTERMEDIATES]);

	// Computes the hash for the text file.
	bool ComputeFileHash(void* pHashBuffer);

	// Validates the message file against the hash resident in the file.
	bool ValidateMessageFile();

	// Determines if a hash is currently being used to validate the message text.
	bool IsUsingHash() const;

	// File access structure pointer.
	FILE*							mpFileAccess;

	// Buffer for reading from file.
	void*							mpFileBuffer;

	// Index that corresponds to the start of the buffer, as taken from the source file.
	MemIndexType					mCurrentBufferIndexInFile;

	// True length of the buffer (in the event of a partial file read).
	MemIndexType					mTrueBufferLength;

	// Will be set to true if a hash is used to validate the message text.
	bool							mbUsingHash;

	// Will be set to true if the text string has been validated against the identifying
	// hash.
	bool							mbMessageValidated;

	// Size of buffer for file reading.
	static const QuantityType		mkDataBufferSize;

	// Size of the hash.
	static const QuantityType		mkHashSize;
};


class CScrollerImageStreamServer : public CDataStreamServer
{
public:
	CScrollerImageStreamServer(const CDataStreamInitialize& initializationData);
	~CScrollerImageStreamServer();

	// Verifies the validity of the stream server object.
	bool							IsValid() const;

	// Retrieves a scroller image stream packet.
	bool							GetDataStreamPacket(const CDataStreamRequest&
		dataStreamRequest, CDataStreamPacket& dataStreamPacket);

	// Determines the width of a string, in pixels, given the current drawing parameters.
	QuantityType					GetScrollerTextWidth(const std::string&
		scrollerText) const;

protected:
	// Initializes the scroller image stream server.
	bool							InitializeDataStreamServer(const CDataStreamInitialize&
		initializationData);

	// Scroller image device context.
	HDC								mhScrollerImageDC;

	// Pointer to scroller image DIB data.
	void*							mpScrollerImageDIBData;

	// Scroller image bitmap handle.
	HBITMAP							mhScrollerImageBitmap;

	// "Original" bitmap present in device context, retained for the purpose of
	// restoring the device context state.
	HBITMAP							mhOldDCBitmap;

	// Font used to draw scroller text.
	HFONT							mhScrollerFont;

	// "Original" font present in device context, retained for the purpose of
	// restoring the device context state.
	HFONT							mhOldDCFont;

	// Image width
	QuantityType					mImageWidth;

	// Image height
	QuantityType					mImageHeight;

	// Image bit depth.
	QuantityType					mImageBitDepth;
};







#endif	// #ifndef _DATASTREAMSERVER_H