#ifndef BUFFEREDSOUND_H
#define BUFFEREDSOUND_H

#include "StdHeaders.hpp"

class BufferedSound
{
public:

	/** Constructor
	@remarks
		Give it a filename and it loads the data up, you also have the option
		to not immediately load the sound up (and later call BufferedSound::load())
		The file should be verified to be a sound file prior to being loaded.
	@param filename The file to be loaded (.wav only for now)
	@param delayLoading Don't immediately load the sound (default: false) */
	BufferedSound(const String& filename,bool isOgg,bool delayLoading=false);

	/** Destructor */
	~BufferedSound();

	/** (re)Loads the sound, unless otherwise specified, this will be done automatically
	@returns Whether or not it was sucessful */
	bool load();

	/** Unloads the sound from memory (it can still be reloaded with BufferedSound::load()) */
	void unload();

	/** Whether or not the sound is currently loaded
	@returns Whether or not it's loaded */
	bool isLoaded();

	/** Whether or not the sound is an Ogg Vorbis stream
	@remarks
		Ogg sounds will not be loaded into memory, only streamed (for now at least)
	@returns Whether or not it is an .ogg */
	bool isOgg();

	unsigned int getBuffer();
	String getFilename();

protected:

	// The OpenAL "name" for the buffer
	unsigned int mBuffer;

	// The filename for the sound
	String mFilename;

	// Whether or not it is loaded
	bool mLoaded;

	// If this sound is a .ogg
	bool mOgg;

};

#endif
