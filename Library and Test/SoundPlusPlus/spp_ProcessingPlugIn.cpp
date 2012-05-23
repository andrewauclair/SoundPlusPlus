/**********************************************************************************************//**
 * \file	spp_ProcessingPlugIn.cpp
 *
 * \brief	Implements the spp audio listener class.
 **************************************************************************************************/

#include "spp_ProcessingPlugIn.h"

/**********************************************************************************************//**
 * \fn	spp_ProcessingPlugIn::spp_ProcessingPlugIn()
 *
 * \brief	Constructor.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

spp_ProcessingPlugIn::spp_ProcessingPlugIn()
{

}

/**********************************************************************************************//**
 * \fn	spp_ProcessingPlugIn::~spp_ProcessingPlugIn()
 *
 * \brief	Destructor.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

spp_ProcessingPlugIn::~spp_ProcessingPlugIn()
{
}

/**********************************************************************************************//**
 * \fn	void spp_ProcessingPlugIn::ProcessSamples(vector<char> rawSampleData)
 *
 * \brief	runs the signal processing function on the provided samples
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	rawSampleData	The raw samples to be processed
 *
 * \return	returns the processed data
 **************************************************************************************************/

void spp_ProcessingPlugIn::ProcessSamples(vector<char> &rawSampleData)
{
}

/**********************************************************************************************//**
 * \fn	void spp_ProcessingPlugIn::SetIsActive(bool state)
 *
 * \brief	Sets wether this plugin will be used when processing is applied
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	state	The desired state of the plugin
 **************************************************************************************************/

void spp_ProcessingPlugIn::SetIsActive(bool state)
{
	mIsActive = state;
}

/**********************************************************************************************//**
 * \fn	void spp_ProcessingPlugIn::ProcessSamples(vector<char> rawSampleData)
 *
 * \brief	Used to tell if this plug in is currently active
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \return	returns the state of the plugin
 **************************************************************************************************/

bool spp_ProcessingPlugIn::IsActive()
{
	return mIsActive;
}