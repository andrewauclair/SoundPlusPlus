#include "DemoProcessor.h"

DemoProcessor::DemoProcessor()
{
}

DemoProcessor::~DemoProcessor()
{
}

void DemoProcessor::ProcessSamples(vector<char> &rawSampleData)
{
	//reduce the volume of the sound
	for(int i = 0; i < rawSampleData.size(); i++)
	{
		rawSampleData[i] *= .2;
	}
}