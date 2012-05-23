SoundPlusPlus
=============

An easy to use sound solution for games and other applications written in C++.

Current project is in Visual Studio 2010, and has been tested on Windows 7 64bit only.

Current Features:
	- Play WAV and OGG files from sources
	- 3D audio by OpenAL
	- Stream OGG files
	- Record from microphone and play back from sources
	- Record from microphone and play back in real time
	- Write custom signal processing plug ins
	
Known Issues:
	- Sample projects require project settings to run
	
Suggestions for Improvement
	- Create/Destroy buffers as sounds are played to better acomodate signal processing
	- Devise a way to allow streaming updates to occur less frequently, or perhaps implement threading
	