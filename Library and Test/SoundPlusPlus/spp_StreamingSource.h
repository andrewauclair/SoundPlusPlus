#pragma once

#include <windows.h>
#include <al.h>
#include <alc.h>
#include <iostream>
#include <map>
#include "spp_AudioManager.h"
#include "spp_AudioSource.h"
#include <vector>
#include <vorbis/vorbisfile.h>

using namespace std;

/**********************************************************************************************//**
 * \class	spp_StreamingSource
 *
 * \brief	Spp streaming source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

class spp_StreamingSource : public spp_SignalProcessor 
{
public:
	spp_StreamingSource(spp_AudioManager* manager);
	~spp_StreamingSource();

	void Play();

	void Stop();

	void Pause();

	void Set3DAudioProperties(spp_3DAudioProperties properties);

	void SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties);

	void Set3DSpatialInformation(spp_3DSpatialInformation properties);

	void PrepareStream(string fileName);
	void Update();

	bool IsFinished();
	bool IsPlaying();

	friend class spp_AudioManager;

private:
	spp_AudioManager* mpManager;

	ALuint mSource;

	//information about the OGG file being streamed
	spp_OGG_Data mOggInfo;

	//list of buffers used in streaming
	ALuint mBufferID[SPP_BUFFERS_TO_QUEUE];

	//the current buffer in the queue
	int mCurrentBuffer;

	bool mIsFinished;

	bool mIsPlaying;
};

/**********************************************************************************************//**
// End of spp_StreamingSource.h
 **************************************************************************************************/

