/**********************************************************************************************//**
 * \file	spp_AudioListener.cpp
 *
 * \brief	Implements the spp audio listener class.
 **************************************************************************************************/

#include "spp_AudioListener.h"
#include "spp_AudioManager.h"

/**********************************************************************************************//**
 * \fn	spp_AudioListener::spp_AudioListener(spp_AudioManager* manager)
 *
 * \brief	Constructor.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param [in,out]	manager	If non-null, the manager.
 **************************************************************************************************/

spp_AudioListener::spp_AudioListener(spp_AudioManager* manager)
{
	mpManager = manager;
}

/**********************************************************************************************//**
 * \fn	spp_AudioListener::~spp_AudioListener()
 *
 * \brief	Destructor.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

spp_AudioListener::~spp_AudioListener()
{
}

/**********************************************************************************************//**
 * \fn	void spp_AudioListener::SetListenerProperties(spp_AudioListenerProperties properties)
 *
 * \brief	Sets a listener properties.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	properties	The properties.
 **************************************************************************************************/

void spp_AudioListener::SetListenerProperties(spp_AudioListenerProperties properties)
{
	alListenerf(AL_GAIN, properties.Volume);
	alListener3f(AL_POSITION, properties.Position[0], properties.Position[1], properties.Position[2]);
	alListener3f(AL_VELOCITY, properties.Velocity[0], properties.Velocity[1], properties.Velocity[2]);
	
	ALfloat orientation[] = { properties.LookAtVector[0], properties.LookAtVector[1], properties.LookAtVector[2], properties.UpVector[0], properties.UpVector[1], properties.UpVector[2]};
	alListenerfv(AL_ORIENTATION, orientation);

	mpManager->Log("SetListenerProperties in spp_AudioListener.");
}
