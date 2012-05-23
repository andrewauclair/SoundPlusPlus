#include "spp_SignalProcessor.h"

/**********************************************************************************************//**
 * \fn	spp_SignalProcessor::spp_SignalProcessor()
 *
 * \brief	Constructor.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

spp_SignalProcessor::spp_SignalProcessor()
{
}

/**********************************************************************************************//**
 * \fn	spp_SignalProcessor::~spp_SignalProcessor()
 *
 * \brief	Destructor.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

spp_SignalProcessor::~spp_SignalProcessor()
{
	RemoveAllProcessingPlugIns();
}

/**********************************************************************************************//**
 * \fn	ALuint spp_SignalProcessor::AddProcessingPlugIn(spp_ProcessingPlugIn* plugIn, string name)
 *
 * \brief	Adds a plugin to the manager. If it is active, it will be applied to all sounds
 *			loaded
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	plugIn	the plug in to add
 * \param	name	the name that will refer to this plugin
 *
 * \return	Returns the buffer of the requested tag, or 0 if the buffer was not found.
 **************************************************************************************************/

void spp_SignalProcessor::AddProcessingPlugIn(spp_ProcessingPlugIn* plugIn, string name)
{
	mPlugInList.insert(pair<string, spp_ProcessingPlugIn*>(name, plugIn));
}

/**********************************************************************************************//**
 * \fn	ALuint spp_SignalProcessor::RemoveProcessingPlugIn(string name)
 *
 * \brief	Removes a plug in from the manager
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	name	the name of the plugin to remove
 **************************************************************************************************/

void spp_SignalProcessor::RemoveProcessingPlugIn(string name)
{
	spp_ProcessingPlugIn* temp = mPlugInList.find(name)->second;

	if(temp != NULL)
	{
		mPlugInList.erase(name);

		delete temp;
	}
}

/**********************************************************************************************//**
 * \fn	ALuint spp_SignalProcessor::RemoveAllProcessingPlugIns()
 *
 * \brief	removes all signal processing plug ins from the manager
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	tag	The tag.
 **************************************************************************************************/

void spp_SignalProcessor::RemoveAllProcessingPlugIns()
{
	map<string, spp_ProcessingPlugIn*>::iterator iterator;

	for (iterator = mPlugInList.begin(); iterator != mPlugInList.end(); ++iterator) 
	{
		delete iterator->second;
	}

	mPlugInList.clear();
}

/**********************************************************************************************//**
 * \fn	ALuint spp_SignalProcessor::ProcessSamples(vector<char> &samples)
 *
 * \brief	Runs sample data through all the signal processing plugins the manager has active
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	samples	A vector containing the sound data to be processed
 **************************************************************************************************/

void spp_SignalProcessor::ProcessSamples(vector<char> &samples)
{
	map<string, spp_ProcessingPlugIn*>::iterator iterator;

	for (iterator = mPlugInList.begin(); iterator != mPlugInList.end(); ++iterator) 
	{
		if(iterator->second->IsActive())
		{
			iterator->second->ProcessSamples(samples);
		}
	}
}

/**********************************************************************************************//**
 * \fn	ALuint spp_SignalProcessor::RemoveProcessingPlugIn(string name)
 *
 * \brief	Sets the active state of a single plugin
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	state	the state the plugins are to have
 **************************************************************************************************/

void spp_SignalProcessor::SetPlugInState(string name, bool state)
{
	spp_ProcessingPlugIn* temp = mPlugInList.find(name)->second;

	if(temp != NULL)
	{
		temp->SetIsActive(state);
	}
}

/**********************************************************************************************//**
 * \fn	ALuint spp_SignalProcessor::SetAllPlugInStates()
 *
 * \brief	Sets the active state of all plug ins in the manager
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	state	the state the plugins are to have
 **************************************************************************************************/

void spp_SignalProcessor::SetAllPlugInStates(bool state)
{
	map<string, spp_ProcessingPlugIn*>::iterator iterator;

	for (iterator = mPlugInList.begin(); iterator != mPlugInList.end(); ++iterator) 
	{
		iterator->second->SetIsActive(state);
	}
}