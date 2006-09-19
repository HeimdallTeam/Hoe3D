
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "sound.h"

#ifdef _HOE_OPENAL_

#pragma comment (lib,"OpenAL32.lib")
#pragma comment (lib,"ALut.lib")

SoundSystemAl::SoundSystemAl()
{
	Device = NULL;
	Context = NULL;
}

bool SoundSystemAl::Init(THoeInitSettings *)
{
	Device = alcOpenDevice(NULL);

	if (Device == NULL)
	{
		Con_Print("Failed to Initialize Open AL");
		return false;
	}

	//Create context(s)
	Context=alcCreateContext(Device,NULL);
	if (Context == NULL)
	{
		Con_Print("Failed to initialize Open AL");
		return false;
	}	
	
	//Set active context
	alcGetError(Device); 
	alcMakeContextCurrent(Context);
	if (alcGetError(Device) != ALC_NO_ERROR)
	{
		Con_Print("Failed to Make Context Current");
		return false;
	}

	// Clear Error Code
	alGetError();
	alcGetError(Device);

	Con_Print("%s %s - %s",alGetString(AL_VERSION),alGetString(AL_VENDOR),alGetString(AL_RENDERER));
	//Con_Print("ext: %s",alGetString(AL_EXTENSIONS));


	return true;
}


void SoundSystemAl::Destroy()
{

	//Disable context
	alcMakeContextCurrent(NULL);
	//Release context(s)
	if (Context)
	{
		alcDestroyContext(Context);
		Context = NULL;
	}
	//Close device
	if (Device)
	{
		alcCloseDevice(Device);
		Device = NULL;
	}

}

///////////////////////////////////////////////////////////////////

HoeALBuffer::HoeALBuffer()
{
	ALint	error;
	// Generate Buffers
	alGenBuffers(1, &buffer);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		Con_Print("alGenBuffers error: %d", error);
	}
}

HoeALBuffer::HoeALBuffer(ALuint b)
{
	buffer = b;
}

bool HoeALBuffer::Create(int channels, int freq, int byts, long samples)
{
  return false;

}

byte * HoeALBuffer::Lock()
{
        return (byte*)NULL;
}

void HoeALBuffer::Unlock()
{
}


/*bool HoeALBuffer::LoadFromFile(const char * filename)
{
	ALint	error;
	ALsizei size,freq;
	ALenum	format;
	ALvoid	*data;
	ALboolean loop;

	// Load ding.wav
	alutLoadWAVFile((ALbyte *)filename,&format,&data,&size,&freq,&loop);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		return false;
	}

	// Copy ding.wav audio data into AL Buffer 1
	alBufferData(buffer,format,data,size,freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		return false;
	}
	
	// Unload ding.wav
	alutUnloadWAV(format,data,size,freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		return false;
	}
	return true;
	return false;
}*/

/////////////////////////////////////////////////////////////

HoeALPlayer::HoeALPlayer()
{
	ALint	error;
	// Generate Buffers
	alGenSources(1, &source);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		Con_Print("alGenSources error: %d", error);
	}
}

void HoeALPlayer::Play(HoeALBuffer * buff)
{
	ALuint i;
	alSourceUnqueueBuffers(source, 1, &i);
	alSourceQueueBuffers(source, 1, &(buff->GetBuffer()));
	alSourcePlay(source);
}

#endif // _HOE_AL_
