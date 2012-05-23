#include <spp_AudioManager.h>
#include <spp_AudioSource.h>
#include "DemoProcessor.h"

void main()
{
	//create the manager
	spp_AudioManager* pManager = new spp_AudioManager();

	//create a source
	spp_AudioSource source(pManager);

	//create the signal processing plug in
	DemoProcessor* demoProcess = new DemoProcessor();

	cout << "Loading...\n";

	//load a sound
	pManager->LoadWAVSound("DemoClean.WAV", "DemoSound");

	//add the plug in to the manager
	pManager->AddProcessingPlugIn(demoProcess, "Demo");

	//load the file again, this time with processing
	pManager->LoadWAVSound("DemoClean.WAV", "DemoSoundProcessed");

	//assign both loaded sounds to the source
	source.AssignSound("DemoSound");
	source.AssignSound("DemoSoundProcessed");

	cout << "Playing normal...\n";

	//play the source
	source.Play("DemoSound");

	//wait for the sound to play
	while(source.IsPlaying())
	{
	}

	cout << "Playing processed...\n";

	//play the source again
	source.Play("DemoSoundProcessed");

	//wait for the sound to play
	while(source.IsPlaying())
	{
	}

	//clean up
	delete pManager;
}