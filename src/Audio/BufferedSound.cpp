#include "BufferedSound.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

BufferedSound::BufferedSound(const String& filename,bool isOgg,bool delayLoad)
{
	mFilename = filename;
	mOgg = isOgg;
	mLoaded = false;
	if(!delayLoad)
	{
		load();
	}
}

BufferedSound::~BufferedSound()
{
	unload();
}

bool BufferedSound::load()
{
	if(!mLoaded&&!mOgg)
	{
		alGetError();
		mBuffer = alutCreateBufferFromFile(mFilename.c_str());
		// Check for errors
		if(alGetError() == AL_NO_ERROR)
		{
			mLoaded = true;
		}
		else
		{
			mLoaded = false;
		}
	}
	return mLoaded;
}

void BufferedSound::unload()
{
	if(mLoaded)
	{
		alDeleteBuffers(1,&mBuffer);
		mLoaded = false;
		mBuffer = 0;
	}
}

bool BufferedSound::isLoaded()
{
	return mLoaded;
}

bool BufferedSound::isOgg()
{
	return mOgg;
}

unsigned int BufferedSound::getBuffer()
{
	return mBuffer;
}

String BufferedSound::getFilename()
{
	return mFilename;
}
