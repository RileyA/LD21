#ifndef SOUNDPTR_H
#define SOUNDPTR_H

#include "StdHeaders.hpp"
#include "ActiveSound.hpp"
#include "BufferedSound.hpp"

class AudioMgr;

/** An object representing a sound, (since the system uses/reuses sound buffers
 *		this is sort of a wrapper representing an individual sound. 
 *		@remarks Always check isValid() before messing with it, since sounds may immediately
 *			be reused... */
class SoundPtr
{
public:

	SoundPtr(unsigned int index,time_t start,AudioMgr* amgr);
	SoundPtr();

	bool isValid();
	bool isActive();

	SoundData& getData();

	unsigned int getIndex(){return mSoundIndex;}

protected:

	time_t mStart;
	unsigned int mSoundIndex;
	bool mValid;
	AudioMgr* mAudioMgr;

};

#endif
