#include <spp_AudioManager.h>
#include <spp_AudioSource.h>

void main()
{
	//create the manager
	spp_AudioManager* pManager = new spp_AudioManager();

	//create a source
	spp_AudioSource source(pManager);

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

	//properties to configure the sound in 3d space
	spp_3DSpatialInformation sourcePosition; 

	//adjust as needed
	sourcePosition.Position[0] = 2;
	sourcePosition.Position[1] = 0;
	sourcePosition.Position[2] = 0;

	sourcePosition.Direction[0] = 0;
	sourcePosition.Direction[1] = 0;
	sourcePosition.Direction[2] = 0;

	sourcePosition.Velocity[0] = 0;
	sourcePosition.Velocity[1] = 0;
	sourcePosition.Velocity[2] = 0;

	//give the source the properties
	source.Set3DSpatialInformation(sourcePosition);

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