#include "spp_AudioSource.h"
#include <string>
#include <al.h>
#include <alc.h>

/**********************************************************************************************//**
 * \fn	spp_AudioSource::spp_AudioSource(spp_AudioManager* manager)
 *
 * \brief	Constructor.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param [in,out]	Needs a reference to the Manager
 **************************************************************************************************/

spp_AudioSource::spp_AudioSource(spp_AudioManager* manager)
{
	mpManager = manager;
}

/**********************************************************************************************//**
 * \fn	spp_AudioSource::~spp_AudioSource()
 *
 * \brief	Deletes all the OpenAL sources that this source had.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

spp_AudioSource::~spp_AudioSource()
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		alDeleteSources(1, &iterator->second);
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::AssignSound(string tag)
 *
 * \brief	Assigns a sound to this Source. Sources can have any number of sounds.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	tag	The tag of the sound to assign. Tags are assigned when sounds are loaded into the 
 * 				spp_AudioManager
 **************************************************************************************************/

void spp_AudioSource::AssignSound(string tag)
{
	ALuint buffer = mpManager->GetBufferForTag(tag);

	if(buffer != 0)
	{
		ALuint tempSource;

		alGenSources(1, &tempSource);
		mpManager->Log("AssignSound in spp_AudioSource");

		alSourcei(tempSource, AL_BUFFER, buffer);
		mpManager->Log("AssignSound in spp_AudioSource");

		mSourceList.insert(pair<string, ALuint>(tag, tempSource));
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::Play(string tag)
 *
 * \brief	Plays one of the sounds on this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	tag	The tag of the sound to play.
 **************************************************************************************************/

void spp_AudioSource::Play(string tag)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		if(iterator->first == tag)
		{
			alSourcePlay(iterator->second);
		}
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::Play()
 *
 * \brief	Plays all the sound associated with this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

void spp_AudioSource::Play()
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		alSourcePlay(iterator->second);
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::Stop(string tag)
 *
 * \brief	Stops one of the sounds associated with this source..
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	tag	The tag of the sound to stop.
 **************************************************************************************************/

void spp_AudioSource::Stop(string tag)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		if(iterator->first == tag)
		{
			alSourceStop(iterator->second);
		}
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::Stop()
 *
 * \brief	Stops all sounds on the source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

void spp_AudioSource::Stop()
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		alSourceStop(iterator->second);
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::Pause(string tag)
 *
 * \brief	Pause a sound associated with this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	tag	The tag of the sound to pause.
 **************************************************************************************************/

void spp_AudioSource::Pause(string tag)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		if(iterator->first == tag)
		{
			alSourcePause(iterator->second);
		}
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::Pause()
 *
 * \brief	Pause all sounds.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

void spp_AudioSource::Pause()
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		alSourcePause(iterator->second);
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::RemoveSound(string tag)
 *
 * \brief	Remove a sound from this source
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	tag	The tag of the sound to delete.
 **************************************************************************************************/

void spp_AudioSource::RemoveSound(string tag)
{
	map<string, ALuint>::iterator iterator = mSourceList.find(tag);

	mSourceList.erase(iterator);
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::RemoveAllSounds()
 *
 * \brief	Remove all sounds from this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

void spp_AudioSource::RemoveAllSounds()
{
	mSourceList.clear();
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::Set3DAudioProperties(spp_3DAudioProperties properties)
 *
 * \brief	Sets the 3D audio properties of all sounds on this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 **************************************************************************************************/

void spp_AudioSource::Set3DAudioProperties(spp_3DAudioProperties properties)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		alSourcef(iterator->second, AL_MAX_DISTANCE, properties.MaxAudibleDistance);
		alSourcef(iterator->second, AL_ROLLOFF_FACTOR, properties.SoundRolloffFactor);
		alSourcef(iterator->second, AL_REFERENCE_DISTANCE, properties.HalfVolumeDistance);
	}

	mpManager->Log("Set3DAudioProperties in spp_AudioSource");
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::Set3DAudioProperties(spp_3DAudioProperties properties, string tag)
 *
 * \brief	Sets the 3D audio properties of a single sound on the source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 * \param	tag		  	The tag of the sound to apply properties to.
 **************************************************************************************************/

void spp_AudioSource::Set3DAudioProperties(spp_3DAudioProperties properties, string tag)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		if(iterator->first == tag)
		{
			alSourcef(iterator->second, AL_MAX_DISTANCE, properties.MaxAudibleDistance);
			alSourcef(iterator->second, AL_ROLLOFF_FACTOR, properties.SoundRolloffFactor);
			alSourcef(iterator->second, AL_REFERENCE_DISTANCE, properties.HalfVolumeDistance);
		}
	}

	mpManager->Log("Set3DAudioProperties in spp_AudioSource.");
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::Set3DSpatialInformation(spp_3DAudioProperties properties)
 *
 * \brief	Sets the 3D positional information of all sounds on this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 **************************************************************************************************/

void spp_AudioSource::Set3DSpatialInformation(spp_3DSpatialInformation properties)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		alSourcefv(iterator->second, AL_POSITION, properties.Position);
		alSourcefv(iterator->second, AL_VELOCITY, properties.Velocity);
		alSourcefv(iterator->second, AL_DIRECTION, properties.Direction);
	}

	mpManager->Log("Set3DSpatialInformation in spp_AudioSource");
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::Set3DSpatialInformation(spp_3DAudioProperties properties, string tag)
 *
 * \brief	Sets the 3D positional information of a single sound on the source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 * \param	tag		  	The tag of the sound to apply properties to.
 **************************************************************************************************/

void spp_AudioSource::Set3DSpatialInformation(spp_3DSpatialInformation properties, string tag)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		if(iterator->first == tag)
		{
			alSourcefv(iterator->second, AL_POSITION, properties.Position);
			alSourcefv(iterator->second, AL_VELOCITY, properties.Velocity);
			alSourcefv(iterator->second, AL_DIRECTION, properties.Direction);
		}
	}

	mpManager->Log("Set3DSpatialInformation in spp_AudioSource.");
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties)
 *
 * \brief	Sets audio playback properties of all sounds on this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 **************************************************************************************************/

void spp_AudioSource::SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		alSourcef(iterator->second, AL_PITCH, properties.Pitch);
		alSourcef(iterator->second, AL_GAIN, properties.Volume);
		alSourcef(iterator->second, AL_MAX_GAIN, properties.MaximumVolume);
		alSourcef(iterator->second, AL_MIN_GAIN, properties.MinimumVolume);
		alSourcei(iterator->second, AL_LOOPING, properties.Looping);
	}

	mpManager->Log("SetAudioPlaybackProperties in spp_AudioSource.");
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties,
 * 		string tag)
 *
 * \brief	Sets audio playback properties for a single sound associated with this source.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties to be applied.
 * \param	tag		  	The tag of the sound to apply the properties to.
 **************************************************************************************************/

void spp_AudioSource::SetAudioPlaybackProperties(spp_AudioPlaybackProperties properties, string tag)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		if(iterator->first == tag)
		{
			alSourcef(iterator->second, AL_PITCH, properties.Pitch);
			alSourcef(iterator->second, AL_GAIN, properties.Volume);
			alSourcef(iterator->second, AL_MAX_GAIN, properties.MaximumVolume);
			alSourcef(iterator->second, AL_MIN_GAIN, properties.MinimumVolume);
			alSourcei(iterator->second, AL_LOOPING, properties.Looping);
		}
	}

	mpManager->Log("SetAudioPlaybackProperties in spp_AudioSource.");
}

/**********************************************************************************************//**
 * \fn	bool spp_AudioSource::IsPlaying()
 *
 * \brief	Query if this source is playing any sounds.
 *
 * \author	Kyle Marchev
 * \date	2/22/2012
 *
 * \return	true if playing, false if not.
 **************************************************************************************************/

bool spp_AudioSource::IsPlaying()
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		//getting the state of the source
		ALenum state;
		alGetSourcei(iterator->second, AL_SOURCE_STATE, &state);

		if(state == AL_PLAYING)
		{
			//a sound is playing from this source
			return true;
		}
	}

	//no sounds are playing
	return false;
}

/**********************************************************************************************//**
 * \fn	bool spp_AudioSource::IsPlaying(string tag)
 *
 * \brief	Query if the sound with the provided tag is playing.
 *
 * \author	Kyle Marchev
 * \date	2/22/2012
 *
 * \param	tag	The tag of the song to query.
 *
 * \return	true if playing, false if not.
 **************************************************************************************************/

bool spp_AudioSource::IsPlaying(string tag)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		if(iterator->first == tag)
		{
			//getting the state of the source
			ALenum state;
			alGetSourcei(iterator->second, AL_SOURCE_STATE, &state);

			if(state == AL_PLAYING)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	mpManager->Log("Requested tag not found.", "IsPlaying in spp_AudioSource");
	return false;
}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::SetPlaybackPosition(float secondsFromStart)
 *
 * \brief	Sets the playback position of all sounds in seconds.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	secondsFromStart	The position to start playing at in seconds
 **************************************************************************************************/

void spp_AudioSource::SetPlaybackPosition(float secondsFromStart)
{
	map<string, ALuint>::iterator iterator;

   for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		alSourcef(iterator->second, AL_SEC_OFFSET, secondsFromStart);
	}

	mpManager->Log("SetPlaybackPosition in spp_AudioSource.");

}

/**********************************************************************************************//**
 * \fn	void spp_AudioSource::SetPlaybackPosition(float secondsFromStart, string tag)
 *
 * \brief	Sets the playback position of the specefied sound in seconds.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	secondsFromStart	The position to start playing at in seconds
 * \param	tag		  	The tag of the sound to apply the properties to.
 **************************************************************************************************/

void spp_AudioSource::SetPlaybackPosition(float secondsFromStart, string tag)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mSourceList.begin(); iterator != mSourceList.end(); ++iterator) 
	{
		if(iterator->first == tag)
		{
			alSourcef(iterator->second, AL_SEC_OFFSET, secondsFromStart);
		}
	}

	mpManager->Log("SetPlaybackPosition in spp_AudioSource.");
}