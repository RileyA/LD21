#ifndef OGGSTREAM_H
#define OGGSTREAM_H

#include "StdHeaders.hpp"

// OpenAl includes
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
// Ogg Vorbis stuff
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#define BUFFER_SIZE (4096 * 8)

class OggStream
{
public:

	OggStream();
	~OggStream();

	void open(String path,unsigned int source);
	void release();
	void display();
	bool playback();
	bool playing();
	bool update();

	unsigned int getSource();

protected:

	bool stream(unsigned int buffer);
	void empty();
	void check();
	String errorString(int code);

	FILE*           mOggFile;       // file handle
	OggVorbis_File  mOggStream;     // stream handle
	vorbis_info*    mVorbisInfo;    // some formatting data
	vorbis_comment* mVorbisComment; // user comments

	unsigned mBuffers[2]; // front and back buffers
	int mFormat;     // internal format

	unsigned int mSource;

	bool mOpen;
};

#endif
