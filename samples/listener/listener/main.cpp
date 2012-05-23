#include <spp_AudioManager.h>
#include <spp_AudioSource.h>
#include <spp_AudioListener.h>

void main()
{
	//create the manager
	spp_AudioManager* pManager = new spp_AudioManager();

	//create a source
	spp_AudioSource source(pManager);

	//create the listener
	spp_AudioListener* pListener;

	//get a referance to the listener
	pListener = pManager->GetListener();

	cout << "Loading...\n";

	//load a sound
	pManager->LoadWAVSound("DemoClean.WAV", "DemoSound");

	//assign the loaded sound to the source
	source.AssignSound("DemoSound");

	cout << "Playing...\n";

	//play the source
	source.Play();

	//wait for the sound to play
	while(source.IsPlaying())
	{
	}

	//properties to configure the listener in 3d space
	spp_AudioListenerProperties listenerPosition; 

	//adjust as needed
	listenerPosition.Position[0] = 2;
	listenerPosition.Position[1] = 0;
	listenerPosition.Position[2] = 0;

	listenerPosition.LookAtVector[0] = 0;
	listenerPosition.LookAtVector[1] = 0;
	listenerPosition.LookAtVector[2] = 1;

	listenerPosition.Velocity[0] = 0;
	listenerPosition.Velocity[1] = 0;
	listenerPosition.Velocity[2] = 0;

	listenerPosition.UpVector[0] = 0;
	listenerPosition.UpVector[1] = 1;
	listenerPosition.UpVector[2] = 0;

	listenerPosition.Volume = 1;

	//give the source the properties
	pListener->SetListenerProperties(listenerPosition);

	cout << "Re-playing...\n";

	//play the source again
	source.Play();

	//wait for the sound to play
	while(source.IsPlaying())
	{
	}

	//clean up
	delete pManager;
}