#ifndef OAL_SUBSYSTEM_H
#define OAL_SUBSYSTEM_H

#include "StdHeaders.hpp"
#include "Subsystem.hpp"

#include "SoundPtr.hpp"
#include "BufferedSound.hpp"
#include "ActiveSound.hpp"

class AudioMgr : public Subsystem
{
public:

	AudioMgr();
	~AudioMgr();

	virtual void init();
	virtual void deinit();
	virtual void update(Real delta);
	virtual void endState();
	virtual String getName();

	SoundPtr play2D(String filename);
	bool bufferSound(String filename);
	void update();
	bool loadSound(String filename);
	// hacky, to be fixed:
	void setDefaultVolume(float volume);
	void setDefaultVolumeOgg(float volume);

	ActiveSound* getActiveSound(unsigned int index);

protected:

	bool mInitialized;

	std::map<std::string,BufferedSound*> mBuffers;
	std::vector<ActiveSound*> mSounds;
	unsigned int mMaxSources;

	float mDefaultVolume;
	float mDefaultVolumeOgg;
	bool mVolumeDirty;
	bool mVolumeOggDirty;
};

#endif
