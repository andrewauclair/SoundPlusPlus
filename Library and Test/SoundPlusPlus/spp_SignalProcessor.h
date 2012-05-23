#pragma once

#include <vector>
#include <map>
#include "spp_ProcessingPlugIn.h"
#include <string>

using namespace std;

/**********************************************************************************************//**
 * \class	spp_SignalProcessor
 *
 * \brief	This is the main class is the base class for things that need to process signals. It 
 *			provides functions for adding, removing, and disable/enabling plug ins, as well as
 *			providing subclasses a way to process their signals.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

class spp_SignalProcessor
{
	public:
		spp_SignalProcessor();
		~spp_SignalProcessor();

		void AddProcessingPlugIn(spp_ProcessingPlugIn* plugIn, string name);
		void RemoveProcessingPlugIn(string name);
		void RemoveAllProcessingPlugIns();
		void SetPlugInState(string name, bool state);
		void SetAllPlugInStates(bool state);

	private:
		//a list of all the plugins to hold
		map<string, spp_ProcessingPlugIn*> mPlugInList;
	
	protected:
		void ProcessSamples(vector<char> &samples);

/**********************************************************************************************//**
// End of spp_SignalProcessor.h
 **************************************************************************************************/

};