
#pragma once
#include <Windows.h>

class HighSpeedTimer
{
private:
	static LARGE_INTEGER mStart, mFinish, mFreq,mInitialStart;//Accurate to microseconds
	static double mTimeScale;//scale of our time
	static int mFrameCount;//How many frames have we done?
	static double mTimeSinceStart;//Seconds since we have started.
	static double mTimeSpentWaiting;//How long have we been waiting?
	//static void (*mLoopFunction) (double deltaTime);//a function pointer to our looped function
public:
	static void Init();
	static void StartFrame();//Starts the frame
	static double GetDeltaTimeSeconds();//gets current time since top of frame in seconds (s)
	static double GetDeltaTimeMiliseconds();//gets current time since top of frame in miliseconds (us)
	static double GetTimeScale();//Gets the timescale
	static double GetTimeSinceStart();//Gets time since Start in seconds
	static int GetFrameCount();//Gets the frame count
	static double AverageFrameTime();//Gets the average time per frame in seconds
	static void Delay(int ms);//Delay current thready in miliseconds
	static double TimeSpentWaiting();//How long have we been doing nothing?
};