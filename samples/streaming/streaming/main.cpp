#include <spp_AudioManager.h>
#include <spp_StreamingSource.h>

void main()
{
	//create the manager
	spp_AudioManager* pManager = new spp_AudioManager();

	//create the streaming source
	spp_StreamingSource stream(pManager);

	cout << "Loading...\n";
	
	//prepare the file for streaming
	stream.PrepareStream("DemoClean.OGG");

	cout << "Playing...\n";

	//play the stream
	stream.Play();

	//update the stream until it is finished
	while(!stream.IsFinished())
	{
		stream.Update();
	}

	//clean up
	delete pManager;
}