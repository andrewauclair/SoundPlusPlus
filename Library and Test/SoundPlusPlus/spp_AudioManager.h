#pragma once

#include <windows.h>
#include <al.h>
#include <alc.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vorbis/vorbisfile.h>
#include "spp_AudioListener.h"
#include <iostream>
#include <fstream>
#include "spp_SignalProcessor.h"

//this is how much of an ogg file is read in at a time
#define SPP_OGG_SIZE_TO_READ 8192

//defines how many buffers to use for audio objects that stream data 
#define SPP_BUFFERS_TO_QUEUE 5

//the frequency at which sound is recorded
#define SPP_RECORDING_FREQUENCY 22050

//the maximum amount of samples to wait for before reading when recording data
#define SPP_MAX_RECORDING_SAMPLE_BATCH_SIZE 8192

using namespace std;

/**********************************************************************************************//**
 * \struct	spp_WAV_Data
 *
 * \brief	This struct holds information on WAV files that is used for playing and streaming
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

struct spp_WAV_Data //this will be used at a later date to facilitate streaming for WAV files
{
	char type[4];
	DWORD size;
	DWORD chunkSize;
	short formatType;
	short channels;
	DWORD sampleRate;
	DWORD averageBytesPerSecond;
	short bytesPerSample;
	short bitsPerSample;
	DWORD dataSize;
	ALuint buffer;
};

/**********************************************************************************************//**
 * \struct	spp_OGG_Data
 *
 * \brief	This struct holds information on OGG files that is used for playing and streaming
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

struct spp_OGG_Data
{
	int endian;
	FILE *File;
	ALenum format;
	vorbis_info *pInfo;
	OggVorbis_File oggFile;
	ALsizei freq;
	DWORD dataSize;
};

/**********************************************************************************************//**
 * \struct	spp_3D_Audio_Data
 *
 * \brief	This struct holds information that is used to configure the 3D playback of audio files.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

struct spp_3D_Audio_Data
{
	float position[3];
	float velocity[3];
	float volume;
	bool isLooping;
};

/**********************************************************************************************//**
 * \fn	static int _ov_header_fseek_wrap(FILE *f,ogg_int64_t off,int whence)
 *
 * \brief	This function is needed for the playback of OGG files.
 *
 * \author	OGG Vorbis
 * \date	2/19/2012
 *
 * \param [in,out]	f	to process.
 * \param	off		 	The off.
 * \param	whence   	The whence.
 *
 * \return	function pointer to the correct fseek function for this system
 **************************************************************************************************/

static int _ov_header_fseek_wrap(FILE *f,ogg_int64_t off,int whence)
{
	if(f==NULL)
	{
		return(-1);
	}

	#ifdef __MINGW32__
	  return fseeko64(f,off,whence);
	#elif defined (_WIN32)
	  return _fseeki64(f,off,whence);
	#else
	  return fseek(f,off,whence);
	#endif
};

/**********************************************************************************************//**
 * \struct	spp_3DAudioProperties
 *
 * \brief	Holds data on the 3D audio properties that can be congired for a sound.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

struct spp_3DAudioProperties
{
	float MaxAudibleDistance;
	float SoundRolloffFactor;
	float HalfVolumeDistance;
};

/**********************************************************************************************//**
 * \struct	spp_3DSpatialInformation
 *
 * \brief	Holds data on a sound's position in 3d space.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

struct spp_3DSpatialInformation
{
	float Position[3];
	float Velocity[3];
	float Direction[3];
};

/**********************************************************************************************//**
 * \struct	spp_AudioPlaybackProperties
 *
 * \brief	Holds data on the standard playback properties of a source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

struct spp_AudioPlaybackProperties
{
	float Pitch;
	float Volume;
	float MinimumVolume;
	float MaximumVolume;
	bool Looping;
};

/**********************************************************************************************//**
 * \class	spp_AudioManager
 *
 * \brief	This is the main class in the SoundPlusPlus audio system. It is used to load OGG and WAV
 * 			files for playback via spp_AudioSources, as well as for getting a reference to the
 * 			spp_AudioListener.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

class spp_AudioManager : public spp_SignalProcessor 
{
public:
	spp_AudioManager();
	~spp_AudioManager();

	void LoadWAVSound(string fileName, string tag);
	void LoadOGGSound(string fileName, string tag);
	void UnloadSound(string tag);
	void UnloadAllSounds();

	void StartDebugMode();
	void StopDebugMode();

	void SetDopplerInformation(float dopplerFactor, float speedOfSound);

	spp_AudioListener* GetListener();

	const char* GetAudioDevices();
	void SetAudioDevice(string device);

	const char* GetRecordingDevices();
	void SetRecordingDevice(string device);

	//This class is friends with other classes to making logging easy and to make
	// the sound tag system work
	friend class spp_AudioSource;
	friend class spp_AudioRecorder;
	friend class spp_AudioListener;
	friend class spp_StreamingSource;
	friend class spp_Microphone;

private:
	map<string, ALuint> mBufferList;
	ALCdevice* mpDevice;
	ALCdevice* mpRecordingDevice;
	ALCcontext* mpContext;

	//this is used by spp_AudioSource to get the buffers to paly
	ALuint GetBufferForTag(string tag);

	void Log(string message, string detail, string location);
	void Log(string message, string location);
	void Log(string location);

	void AddBuffer(ALuint buffer, string tag);

	ALCdevice* GetRecordingDevice();

	spp_AudioListener* mpListener;

	bool mIsDebugging;

	ofstream mDebugStream;

/**********************************************************************************************//**
// End of spp_AudioManager.h
 **************************************************************************************************/

};