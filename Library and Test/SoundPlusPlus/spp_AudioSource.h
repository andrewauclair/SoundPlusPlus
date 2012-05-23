#pragma once

#include <windows.h>
#include <al.h>
#include <alc.h>
#include <iostream>
#include <map>
#include "spp_AudioManager.h"

using namespace std;

/**********************************************************************************************//**
 * \class	spp_AudioSource
 *
 * \brief	A source of audio in the scene. It can have multiple sounds associated with it. The
 * 			properties and playback of each sound can be controlled individually or together. For
 * 			large sound files, it is reccomended you use spp_StreamingSource.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

class spp_AudioSource
{
public:
	spp_AudioSource(spp_AudioManager* manager);
	~spp_AudioSource();

	void Play();
	void Play(string tag);

	void Stop();
	void Stop(string tag);

	void Pause();
	void Pause(string tag);
	
	void RemoveAllSounds();
	void RemoveSound(string tag); 

	void AssignSound(string tag);

	bool IsPlaying();
	bool IsPlaying(string tag);

	void Set3DAudioProperties(spp_3DAudioProperties properties);
	void Set3DAudioProperties(spp_3DAudioProperties properties, string tag);

	void Set3DSpatialInformation(spp_3DSpatialInformation properties);
	void Set3DSpatialInformation(spp_3DSpatialInformation properties, string tag);

	void SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties);
	void SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties, string tag);

	void SetPlaybackPosition(float secondsFromStart);
	void SetPlaybackPosition(float secondsFromStart, string tag);

	friend class spp_AudioManager;

private:
	spp_AudioManager* mpManager;
	map<string, ALuint> mSourceList;	
};

/**********************************************************************************************//**
// End of spp_AudioSource.h
 **************************************************************************************************/

