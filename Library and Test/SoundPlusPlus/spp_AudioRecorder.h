#pragma once

#include "spp_AudioManager.h"
#include <vector>

/**********************************************************************************************//**
 * \class	spp_AudioRecorder
 *
 * \brief	This class records audio data and stores it for playback at a later date.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

class spp_AudioRecorder
{
public:
	spp_AudioRecorder(spp_AudioManager* manager);
	~spp_AudioRecorder();

	void Start();
	void Stop();
	void Update();
	void AddSoundToManager(string tag);

	void SetSamplesCapturedBeforeRead(int samples);

private:

	// Reference to the manager
	spp_AudioManager* mpManager;

	vector<short> mRecordingBuffer;

	ALuint mBuffer;

	//the samples to wait for before reading from recording device
	int mSamplesCapturedBeforeRead;

/**********************************************************************************************//**
// End of spp_AudioRecorder.h
 **************************************************************************************************/

};