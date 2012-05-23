#include <spp_ProcessingPlugIn.h>

class DemoProcessor : public spp_ProcessingPlugIn
{
public:

	DemoProcessor();
	~DemoProcessor();

	void ProcessSamples(vector<char> &rawSampleData);
};