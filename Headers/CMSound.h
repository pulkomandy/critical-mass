//	header file for an autonomous sound that plays itself

#ifndef __CMSOUND_H__
#define __CMSOUND_H__

class CMSound;							//	forward declaration

#include <MediaKit.h>
#include "CMSemaphore.h"

class CMSound
	{
	private:
	char *theSoundData;					//	the sampled data
	char *framePointer;					//	pointer to the "current" frame
	char *tailEnd;						//	pointer to the tail end of the sound data
	CMSemaphore *alreadyPlaying;			//	mutex semaphore
	BSoundPlayer* the_audio_stream;
	
	public:
	CMSound(char *newSoundData, char *soundName, long soundLength);
									//	constructor that takes 
									//	a pointer to the sound data
	
	~CMSound();						//	destructor - releases the sound

	void Playback(void *sound, size_t& nBytes);//	this is called by the system to play each chunk
	
	void Play();						//	plays the sound once
	void DonePlaying();					//	cleans up when done
	
	friend void play_buffer(void *cookie, void * buffer, size_t size, const media_raw_audio_format & format);

	
};

#endif
