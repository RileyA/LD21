#include "SoundPtr.hpp"
#include "AudioMgr.hpp"

SoundPtr::SoundPtr(unsigned int index,time_t start,AudioMgr* amgr)
	:mStart(start),
	mSoundIndex(index),
	mAudioMgr(amgr)
{}
//-----------------------------------------------------------------------

SoundPtr::SoundPtr()
{
	mStart = false;
	mAudioMgr = 0;
	mSoundIndex = 5000;
}
//-----------------------------------------------------------------------

SoundData& SoundPtr::getData()
{
	return mAudioMgr->getActiveSound(mSoundIndex)->getData();
}
//-----------------------------------------------------------------------

bool SoundPtr::isValid()
{
	return mAudioMgr->getActiveSound(mSoundIndex)->getStartTime() == mStart;
}
//-----------------------------------------------------------------------

bool SoundPtr::isActive()
{
	return isValid() && mAudioMgr->getActiveSound(mSoundIndex)->isActive();
}
