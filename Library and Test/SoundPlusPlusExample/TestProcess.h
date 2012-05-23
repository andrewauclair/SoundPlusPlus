#include <spp_ProcessingPlugIn.h>

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

class TestProcess : public spp_ProcessingPlugIn
{
public:

	TestProcess();
	~TestProcess();

	void ProcessSamples(vector<char> &rawSampleData);
};