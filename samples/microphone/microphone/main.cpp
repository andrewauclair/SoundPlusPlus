#include <spp_AudioManager.h>
#include <spp_Microphone.h>
#include <time.h>

void main()
{
	//create the manager
	spp_AudioManager* pManager = new spp_AudioManager();

	//create the microphone
	spp_Microphone microphone(pManager);

	//create varrialbes to track time
	time_t startTime;
	time_t currentTime;

	time(&startTime);
	time(&currentTime);

	cout << "Recording...\n";

	//start the recorder
	microphone.Start();

	//record for ten seconds
	while(difftime(currentTime, startTime) < 10.0)
	{
		//updating the recorder
		microphone.Update();

		time(&currentTime);
	}

	//stop the recorder
	microphone.Stop();

	//clean up
	delete pManager;
}