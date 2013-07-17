#include "BackgroundAudio.h"
#include "windows.h"

CBackgroundAudio::CBackgroundAudio(const std::string& fileName) : mpFileBuffer(NULL),
	mFileBufferSize(0), mhBassAudioStream(0)
{
	this->Initialize(fileName);
}


CBackgroundAudio::~CBackgroundAudio()
{
	this->Conclude();
}



bool CBackgroundAudio::Initialize(const std::string& fileName)
{
	bool							bInitializedSuccessfully = false;

	//Open file in read-only mode.
	const char						kFileOpenMode[] = "rb";

	if(!fileName.empty()) {
		FILE*						pFilePointer =
			::fopen(fileName.c_str(), kFileOpenMode);

		fpos_t						fileEndPosition = 0;

		//Attempt to find the end of the file (in order to determine the
		//file size)...
		if(pFilePointer) {			
			if(!::fseek(pFilePointer, 0, SEEK_END) &&
				!::fgetpos(pFilePointer, &fileEndPosition) &&
				(fileEndPosition > 0))
			{
				//Store the file size
				const size_t			fileSize = fileEndPosition;

				//Allocate memory for the file 
				this->mpFileBuffer = ::malloc(fileSize);

				//Set the file pointer to the beginning of the file, and read
				//the data into memory.
				if(this->mpFileBuffer && !::fseek(pFilePointer, 0, SEEK_SET)) {				
					::fread(this->mpFileBuffer, 1, fileSize, pFilePointer);
				
					//Store the file buffer size.
					this->mFileBufferSize = fileSize;

					//Initialize the BASS audio library.
					bInitializedSuccessfully = this->InitializeBASS();
				}
			}

			//Close the file.
			::fclose(pFilePointer);
		}
	}

	return(bInitializedSuccessfully);
}


bool CBackgroundAudio::InitializeBASS()
{
	bool							bBassInitializedSuccessfully = false;

	//BASS default device constant
	const int						kBASSDefaultDevice = -1;
	//Output rate of 44.1 kHz...
	const int						kOutputRateHz = 44100;

	//Initialize the BASS library..
	if(::BASS_Init(kBASSDefaultDevice, kOutputRateHz, 0, 0, NULL) &&
		this->mpFileBuffer && (this->mFileBufferSize > 0)) {

		//Create a BASS audio stream, using the file in the memory
		//buffer (the stream will be looped, as indicated by the appropriate
		//flag)...
		HSTREAM						hBassAudioStream =
			BASS_StreamCreateFile(true, this->mpFileBuffer, 0,
			this->mFileBufferSize, BASS_SAMPLE_LOOP);

		if(hBassAudioStream) {
			this->mhBassAudioStream = hBassAudioStream;
		}
	}
    
	return(bBassInitializedSuccessfully);
}

bool CBackgroundAudio::Conclude()
{
	bool							bConcludedSuccessfully = false;

	//Stop background music playback.
	this->StopAudio();

	//Release the memory allocated for the file buffer.
	if(this->mpFileBuffer) {
		::free(this->mpFileBuffer);
	}

	return(bConcludedSuccessfully);
}

bool CBackgroundAudio::PlayAudio()
{
	//Begin playing the background audio using the BASS audio library.
	bool							bPlayedSuccessfully =
		this->mpFileBuffer && this->mhBassAudioStream &&
		BASS_ChannelPlay(this->mhBassAudioStream, true) &&
		BASS_ChannelSetFlags(this->mhBassAudioStream, BASS_SAMPLE_LOOP);

	return(bPlayedSuccessfully);
}

bool CBackgroundAudio::StopAudio()
{
	//Request playback to end through the BASS audio library.
	bool							bStoppedSuccessfully =
		this->mpFileBuffer && this->mhBassAudioStream &&
		::BASS_ChannelStop(this->mhBassAudioStream);

	return(bStoppedSuccessfully);
}