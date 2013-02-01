#include <cstdlib>
#include <iostream>
#include <string>
#include <spp_AudioManager.h>
#include <spp_AudioSource.h>
#include <spp_StreamingSource.h>
#include <spp_AudioRecorder.h>
#include <spp_Microphone.h>
#include "HighSpeedTimer.h"
#include "TestProcess.h"
#include <time.h>

using namespace std;

int main()
{
	//creating and initializing the manager
	spp_AudioManager* mpAudioManager = new spp_AudioManager();
	spp_AudioListener* mpListener = mpAudioManager->GetListener();
	//spp_AudioSource* mpSource = new spp_AudioSource(mpAudioManager);
	spp_StreamingSource* mpStreamingSource = new spp_StreamingSource(mpAudioManager);
	//spp_Microphone* mpMicrophone = new spp_Microphone(mpAudioManager);
	//spp_AudioRecorder* mpAudioRecorder = new spp_AudioRecorder(mpAudioManager);

	//TestProcess* testProcessing = new TestProcess();
	
	//mpAudioManager->StartDebugMode();

	////create varrialbes to track time
	//time_t startTime;
	//time_t currentTime;

	//time(&startTime);
	//time(&currentTime);

	////normal use tests

	//cout << "Loading WAV and playing source.\n";

	////source
	//mpAudioManager->LoadWAVSound("TruckHorn.WAV", "test");
	//mpAudioManager->AddProcessingPlugIn(testProcessing, "test");
	//mpAudioManager->LoadWAVSound("TruckHorn.WAV", "testQuiet");
	//
	//mpSource->AssignSound("testQuiet");
	//mpSource->Play("test");
	//Sleep(5000);
	//mpSource->Play("testQuiet");
	//Sleep(5000);

	cout << "Streaming source.\n";

	////stream
	mpStreamingSource->PrepareStream("She Sells Sanctuary Short.OGG");
	mpStreamingSource->Play();
	
	cin.get();

	cout << "Paused.\n";
	mpStreamingSource->Pause();

	cin.get();

	cout << "Resumed.\n";
	mpStreamingSource->Play();

	cin.get();

	cout << "Stopped.\n";
	mpStreamingSource->Stop();

	cin.get();

	cout << "Restarted.\n";
	mpStreamingSource->Play();
	while(mpStreamingSource->IsPlaying())
	{
		static int i = 0;
		i++;
	}

	//mpStreamingSource->AddProcessingPlugIn(testProcessing, "test");
	//mpStreamingSource->PrepareStream("She Sells Sanctuary Short.OGG");
	//mpStreamingSource->Play();
	//while(mpStreamingSource->IsPlaying())
	//{
	//	mpStreamingSource->Update();
	//}

	//cout << "Recording and playing back.\n";

	////recorder
	//mpAudioRecorder->Start();
	//time(&startTime);
	//while(difftime(currentTime, startTime) < 10.0)
	//{
	//	mpAudioRecorder->Update();

	//	time(&currentTime);
	//}

	//mpAudioRecorder->Stop();

	//mpAudioRecorder->AddSoundToManager("recordtest");
	//mpSource->AssignSound("recordtest");
	//cout << "Recording and playing back II.\n";
	//mpSource->Play("recordtest");
	//Sleep(10000);

	//cout << "Micriphone.\n";

	//////mic
	//mpMicrophone->Start();
	//time(&startTime);
	//while(difftime(currentTime, startTime) < 10.0)
	//{
	//	mpMicrophone->Update();

	//	time(&currentTime);
	//}

	//cout << "Done.";

	//mpAudioManager->StopDebugMode();

	cin.get();

	delete mpAudioManager;
	//delete mpSource;
	delete mpStreamingSource;
	//delete mpMicrophone;
	//delete mpAudioRecorder
	
	return 0;
}