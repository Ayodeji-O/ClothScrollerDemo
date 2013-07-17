#include "PrimaryHeaders.h"
#include "Bass.h"



//Background audio class (dependent upon the BASS audio library).
class CBackgroundAudio
{
public:

	CBackgroundAudio(const std::string& fileName);
	~CBackgroundAudio();

	bool PlayAudio();

	bool StopAudio();


protected:
	//Initializes the background audio object (loads the file into memory,
	//initializes the BASS audio library.
	bool Initialize(const std::string& fileName);

	//Initializes the BASS Library, and creates an audio stream from the
	//file resident in the buffer.
	bool InitializeBASS();

	//Completes any final actions required before destruction of the
	//background audio object.
	bool Conclude();

	//Buffer for audio file.
	void*							mpFileBuffer;

	//Audio file buffer size.
	size_t							mFileBufferSize;

	//Background music audio stream handle used by the BASS audio library.
	HSTREAM							mhBassAudioStream;
};