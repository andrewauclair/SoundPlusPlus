#include "spp_StreamingSource.h"
#include <string>
#include <al.h>
#include <alc.h>

/**********************************************************************************************//**
 * \fn	spp_StreamingSource::spp_StreamingSource(spp_AudioManager* manager)
 *
 * \brief	Constructor.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param [in,out]	manager	If non-null, the manager.
 **************************************************************************************************/

spp_StreamingSource::spp_StreamingSource(spp_AudioManager* manager)
{
	mpManager = manager;
	alGenSources(1, &mSource);
	mIsFinished = true;
	mIsPlaying = false;
}

/**********************************************************************************************//**
 * \fn	spp_StreamingSource::~spp_StreamingSource()
 *
 * \brief	Destructor.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

spp_StreamingSource::~spp_StreamingSource()
{
	ov_clear(&mOggInfo.oggFile);
	alDeleteBuffers(SPP_BUFFERS_TO_QUEUE, mBufferID);
    alDeleteSources(1, &mSource);
}

/**********************************************************************************************//**
 * \fn	void spp_StreamingSource::PlaySound()
 *
 * \brief	Plays the streamed sound.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

void spp_StreamingSource::Play()
{
	if(!mIsPlaying)
	{
		//the source has not finished playing
		mIsFinished = false;
		mIsPlaying = true;
		alSourcePlay(mSource);
	}
}

/**********************************************************************************************//**
 * \fn	void spp_StreamingSource::StopSound()
 *
 * \brief	Stops the streaming sound. Use caution stopping a streaming sound, as stopping it will
 * 			stop the stream.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

void spp_StreamingSource::Stop()
{
	if(mIsPlaying)
	{
		mIsPlaying = false;
		alSourceStop(mSource);
	}
}

/**********************************************************************************************//**
 * \fn	void spp_StreamingSource::PauseSound()
 *
 * \brief	Pauses the stremaing Sound
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

void spp_StreamingSource::Pause()
{
	alSourcePause(mSource);
}

/**********************************************************************************************//**
 * \fn	void spp_StreamingSource::Set3DAudioProperties(spp_3DAudioProperties properties)
 *
 * \brief	Sets the 3D audio properties of all sounds on this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 **************************************************************************************************/

void spp_StreamingSource::Set3DAudioProperties(spp_3DAudioProperties properties)
{
	alSourcef(mSource, AL_MAX_DISTANCE, properties.MaxAudibleDistance);
	alSourcef(mSource, AL_ROLLOFF_FACTOR, properties.SoundRolloffFactor);
	alSourcef(mSource, AL_REFERENCE_DISTANCE, properties.HalfVolumeDistance);

	mpManager->Log("Set3DAudioProperties in spp_StreamingSource");
}

/**********************************************************************************************//**
 * \fn	void spp_StreamingSource::Set3DSpatialInformation(spp_3DAudioProperties properties)
 *
 * \brief	Sets the 3D positional information of all sounds on this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 **************************************************************************************************/

void spp_StreamingSource::Set3DSpatialInformation(spp_3DSpatialInformation properties)
{
	alSourcefv(mSource, AL_POSITION, properties.Position);
	alSourcefv(mSource, AL_VELOCITY, properties.Velocity);
	alSourcefv(mSource, AL_DIRECTION, properties.Direction);

	mpManager->Log("Set3DSpatialInformation in spp_StreamingSource");
}

/**********************************************************************************************//**
 * \fn	void spp_StreamingSource::SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties)
 *
 * \brief	Sets audio playback properties of all sounds on this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 **************************************************************************************************/

void spp_StreamingSource::SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties)
{
	alSourcef(mSource, AL_PITCH, properties.Pitch);
	alSourcef(mSource, AL_GAIN, properties.Volume);
	alSourcef(mSource, AL_MAX_GAIN, properties.MaximumVolume);
	alSourcef(mSource, AL_MIN_GAIN, properties.MinimumVolume);

	mpManager->Log("Warning: Looping and Playback Position do not apply", "SetAudioPlaybackProperties in spp_StreamingSource");
	mpManager->Log("SetAudioPlaybackProperties in spp_StreamingSource");
}

/**********************************************************************************************//**
 * \fn	void spp_StreamingSource::PrepareStream(string fileName)
 *
 * \brief	This prepares the streaming of an OGG file. After this is called, be sure
 * 			to call UpdateStream as often as possible to maintain the stream.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	fileName	Filename of the OGG file to stream.
 **************************************************************************************************/

void spp_StreamingSource::PrepareStream(string fileName)
{
	//data needed for reading file and filling buffer
	int bitStream = 0;
	long dataReadFromFile;
	ALsizei frequency;
	
	//resetting the count of the current buffer
	mCurrentBuffer = 0;

	mOggInfo.endian = 0;

	//opening the raw file
	fopen_s(&mOggInfo.File, fileName.c_str(), "rb");

	// this awful mess is function pointers to functions needed to open an ogg file.
	ov_callbacks OV_CALLBACKS_DEFAULT = {
											(size_t (*)(void *, size_t, size_t, void *))  fread,
											(int (*)(void *, ogg_int64_t, int))           _ov_header_fseek_wrap,
											(int (*)(void *))                             fclose,
											(long (*)(void *))                            ftell
										};

	//opening the file for real
	int errorCode = ov_open_callbacks(mOggInfo.File, &mOggInfo.oggFile, NULL, 0, OV_CALLBACKS_DEFAULT);
	if(errorCode < 0)
	{
		mpManager->Log("OGG file could not be opened for streaming: ", fileName,  "PrepareStream in spp_StreamingSource");
		return;
	}

	//info obtained about the file
	mOggInfo.pInfo = ov_info(&mOggInfo.oggFile, -1);

	//setting the sound format
	if(mOggInfo.pInfo->channels == 1)
	{
		mOggInfo.format = AL_FORMAT_MONO16;
	}
	else
	{
		mOggInfo.format = AL_FORMAT_STEREO16;
		mpManager->Log("Stereo sound loaded: 3D audio will not work with this file: ", fileName,  "PrepareStream in spp_StreamingSource");
	}

	//bitrate
	frequency = mOggInfo.pInfo->rate;

	//this is done five times to queue up the first five buffers
	for(int i = 0; i < SPP_BUFFERS_TO_QUEUE; i++)
	{
		char bufferInformation[SPP_OGG_SIZE_TO_READ];
		vector<char> dataForBuffer;

		//read the chunk of data from the file
		dataReadFromFile = ov_read(&mOggInfo.oggFile, bufferInformation, SPP_OGG_SIZE_TO_READ, mOggInfo.endian, 2, 1, &bitStream);

		//check to make sure something was read
		if(dataReadFromFile > 0)
		{
			//put the data in the buffer
			dataForBuffer.insert(dataForBuffer.end(), bufferInformation, bufferInformation + dataReadFromFile);

			mOggInfo.dataSize = static_cast<ALsizei>(dataForBuffer.size());

			ProcessSamples(dataForBuffer);

			//generate the OpenAL buffer
			alGenBuffers(1, &mBufferID[i]);
			alBufferData(mBufferID[i], mOggInfo.format, &dataForBuffer[0], mOggInfo.dataSize, frequency);
			mpManager->Log("PrepareStream in spp_StreamingSource");

			//queues the buffer for playback by the source. queued buffer will be played in sequence.
			alSourceQueueBuffers(mSource, 1, &mBufferID[i]);

			dataReadFromFile = 0;
		}
	}
}

/**********************************************************************************************//**
 * \fn	void spp_StreamingSource::UpdateStream()
 *
 * \brief	This function updates the stream. It should be called as frequently as possible, as
 * 			updating it too slowly will cause gaps in the audio if it runs out of data to play.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

void spp_StreamingSource::Update()
{
	if(mIsPlaying)
	{
		//getting the state of the source
		ALenum state;
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);

		//if the source is not playing, this has finished streaming
		if(state != AL_PLAYING)
		{
			alSourcePlay(mSource);
		}
 
		//getting the number of buffers the source has processed in it's queue
		int buffersProcessed;
		alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &buffersProcessed);
	
		//if it has processed a buffer, load more data
		if(buffersProcessed  > 0)
		{
			//loading information
			long dataReadFromFile = 0;
			char charArray[SPP_OGG_SIZE_TO_READ];
			int bitStream = 0;

			//reading the data from the file.
			dataReadFromFile = ov_read(&mOggInfo.oggFile, charArray, SPP_OGG_SIZE_TO_READ, mOggInfo.endian, 2, 1, &bitStream);

			if(dataReadFromFile > 0)
			{
				//inserting the read data into a holder
				vector<char> buffer;
				buffer.insert(buffer.end(), charArray, charArray + dataReadFromFile);

				mOggInfo.dataSize = static_cast<ALsizei>(buffer.size());

				//getting rid of the finished buffer
				alSourceUnqueueBuffers(mSource, 1, &mBufferID[mCurrentBuffer]);
				mpManager->Log("UpdateStream in spp_StreamingSource 1");

				ProcessSamples(buffer);

				//generating a buffer to replace the spent one
				alGenBuffers(1, &mBufferID[mCurrentBuffer]);
				mpManager->Log("UpdateStream in spp_StreamingSource 2");

				//putting the data in the new buffer
				alBufferData(mBufferID[mCurrentBuffer], mOggInfo.format, &buffer[0], mOggInfo.dataSize, mOggInfo.pInfo->rate);
				mpManager->Log("UpdateStream in spp_StreamingSource 3");

				//adding the newest buffer into the queue
				alSourceQueueBuffers(mSource, 1, &mBufferID[mCurrentBuffer]);

				//keeping track of which buffer is at the front, so the correct one can be replaced
				mCurrentBuffer++;
				if(mCurrentBuffer <= SPP_BUFFERS_TO_QUEUE)
				{
					mCurrentBuffer = 0;
				}
			}
			else
			{
				//no more data, the stream is finished
				mIsFinished = true;
				mIsPlaying = false;
			}
		}	
	}
}

/**********************************************************************************************//**
 * \fn	bool spp_StreamingSource::FinishedStreaming()
 *
 * \brief	Determines if this has finished streaming.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \return	True of the source has finished streaming, false otherwise
 **************************************************************************************************/

bool spp_StreamingSource::IsFinished()
{
	return mIsFinished;
}

/**********************************************************************************************//**
 * \fn	bool spp_StreamingSource::IsPlaying()
 *
 * \brief	Query if this object is playing.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \return	true if playing, false if not.
 **************************************************************************************************/

bool spp_StreamingSource::IsPlaying()
{
	return mIsPlaying;
}