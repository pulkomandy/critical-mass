//	CMSound - an autonomous sound that plays itself

#include "CMSound.h"

#define AUDIO_SUBSCRIPTION_FAILED SUICIDE_NOTE"I couldn't connect to the audio server"

void
play_buffer(void *cookie, void * buffer, size_t size, const media_raw_audio_format & format)
{
	CMSound* playing = (CMSound*) cookie;
	
	playing->Playback(buffer, size);
}


CMSound::CMSound(char *soundName, char *newSoundData, long soundLength)	
	//	constructor that takes a pointer to the sound data
{
	theSoundData = framePointer = newSoundData;
									//	point them both to the sound
	tailEnd = theSoundData + soundLength;	//	set a sentinel to stop it
	alreadyPlaying = new CMSemaphore(soundName);
									//	create a semaphore for mutex access
     								
    media_raw_audio_format format;
	format = media_raw_audio_format::wildcard;
	format.frame_rate = 8000;
	format.channel_count = 2;
	format.format = media_raw_audio_format::B_AUDIO_SHORT;
	format.byte_order = B_MEDIA_LITTLE_ENDIAN;
	format.buffer_size = B_PAGE_SIZE;
	
	the_audio_stream = new BSoundPlayer(&format, soundName, play_buffer, NULL, this);
	the_audio_stream->SetVolume(0.1f);
}
	
CMSound::~CMSound()						//	destructor - releases the sound
	{
	alreadyPlaying->Acquire();			//	make sure it's done playing
	delete theSoundData;				//	release memory
	delete the_audio_stream;	
	delete alreadyPlaying;				//	delete the semaphore
	} // end of ~CMSound destructor	


void CMSound::Play()					//	plays the sound once
	{
	alreadyPlaying->Acquire();			//	make sure that it's not already playing
	//TODO EnterStream(0, true, this, _play_back, _done_playing, true);

	the_audio_stream->SetHasData(true);
	the_audio_stream->Start();

	
	} // end of CMSound::Play()
	
	
void CMSound::Playback(void *sound, size_t& nBytes)
									//	this is called by the system to play each chunk
	{
	char *bufferPosition;				//	where we're copying to
	char *endOfChunk;					//	sentinel for buffering
	if (tailEnd - framePointer < nBytes)	//	if we're at the end of the sound
		endOfChunk = tailEnd;			// 	send what we've got left
	else // i.e. lots left
		endOfChunk = framePointer + nBytes;//	assume we fill it
	bufferPosition = (char*)sound;				//	start at the front of the buffer
     for ( ; framePointer < endOfChunk; framePointer++) 
     	{ 
		*bufferPosition = *framePointer;	//	copy a byte for the left channel
		bufferPosition++;				//	advance the pointer
     	}
	//return (framePointer < tailEnd);		//	FALSE => done
	
	if (framePointer >= tailEnd) DonePlaying();
	} // end of Playback()

	
void CMSound::DonePlaying()				//	cleans up when done
	{
	the_audio_stream->SetHasData(false);
	framePointer = theSoundData;			//	reset the frame pointer
	alreadyPlaying->Release();			//	and release the semaphore
	}
