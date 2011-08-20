#ifndef MapManager_h
#define MapManager_h

#include "Object.hpp"
#include "Chunk.hpp"
#include "Ogre.h"

class MapManager : public Object
{
public:

	MapManager(Ogre::Camera* cam);
	~MapManager();

	void update(Real delta);

	void gen(int d);

	std::deque<Chunk*> mChunks;
	Ogre::Camera* mCam;
	int genTo;
	int buildTo;
	int gennedTo; 

};

#endif
