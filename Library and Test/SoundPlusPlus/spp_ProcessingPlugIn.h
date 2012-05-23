#pragma once

#include <vector>

using namespace std;

/**********************************************************************************************//**
 * \class	spp_ProcessingPlugIn
 *
 * \brief	This class is a base class for signal processing plug-ins. To use this, extend this class
 *		    and put your desired signal processing algorithm in the ProcessSamples function.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

class spp_ProcessingPlugIn
{
public:

	spp_ProcessingPlugIn();
	~spp_ProcessingPlugIn();

	void SetIsActive(bool state);
	bool IsActive();

	virtual void ProcessSamples(vector<char> &rawSampleData);

private:

	bool mIsActive;

/**********************************************************************************************//**
// End of spp_ProcessingPlugIn.h
 **************************************************************************************************/

};