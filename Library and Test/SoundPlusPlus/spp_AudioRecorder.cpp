#include "spp_AudioRecorder.h"
#include <alc.h>

/**********************************************************************************************//**
 * \fn	spp_AudioRecorder::spp_AudioRecorder(spp_AudioManager* manager)
 *
 * \brief	Constructor.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param [in,out]	manager	Reference to the manager.
 **************************************************************************************************/

spp_AudioRecorder::spp_AudioRecorder(spp_AudioManager* manager)
{
	mpManager = manager;

	mSamplesCapturedBeforeRead = 2048;
}

/**********************************************************************************************//**
 * \fn	spp_AudioRecorder::~spp_AudioRecorder()
 *
 * \brief	Destructor.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

spp_AudioRecorder::~spp_AudioRecorder()
{
	alDeleteBuffers(1, &mBuffer);
}

/**********************************************************************************************//**
 * \fn	void spp_AudioRecorder::Start()
 *
 * \brief	Starts the recording process.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

void spp_AudioRecorder::Start()
{
	if(mpManager->GetRecordingDevice() != NULL)
	{
		//clear the buffer of previous data
		mRecordingBuffer.clear();

		//delete the buffer that held the old finished data
		alDeleteBuffers(1, &mBuffer);

		//start captureing data
		alcCaptureStart(mpManager->GetRecordingDevice());
		mpManager->Log("SetAudioPlaybackProperties in spp_Microphone.");
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioRecorder::Stop()
 *
 * \brief	Stops a recording.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

void spp_AudioRecorder::Stop()
{
	if(mpManager->GetRecordingDevice() != NULL)
	{
		//stop capturing data
		alcCaptureStop(mpManager->GetRecordingDevice());

		//get the last full chunk of data
		Update();

		//get any last few remaining samples
		//ALint samplesAvalible;
		//alcGetIntegerv(mpManager->GetRecordingDevice(), ALC_CAPTURE_SAMPLES, 1, &samplesAvalible);
		//short tempBuffer[SPP_MAX_RECORDING_SAMPLE_BATCH_SIZE];
		//alcCaptureSamples(mpManager->GetRecordingDevice(), tempBuffer, mSamplesCapturedBeforeRead);
		//mRecordingBuffer.insert(mRecordingBuffer.end(), tempBuffer, tempBuffer + mSamplesCapturedBeforeRead);
		//mpManager->Log("StopRecording in spp_AudioRecorder.");

		//creating the OpenAL buffer to hold the data
		alGenBuffers(1, &mBuffer);
		mpManager->Log("StopRecording in spp_AudioRecorder.");

		//assigning the data to the buffer
		alBufferData(mBuffer, AL_FORMAT_MONO16, &mRecordingBuffer[0], mRecordingBuffer.size() * sizeof(short), SPP_RECORDING_FREQUENCY);
		mpManager->Log("StopRecording in spp_AudioRecorder.");
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioRecorder::Update()
 *
 * \brief	Updates the recording.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

void spp_AudioRecorder::Update()
{
	if(mpManager->GetRecordingDevice() != NULL)
	{
		//holds how many samples the capture device has received
		ALint samplesAvalible;

		//getting the samples avalible
		alcGetIntegerv(mpManager->GetRecordingDevice(), ALC_CAPTURE_SAMPLES, 1, &samplesAvalible);

		//temporary buffer to hold the captured data
		short tempBuffer[SPP_MAX_RECORDING_SAMPLE_BATCH_SIZE];

		//only get the data if there is a signifigant amount
		if(samplesAvalible > mSamplesCapturedBeforeRead)
		{
			//get the samples from the device
			alcCaptureSamples(mpManager->GetRecordingDevice(), tempBuffer, mSamplesCapturedBeforeRead);
		
			//put the samples into the recording buffer
			mRecordingBuffer.insert(mRecordingBuffer.end(), tempBuffer, tempBuffer + mSamplesCapturedBeforeRead);
			mpManager->Log("UpdateRecording in spp_AudioRecorder.");
		}
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioRecorder::AddSoundToManager(string tag)
 *
 * \brief	Adds a sound to manager for use in other parts of the project.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param	tag	The tag that this sound can be accessed by.
 **************************************************************************************************/

void spp_AudioRecorder::AddSoundToManager(string tag)
{
	if(mBuffer > 0)
	{
		mpManager->AddBuffer(mBuffer, tag);
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioRecorder::SetSamplesCapturedBeforeRead(int samples)
 *
 * \brief	Sets the samples captured before data is read from the audio device.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param	samples	The samples to read.
 **************************************************************************************************/

void spp_AudioRecorder::SetSamplesCapturedBeforeRead(int samples)
{
	mSamplesCapturedBeforeRead = samples;
}
