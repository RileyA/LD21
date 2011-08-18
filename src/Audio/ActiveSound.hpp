#ifndef ACTIVESOUND_H
#define ACTIVESOUND_H

#include "StdHeaders.hpp"
#include "BufferedSound.hpp"
#include "OgreVector3.h"

using Ogre::Vector3;

class OggStream;

struct SoundData
{
public:

	SoundData()
	{
		toDefaults();
		mBuffer = 0;
	}
	SoundData(BufferedSound* buffer)
	{
		toDefaults();
		mBuffer = buffer;
	}
	~SoundData()
	{

	}

	void toDefaults()
	{
		mPosition = Vector3(0,0,0);
		mVelocity = Vector3(0,0,0);
		mGain = 1.f;
		mPitch = 1.f;
		mRolloff = 1.f;
		mMinGain = 0.f;
		mMaxGain = 1.f;
		mLooping = 0;
		mRefDist = 1.f;
		mMaxDist = 2000.f;
		mOffset = 0.f;
	}

	Vector3 mPosition;
	Vector3 mVelocity;
	float mGain;
	float mPitch;
	float mRolloff;
	float mMinGain;
	float mMaxGain;
	bool mLooping;
	float mRefDist;
	float mMaxDist;
	float mOffset;

	BufferedSound* mBuffer;
};

class ActiveSound
{
public:

	ActiveSound();
	~ActiveSound();

	void update();
	void clear();
	bool isActive();
	void playSound(SoundData data);

	/** Get a reference to the sound data,  */
	SoundData& getData();

	/** Updates the sound to reflect any changes made to the sound properties */
	void updateData();

	time_t getStartTime();

protected:

	unsigned int mSource;
	SoundData mData;
	bool mActive;
	time_t mStart;
	OggStream* mOgg;

};

#endif
