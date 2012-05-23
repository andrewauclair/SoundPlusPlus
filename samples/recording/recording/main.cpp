#include <spp_AudioManager.h>
#include <spp_AudioRecorder.h>
#include <spp_AudioSource.h>
#include <time.h>

void main()
{
	//create the manager
	spp_AudioManager* pManager = new spp_AudioManager();

	//create the recorder
	spp_AudioRecorder recorder(pManager);

	//create the source
	spp_AudioSource source(pManager);

	//create varrialbes to track time
	time_t startTime;
	time_t currentTime;

	time(&startTime);
	time(&currentTime);

	cout << "Recording...\n";

	//start the recorder
	recorder.Start();

	//record for ten seconds
	while(difftime(currentTime, startTime) < 10.0)
	{
		//updating the recorder
		recorder.Update();

		time(&currentTime);
	}

	//stop the recorder
	recorder.Stop();

	//add the recorded sound to the audio manager
	recorder.AddSoundToManager("Demo");

	//assign the recorded sound to the source
	source.AssignSound("Demo");

	cout << "Playing...\n";

	//play the source
	source.Play();

	//wait for the source to play
	while(source.IsPlaying())
	{
	}

	//clean up
	delete pManager;
}