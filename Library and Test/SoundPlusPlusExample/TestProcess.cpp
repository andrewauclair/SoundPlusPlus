#include "TestProcess.h"

TestProcess::TestProcess()
{
}

TestProcess::~TestProcess()
{
}

void TestProcess::ProcessSamples(vector<char> &rawSampleData)
{
	for(int i = 0; i < rawSampleData.size(); i++)
	{
		rawSampleData[i] *= .2;
	}
}