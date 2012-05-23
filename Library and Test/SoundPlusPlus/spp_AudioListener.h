#pragma once

#include <windows.h>
#include <al.h>
#include <alc.h>
#include <iostream>
#include <map>

using namespace std;

/**********************************************************************************************//**
 * \struct	spp_AudioListenerProperties
 *
 * \brief	This struct contains all the properties of the Audio Listener class that can be congigured.
 * 			You can change Listener settings by passing it one of these structs with the 
 * 			SetListenerProperties method.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

struct spp_AudioListenerProperties
{
	float Volume;

	//x, y, z position of source
	float Position[3];

	//x, y, x velocity of source.
	float Velocity[3];

	//the vector that the listener is looking at
	float LookAtVector[3];

	//the directin that is "up" relative to the listener
	float UpVector[3];
};

/**********************************************************************************************//**
 * \class	spp_AudioListener
 *
 * \brief	This class provides control over the audio listener in the scene. All sounds are modulated
 * 			based on where they are in relation to this class. There can only ever be one audio listener.
 * 			To get a refrance to the listener, use the GetListener() function of the audio manager class.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

class spp_AudioListener
{
public:

	void SetListenerProperties(spp_AudioListenerProperties properties);

	//the manager is a friend so it can use the constructor.
	friend class spp_AudioManager;

private:
	// Reference to the manager
	spp_AudioManager* mpManager;

	// The constructor/destructor is private so only the AudioManager can create an instance. This is to prevent multiple 
	// copies of the listener being created.
	spp_AudioListener(spp_AudioManager* manager);
	~spp_AudioListener();

/**********************************************************************************************//**
// End of spp_AudioListener.h
 **************************************************************************************************/

};