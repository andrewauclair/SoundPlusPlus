#pragma once

#include "spp_AudioManager.h"
#include <vector>

/**********************************************************************************************//**
 * \class	spp_Microphone
 *
 * \brief	This class records audio and plays it back in real time.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

class spp_Microphone
{
public:
	//constructor and destructor
	spp_Microphone(spp_AudioManager* manager);
	~spp_Microphone();

	//start and stop the microphone
	void Start();
	void Stop();

	//must be called as often as possible to maintain the audio stream
	void Update();

	//sets the amount of data the audio device needs to capture before being read into memory
	void SetSamplesCapturedBeforeRead(int samples);
	
	//used to set playback properties of the microphone
	void Set3DAudioProperties(spp_3DAudioProperties properties);
	void SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties);
	void Set3DSpatialInformation(spp_3DSpatialInformation properties);
	

private:

	// Reference to the manager
	spp_AudioManager* mpManager;

	//data retreived from the audio device is sotred here
	vector<short> mInputBuffer;

	//buffers
	ALuint mMicrophoneBuffers[SPP_BUFFERS_TO_QUEUE];

	//openAL source playing micriphone data
	ALuint mSource;

	//tracks which buffer in the queue is currently being played
	int mCurrentBuffer;

	//the samples to wait for before reading from recording device
	int mSamplesCapturedBeforeRead;

/**********************************************************************************************//**
// End of spp_Microphone.h
 **************************************************************************************************/

};