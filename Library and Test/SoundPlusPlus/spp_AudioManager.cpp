#include "spp_AudioManager.h"
#include "spp_AudioSource.h"
#include <string>
#include <al.h>
#include <alc.h>
#include <vorbis/vorbisfile.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>

/**********************************************************************************************//**
 * \fn	spp_AudioManager::spp_AudioManager()
 *
 * \brief	Performs initialization of OpenAL.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

spp_AudioManager::spp_AudioManager()
{
	//gets a reference to the audio device (NULL gets the default).
	mpDevice = alcOpenDevice(NULL);
	Log("alcOpenDevice in spp_AudioManager.");

	if(mpDevice == NULL)
	{
		Log("No default audio device detected.");
	}

	//creates the "context" that OpenAL works in.
	mpContext = alcCreateContext(mpDevice, NULL);
	Log("alcCreateContext in spp_AudioManager.");

	//set context to current
	alcMakeContextCurrent(mpContext);
	Log("alcMakeContextCurrent in spp_AudioManager.");

	//getting the default capture device
	mpRecordingDevice = alcCaptureOpenDevice(NULL, SPP_RECORDING_FREQUENCY, AL_FORMAT_MONO16, SPP_RECORDING_FREQUENCY/2);
	Log("alcCaptureOpenDevice in spp_AudioManager.");

	if(mpRecordingDevice == NULL)
	{
		Log("No default audio recording device detected.", "spp_AudioManager() in spp_AudioManager");
	}

	//Creating the audio listener class.
	mpListener = new spp_AudioListener(this);

	mIsDebugging = false;
}

/**********************************************************************************************//**
 * \fn	spp_AudioManager::~spp_AudioManager()
 *
 * \brief	Destructor. Gets rid of the listener and closes OpenAL.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

spp_AudioManager::~spp_AudioManager()
{
	//deleting all the stored buffers
	UnloadAllSounds();

	RemoveAllProcessingPlugIns();

	//deleting the listener
	delete mpListener;

	StopDebugMode();

	//closing OpenAL
	alcMakeContextCurrent(NULL);
	alcDestroyContext(mpContext);
	alcCloseDevice(mpDevice);
	alcCaptureCloseDevice(mpRecordingDevice);
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::LoadWAVSound(string fileName, string tag)
 *
 * \brief	Loads a wav sound into the system.
 *
 * \author	erikkkkire. Modified by Kyle Marchev.
 * \date	2/19/2012
 *
 * \param	fileName	Filename of the WAV file.
 * \param	tag			The tag used to assign this sound to spp_AudioSources.
 **************************************************************************************************/

void spp_AudioManager::LoadWAVSound(string fileName, string tag)
{
	//source http://www.youtube.com/watch?v=V83Ja4FmrqE
	
	//information needed to play the file
	DWORD chunkSize;
	DWORD fileSize;
	short formatType;
	short channels;
	DWORD sampleRate;
	DWORD averageBytesPerSecond;
	DWORD dataSize;
	short bytesPerSample;
	short bitsPerSample;
	char typeInfo[4];
	ALenum format=0;

	//opining the file
	FILE *wavFile = NULL;
	fopen_s(&wavFile, fileName.c_str(), "rb");

	if(wavFile == NULL)
	{
		Log("WAV file not found: ", fileName, "LoadWAVFile in spp_AudioManager.");
		return;
	}

	//the next three chunks do error checking. When the error output is implemented, they will 
	// tell the user what went wrong, but for now they just return
	fread(typeInfo, sizeof(char),4,wavFile);
	if(typeInfo[0]!='R' || typeInfo[1]!='I' || typeInfo[2] != 'F' || typeInfo[3] != 'F')
	{
		Log("File did not have RIFF: ", fileName, "LoadWAVFile in spp_AudioManager.");
		return;
	}

	fread(&fileSize, sizeof(DWORD), 1, wavFile);
	fread(typeInfo, sizeof(char), 4, wavFile);
	if(typeInfo[0]!='W' || typeInfo[1]!='A' || typeInfo[2] != 'V' || typeInfo[3] != 'E')
	{
		Log("File is not a WAV file: ", fileName, "LoadWAVFile in spp_AudioManager.");
		return;
	}

	fread(typeInfo, sizeof(char), 4, wavFile);
	if(typeInfo[0]!='f' || typeInfo[1]!='m' || typeInfo[2] != 't' || typeInfo[3] != ' ')
	{
		Log("WAV did not have fmt header: ", fileName, "LoadWAVFile in spp_AudioManager.");
		return;
	}

	//reading data about the sound contained in the wave file
	fread(&chunkSize, sizeof(DWORD), 1, wavFile);
	fread(&formatType, sizeof(short), 1, wavFile);
	fread(&channels, sizeof(short), 1, wavFile);
	fread(&sampleRate, sizeof(DWORD), 1, wavFile);
	fread(&averageBytesPerSecond, sizeof(DWORD), 1, wavFile);
	fread(&bytesPerSample, sizeof(short), 1, wavFile);
	fread(&bitsPerSample, sizeof(short), 1, wavFile);

	//validating that data is present
	fread(typeInfo, sizeof(char),4,wavFile);
	if(typeInfo[0]!='d' || typeInfo[1]!='a' || typeInfo[2] != 't' || typeInfo[3] != 'a')
	{
		Log("WAV did not have data: ", fileName, "LoadWAVFile in spp_AudioManager.");
		return;
	}

	//getting the size of the data
	fread(&dataSize, sizeof(DWORD), 1,wavFile);

	//reading the audio data into a form that can be played in an OpenAL buffer
	unsigned char* audioData = new unsigned char[dataSize];
	fread(audioData,sizeof(BYTE), dataSize, wavFile);

	vector<char> dataVector;
	dataVector.insert(dataVector.end(), audioData, audioData + dataSize);

	//setting for format, stereo or mono
	if(bitsPerSample == 8)
	{
		if(channels = 1)
		{
			format = AL_FORMAT_MONO8;
		}
		else if(channels == 2)
		{
			format = AL_FORMAT_STEREO8;
			Log("Stereo sound loaded: 3D audio will not work with this file: ", fileName, "LoadWAVFile in spp_AudioManager.");
		}
	}
	else if(bitsPerSample == 16)
	{
		if(channels == 1)
		{
			format = AL_FORMAT_MONO16;
		}
		else if(channels == 2)
		{
			format = AL_FORMAT_STEREO16;
			Log("Stereo sound loaded: 3D audio will not work with this file: ", fileName, "LoadWAVFile in spp_AudioManager.");
		}
	}

	ProcessSamples(dataVector);

	//creating the OpenAL buffer to hold the data
	ALuint buffer;
	alGenBuffers(1, &buffer);
	Log("LoadWAVSound in spp_AudioManager.");

	//assigning the data to the buffer
	alBufferData(buffer, format, &dataVector[0], dataSize, sampleRate);
	Log("LoadWAVSound in spp_AudioManager.");

	//adding it to the manager's list of buffers
	mBufferList.insert(pair<string, ALuint>(tag, buffer));

	//closing the WAV file
	fclose(wavFile);
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::LoadOGGSound(string fileName, string tag)
 *
 * \brief	Loads an ogg sound.
 *
 * \author	Anthony "TangentZ" Yuen. Modified by Kyle Marchev.
 * \date	2/19/2012
 *
 * \param	fileName	Filename of the file.
 * \param	tag			The tag.
 **************************************************************************************************/

void spp_AudioManager::LoadOGGSound(string fileName, string tag)
{
	//source http://www.gamedev.net/page/resources/_/technical/game-programming/introduction-to-ogg-vorbis-r2031
	
	//information needed to open file
	int endian = 0;
	int bitStream;
	long dataReadFromFile;
	char bufferInformation[SPP_OGG_SIZE_TO_READ];
	ALenum format;
	ALsizei freqency;
	DWORD dataSize;
	ALuint buffer;
	FILE *file;
	vector < char > dataForBuffer;

	vorbis_info *oggInformation;
	OggVorbis_File oggFile;

	//opening the file
	fopen_s(&file, fileName.c_str(), "rb");

	//this is needed to properly open an ogg file on windows
	ov_callbacks OV_CALLBACKS_DEFAULT = {
										(size_t (*)(void *, size_t, size_t, void *))  fread,
										(int (*)(void *, ogg_int64_t, int))           _ov_header_fseek_wrap,
										(int (*)(void *))                             fclose,
										(long (*)(void *))                            ftell
									};

	//This is what actually opens the file
	int fileError = ov_open_callbacks(file, &oggFile, NULL, 0, OV_CALLBACKS_DEFAULT);

	if(fileError < 0)
	{
		Log("OGG file could not be opened: ", fileName,  "LoadOGGFile in spp_AudioManager.");
		return;
	}
	
	//format information about the OGG file
	oggInformation = ov_info(&oggFile, -1);

	//information on channels
	if(oggInformation->channels == 1)
	{
		format = AL_FORMAT_MONO16;
	}
	else
	{
		format = AL_FORMAT_STEREO16;
		Log("Stereo sound loaded: 3D audio will not work with this file: ", fileName,  "LoadOGGFile in spp_AudioManager.");
	}

	freqency = oggInformation->rate;

	//while data is still found in the file, add it to the buffer
	do
	{
		dataReadFromFile = ov_read(&oggFile, bufferInformation, SPP_OGG_SIZE_TO_READ, endian, 2, 1, &bitStream);

		dataForBuffer.insert(dataForBuffer.end(), bufferInformation, bufferInformation + dataReadFromFile);
	}while(dataReadFromFile > 0);

	//close the OGG file
	ov_clear(&oggFile);

	dataSize = static_cast<ALsizei>(dataForBuffer.size());

	ProcessSamples(dataForBuffer);

	//creating the OpenAL buffer
	alGenBuffers(1, &buffer);
	Log("LoadOGGFile in spp_AudioManager.");

	alBufferData(buffer, format, &dataForBuffer[0], dataSize, freqency);
	Log("LoadOGGFile in spp_AudioManager.");

	//adding the buffer to the list
	mBufferList.insert(pair<string, ALuint>(tag, buffer));
}

/**********************************************************************************************//**
 * \fn	ALuint spp_AudioManager::UnloadSound(string tag)
 *
 * \brief	unloads a single sound from the audio device
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	tag	The tag of the sound to remove.
 **************************************************************************************************/

void spp_AudioManager::UnloadSound(string tag)
{
	map<string, ALuint>::iterator iterator;

	for (iterator = mBufferList.begin(); iterator != mBufferList.end(); ++iterator) 
	{
		if(iterator->first == tag)
		{
			alDeleteBuffers(1, &iterator->second);
		}
	}
}

/**********************************************************************************************//**
 * \fn	ALuint spp_AudioManager::UnloadAllSounds()
 *
 * \brief	unloads all the sounds in the manager from the sound device
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 **************************************************************************************************/

void spp_AudioManager::UnloadAllSounds()
{
	map<string, ALuint>::iterator iterator;

	for (iterator = mBufferList.begin(); iterator != mBufferList.end(); ++iterator) 
	{
		alDeleteBuffers(1, &iterator->second);
	}
}

/**********************************************************************************************//**
 * \fn	ALuint spp_AudioManager::GetBufferForTag(string tag)
 *
 * \brief	Gets the WAV file with the specefied tag. This function in used internally by
 * 			spp_Audio_Sources.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \param	tag	The tag.
 *
 * \return	Returns the buffer of the requested tag, or 0 if the buffer was not found.
 **************************************************************************************************/

ALuint spp_AudioManager::GetBufferForTag(string tag)
{
	map<string, ALuint>::iterator iterator;

    for (iterator = mBufferList.begin(); iterator != mBufferList.end(); ++iterator) 
	{
		if(iterator->first == tag)
		{
			return iterator->second;
		}
	}

	Log("Requested tag not found: ", tag, "GetBufferForTag in spp_AudioManager.");
	return 0;
}

/**********************************************************************************************//**
 * \fn	spp_AudioListener* spp_AudioManager::GetListener()
 *
 * \brief	Gets a reference to the listener.
 *
 * \author	Kyle Marchev
 * \date	2/19/2012
 *
 * \return	null if it fails, else the listener.
 **************************************************************************************************/

spp_AudioListener* spp_AudioManager::GetListener()
{
	return mpListener;
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::SetDopplerInformation(float dopplerFactor, float speedOfSound)
 *
 * \brief	Sets the doppler factor and the speed of sound for all sources.
 *
 * \author	Kyle Marchev
 * \date	2/22/2012
 *
 * \param	dopplerFactor	The doppler factor. Increase this to exagerate the doppler effect.
 * \param	speedOfSound 	The speed of sound. This should be in the same units as position and
 * 							velocity used elsewhere in the program. Default is 343.3.
 **************************************************************************************************/

void spp_AudioManager::SetDopplerInformation(float dopplerFactor, float speedOfSound)
{
	alDopplerFactor(dopplerFactor);
	Log("SetDopplerInformation in spp_AudioManager.");

	alSpeedOfSound(speedOfSound);
	Log("SetDopplerInformation in spp_AudioManager.");
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::AddBuffer(ALuint buffer, string tag)
 *
 * \brief	Adds a buffer of name 'tag' for use by other audio objects.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param	buffer	The buffer to make avalivle.
 * \param	tag   	The tag used to access the buffer.
 **************************************************************************************************/

void spp_AudioManager::AddBuffer(ALuint buffer, string tag)
{
	//adding the buffer to the list
	mBufferList.insert(pair<string, ALuint>(tag, buffer));
}

/**********************************************************************************************//**
 * \fn	ALCdevice* spp_AudioManager::GetRecordingDevice()
 *
 * \brief	Gets the current recording device.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \return	null if it fails, else the recording device.
 **************************************************************************************************/

ALCdevice* spp_AudioManager::GetRecordingDevice()
{
	return mpRecordingDevice;
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::StartDebugMode()
 *
 * \brief	Starts debug mode. Enableing this may negatively impact program performance.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

void spp_AudioManager::StartDebugMode()
{
	if(!mIsDebugging)
	{
		mIsDebugging = true;

		mDebugStream.open ("Sound++_Debug_Output.txt");
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::StopDebugMode()
 *
 * \brief	Stops debug mode.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 **************************************************************************************************/

void spp_AudioManager::StopDebugMode()
{
	if(mIsDebugging)
	{
		mIsDebugging = false;
		mDebugStream.close();
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::Log(string message)
 *
 * \brief	Logs a custom message. Used to give debug output at SoundPlusPlus failure points.
 * 			Outputs to a log file. 
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param	message	The message that is sent to the log file.
 * \param	detail A detail about the error, such as a file name.
 * \param	location The Location that the error occured
 **************************************************************************************************/

void spp_AudioManager::Log(string message, string detail, string location)
{	
	if(mIsDebugging)
	{
		mDebugStream << message << detail << " at " << location << endl << endl;
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::Log(string message)
 *
 * \brief	Logs a custom message. Used to give debug output at SoundPlusPlus failure points.
 * 			Outputs to a log file. 
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param	message	The message that is sent to the log file.
 * \param	location The Location that the error occured
 **************************************************************************************************/

void spp_AudioManager::Log(string message, string location)
{	
	if(mIsDebugging)
	{
		mDebugStream << message << " at " << location << endl << endl;
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::Log()
 *
 * \brief	Checks for any OpenAL errors, and outputs to the log file if any are thrown.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param	location The Location that the error occured
 **************************************************************************************************/

void spp_AudioManager::Log(string location)
{
	if(mIsDebugging)
	{
		//getting OpenAL errors
		ALenum error = alGetError();

		if(error != AL_NO_ERROR)
		{
			//giving the appropriate error message
			switch(error)
			{
			case AL_INVALID_NAME:
				mDebugStream << "OpenAL error: An invalid name was supplied to an OpenAL function" << " at " << location << endl << endl;
				break;

			case AL_INVALID_ENUM:
				mDebugStream << "OpenAL error: An invalid enum was supplied to an OpenAL function" << " at " << location << endl << endl;
				break;

			case AL_INVALID_VALUE:
				mDebugStream << "OpenAL error: An invalid value was supplied to an OpenAL function.\nThis is usually caused by using a number outside of the allowed range for an opperation" << " at " << location << endl << endl;
				break;

			case AL_INVALID_OPERATION:
				mDebugStream << "OpenAL error: An invalid OpenAL opperation was attempted." << " at " << location << endl << endl;
				break;

			case AL_OUT_OF_MEMORY:
				mDebugStream << "OpenAL error: Out of Memory. This may be caused by loading too many sounds into memory\nor allocating too many sources or buffers. It can also be caused by processes unrelated to OpenAL in some curcimstances" << " at " << location << endl << endl;
				break;

			default:
				mDebugStream << "An unknown OpenAL error occurred" << " at " << location << endl << endl;
				break;
			}
		}
	}
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::GetAudioDevices()
 *
 * \brief	Gets a list of playback devices. Each identifier is sepparated by /0, and the string 
 *			ends with /0/0
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \return	null if it fails, else the recording device.
 **************************************************************************************************/

const char* spp_AudioManager::GetAudioDevices()
{
	//make sure this feature is supported 
	if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) 
	{
		const ALCchar* deviceList;

		deviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER); 

		const char* deviceArray = deviceList;

		return deviceArray;
	}

	return NULL;
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::SetAudioDevice(string device)
 *
 * \brief	Attempts to open the requested playback device. Opens the default if it fails.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param	device The device identifier of the playback device to use
 **************************************************************************************************/

void spp_AudioManager::SetAudioDevice(string device)
{
	//get rid of the current device and context to prepare for the new one
	alcCloseDevice(mpDevice);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(mpContext);

	//attempt to open a device
	mpDevice = alcOpenDevice(device.c_str());
	Log("alcOpenDevice in spp_AudioManager.");

	//if the device is not null, opening was sucessfull
	if(mpDevice != NULL)
	{
		Log("Sound device changed.", "SetAudioDevice in spp_AudioManager.");
	}
	else//opening failed, open the default as a fallback
	{
		Log("Audio device not found, using default.");
		
		mpDevice = alcOpenDevice(NULL);
		Log("alcOpenDevice in spp_AudioManager.");
	}
	
	mpContext = alcCreateContext(mpDevice, NULL);
	Log("alcCreateContext in spp_AudioManager.");

	//set context to current
	alcMakeContextCurrent(mpContext);
	Log("alcMakeContextCurrent in spp_AudioManager.");
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::GetRecordingDevices()
 *
 * \brief	Gets a list of recording devices. Each identifier is sepparated by /0, and the string 
 *			ends with /0/0
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \return	null if it fails, else the recording device.
 **************************************************************************************************/

const char*  spp_AudioManager::GetRecordingDevices()
{
	//make sure this feature is supported 
	if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) 
	{
		const ALCchar* deviceList;

		deviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER); 

		const char* deviceArray = deviceList;

		return deviceArray;
	}

	return NULL;
}

/**********************************************************************************************//**
 * \fn	void spp_AudioManager::SetRecordingDevice(string device)
 *
 * \brief	Attempts to open the requested recording device. Opens the default if it fails.
 *
 * \author	Kyle Marchev
 * \date	3/7/2012
 *
 * \param	device The device identifier of the recording device to use
 **************************************************************************************************/

void spp_AudioManager::SetRecordingDevice(string device)
{
	mpRecordingDevice = alcCaptureOpenDevice(device.c_str(), SPP_RECORDING_FREQUENCY, AL_FORMAT_MONO16, SPP_RECORDING_FREQUENCY/2);
	Log("alcCaptureOpenDevice in spp_AudioManager.");

	if(mpRecordingDevice == NULL)
	{
		mpRecordingDevice = alcCaptureOpenDevice(device.c_str(), SPP_RECORDING_FREQUENCY, AL_FORMAT_MONO16, SPP_RECORDING_FREQUENCY/2);
		Log("Could not open recording device, opening default", "SetRecordingDevice() in spp_AudioManager");

		if(mpRecordingDevice == NULL)
		{
			Log("No default audio recording device detected.", "SetRecordingDevice() in spp_AudioManager");
		}
		
	}
}

