#include "AudioMgr.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
// Ogg Vorbis stuff
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>


AudioMgr::AudioMgr()
	:Subsystem(),mInitialized(0){}
//-----------------------------------------------------------------------

AudioMgr::~AudioMgr()
{
	deinit();
}
//-----------------------------------------------------------------------

void AudioMgr::init()
{
	if(!mInitialized)
	{
		mDefaultVolume = 1.f;
		mDefaultVolumeOgg = 1.f;

		alutInit(NULL,NULL);

		ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
		ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
		ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
		alListenerfv(AL_POSITION,    ListenerPos);
		alListenerfv(AL_VELOCITY,    ListenerVel);
		alListenerfv(AL_ORIENTATION, ListenerOri);

		unsigned int a = 0;
		bool failed = false;
		ALuint sources[64];
		while(!failed&&a<64)
		{
			alGenSources(1,&sources[a]);
			if(alGetError()!=AL_NO_ERROR)
			{
				failed = true;
			}
			else
			{
				a++;
			}
		}
		mMaxSources = a;// good info to have on hand...


		unsigned int b = 0;
		while(b<a)
		{
			alDeleteSources(1,&sources[b]);
			b++;
		}

		b = 0;
		while(b<a)
		{
			mSounds.push_back(new ActiveSound());// empty sound source
			b++;
		}
		mInitialized = true;
		SoundPtr s = play2D("media/audio/crappymusic.wav");
		s.getData().mLooping = true;
		s.getData().mGain = 0.03;
		getActiveSound(s.getIndex())->updateData();
	}
}
//-----------------------------------------------------------------------

void AudioMgr::deinit()
{
	if(mInitialized)
	{
		for(unsigned int i=0;i<mSounds.size();++i)
		{
			delete mSounds[i];
			mSounds[i] = 0;
		}
		mSounds.clear();

		std::map<String,BufferedSound*>::iterator iter;
		for(iter = mBuffers.begin();iter != mBuffers.end();++iter)
		{
			delete iter->second;
			iter->second = 0;
		}
		mBuffers.clear();

		alutExit();
		mInitialized = false;
	}
}
//-----------------------------------------------------------------------

void AudioMgr::update(Real delta)
{
	for(unsigned int i=0;i<mSounds.size();++i)
	{
		mSounds[i]->update();
		if(mSounds[i]->isActive())
		{
			if(!mSounds[i]->getData().mBuffer->isOgg())
			{
				mSounds[i]->getData().mGain = mDefaultVolume;
			}
			else
			{
				mSounds[i]->getData().mGain = mDefaultVolumeOgg;
			}
			mSounds[i]->updateData();
		}
	}
}
//-----------------------------------------------------------------------

void AudioMgr::endState()
{

}
//-----------------------------------------------------------------------

String AudioMgr::getName()
{
	return String("AudioMgr");
}
//-----------------------------------------------------------------------

SoundPtr AudioMgr::play2D(String filename)
{
	if(mBuffers.find(filename)!=mBuffers.end())
	{
		if(!mBuffers[filename]->isLoaded())
		{
			mBuffers[filename]->load();
		}
	}
	else
	{
		if(!loadSound(filename))
		{
			return SoundPtr(0,0,0);
		}
	}

	unsigned int index = 0;
	for(unsigned int a = 0;a<mSounds.size();++a)
	{
		if(!mSounds[a]->isActive())
		{
			index = a;
			mSounds[a]->playSound(SoundData(mBuffers[filename]));
			break;
		}
	}

	return SoundPtr(index,mSounds[index]->getStartTime(),this);
}
//-----------------------------------------------------------------------

bool AudioMgr::bufferSound(String filename)
{
	return false;
}
//-----------------------------------------------------------------------

bool AudioMgr::loadSound(String filename)
{
	String outName,outExt;
	outExt = filename.substr(filename.size()-3);
	if(outExt=="wav")
	{
		mBuffers[filename] = new BufferedSound(filename,false);
		return true;
	}
	else if(outExt=="ogg")
	{
		mBuffers[filename] = new BufferedSound(filename,true);
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------

void AudioMgr::setDefaultVolume(float volume)
{
	mDefaultVolume = volume;
}
//-----------------------------------------------------------------------

void AudioMgr::setDefaultVolumeOgg(float volume)
{
	mDefaultVolumeOgg = volume;
}

//-----------------------------------------------------------------------
ActiveSound* AudioMgr::getActiveSound(unsigned int index)
{
	if(index<mSounds.size())
	{
		return mSounds[index];
	}
	return 0;
}

