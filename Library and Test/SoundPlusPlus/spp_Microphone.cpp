#include "spp_Microphone.h"
#include <alc.h>

/**********************************************************************************************//**
 * \fn	spp_Microphone::spp_Microphone(spp_AudioManager* manager)
 *
 * \brief	Constructor.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param [in,out]	manager	Reference to the manager.
 **************************************************************************************************/

spp_Microphone::spp_Microphone(spp_AudioManager* manager)
{
	mpManager = manager;
	mCurrentBuffer = 0;

	//creating the OpenAL buffer to hold the data 
	for(int i = 0; i < SPP_BUFFERS_TO_QUEUE; i++)
	{
		alGenBuffers(1, &mMicrophoneBuffers[i]);
	}

	alGenSources(1, &mSource);

	mSamplesCapturedBeforeRead = 2048;

	mpManager->Log("spp_Microphone() in spp_Microphone.");
}

/**********************************************************************************************//**
 * \fn	spp_Microphone::~spp_Microphone()
 *
 * \brief	Destructor.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

spp_Microphone::~spp_Microphone()
{
	alDeleteBuffers(SPP_BUFFERS_TO_QUEUE, mMicrophoneBuffers);
	alDeleteSources(1, &mSource);
}

/**********************************************************************************************//**
 * \fn	void spp_Microphone::Start()
 *
 * \brief	This starts the recording/plaback process.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

void spp_Microphone::Start()
{
	if(mpManager->GetRecordingDevice() != NULL)
	{
		//clear any lefover data from the main buffer
		mInputBuffer.clear();

		alSourcePlay(mSource);

		//queue up the first empty buffers
		for(int i = 0; i < SPP_BUFFERS_TO_QUEUE; i++)
		{
			alSourceQueueBuffers(mSource, 1, &mMicrophoneBuffers[i]);
		}

		//start captureing data
		alcCaptureStart(mpManager->GetRecordingDevice());
		mpManager->Log("StartMicrophone in spp_Microphone.");
	}
}

/**********************************************************************************************//**
 * \fn	void spp_Microphone::Stop()
 *
 * \brief	This stops the recording/playback process
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

void spp_Microphone::Stop()
{
	//stop captureing and the source
	if(mpManager->GetRecordingDevice() != NULL)
	{
		alcCaptureStop(mpManager->GetRecordingDevice());
		alSourceStop(mSource);
	}
}

/**********************************************************************************************//**
 * \fn	void spp_Microphone::Update()
 *
 * \brief	This updates the input and output streams of the microphone. It should be called often
 * 			while the class is recording.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

void spp_Microphone::Update()
{
	if(mpManager->GetRecordingDevice() != NULL)
	{
		//the samples avalible to be collected from the recording device.
		ALint samplesAvalible;

		//getting the actual number of samples.
		alcGetIntegerv(mpManager->GetRecordingDevice(), ALC_CAPTURE_SAMPLES, 1, &samplesAvalible);


		//a chunk of data large enough is avalible, take it in
		if(samplesAvalible > mSamplesCapturedBeforeRead)
		{
			//a temporary buffer to hold the data taken in
			short tempInputBuffer[SPP_MAX_RECORDING_SAMPLE_BATCH_SIZE];

			//reading the data from the recording device.
			alcCaptureSamples(mpManager->GetRecordingDevice(), tempInputBuffer, mSamplesCapturedBeforeRead);
			mpManager->Log("UpdateMicrophone in spp_Microphone.");

			//inserting data into the main buffer
			mInputBuffer.insert(mInputBuffer.end(), tempInputBuffer, tempInputBuffer + mSamplesCapturedBeforeRead);
		
		}

		//getting the playback state of the source
		ALenum state;
		alGetSourcei(mSource,AL_SOURCE_STATE, &state);
    
		//make sure the source is playing
		if (state!=AL_PLAYING)
		{
			alSourcePlay(mSource);
		}

		//proceed if buffers have been processed and audio data is avalible
		if(mInputBuffer.size() > 0)
		{
			vector<short> tempBuffer;

			//putting the data into a temporary buffer so the main buffer can be cleared
			tempBuffer.insert(tempBuffer.begin(), mInputBuffer.begin(), mInputBuffer.end());
		
			mInputBuffer.clear();

			//getting rid of the finished buffer
			alSourceUnqueueBuffers(mSource, 1, &mMicrophoneBuffers[mCurrentBuffer]);

			//ProcessSamples(tempBuffer);

			//generating a buffer to replace the spent one
			alGenBuffers(1, &mMicrophoneBuffers[mCurrentBuffer]);

			//putting the data in the new buffer
			alBufferData(mMicrophoneBuffers[mCurrentBuffer], AL_FORMAT_MONO16, &tempBuffer[0], tempBuffer.size() * sizeof(short), SPP_RECORDING_FREQUENCY);

			//adding the newest buffer into the queue
			alSourceQueueBuffers(mSource, 1, &mMicrophoneBuffers[mCurrentBuffer]);

			//keeping track of which buffer is at the front, so the correct one can be replaced
			mCurrentBuffer++;
			if(mCurrentBuffer <= SPP_BUFFERS_TO_QUEUE)
			{
				mCurrentBuffer = 0;
			}
		}
	}
}

/**********************************************************************************************//**
 * \fn	void spp_Microphone::Set3DAudioProperties(spp_3DAudioProperties properties)
 *
 * \brief	Sets the 3D audio properties of all sounds on this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 **************************************************************************************************/

void spp_Microphone::Set3DAudioProperties(spp_3DAudioProperties properties)
{
	alSourcef(mSource, AL_MAX_DISTANCE, properties.MaxAudibleDistance);
	alSourcef(mSource, AL_ROLLOFF_FACTOR, properties.SoundRolloffFactor);
	alSourcef(mSource, AL_REFERENCE_DISTANCE, properties.HalfVolumeDistance);

	mpManager->Log("Set3DAudioProperties in spp_Microphone.");
}

/**********************************************************************************************//**
 * \fn	void spp_Microphone::Set3DSpatialInformation(spp_3DAudioProperties properties)
 *
 * \brief	Sets the 3D positional information of all sounds on this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 **************************************************************************************************/

void spp_Microphone::Set3DSpatialInformation(spp_3DSpatialInformation properties)
{
	alSourcefv(mSource, AL_POSITION, properties.Position);
	alSourcefv(mSource, AL_VELOCITY, properties.Velocity);
	alSourcefv(mSource, AL_DIRECTION, properties.Direction);

	mpManager->Log("Set3DSpatialInformation in spp_Microphone");
}


/**********************************************************************************************//**
 * \fn	void spp_Microphone::SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties)
 *
 * \brief	Sets audio playback properties of all sounds on this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 **************************************************************************************************/

void spp_Microphone::SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties)
{
	alSourcef(mSource, AL_PITCH, properties.Pitch);
	alSourcef(mSource, AL_GAIN, properties.Volume);
	alSourcef(mSource, AL_MAX_GAIN, properties.MaximumVolume);
	alSourcef(mSource, AL_MIN_GAIN, properties.MinimumVolume);

	mpManager->Log("Warning: Looping and Playback Position do not apply", "SetAudioPlaybackProperties in spp_Microphone");
	mpManager->Log("SetAudioPlaybackProperties in spp_Microphone.");
}

/**********************************************************************************************//**
 * \fn	void spp_Microphone::SetSamplesCapturedBeforeRead(int samples)
 *
 * \brief	Sets the samples captured before data is read from the audio device.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param	samples	The samples to read.
 **************************************************************************************************/

void spp_Microphone::SetSamplesCapturedBeforeRead(int samples)
{
	mSamplesCapturedBeforeRead = samples;
}

