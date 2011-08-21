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

	Real destruction;

	struct Debris
	{
		Ogre::SceneNode* node;
		Ogre::Entity* entity;
		Vector3 dir;
		Real dist;
		bool changedCourse;
		Real life;
		Real delay;
	};

	void makeDebris(Chunk* c, int type, int i, int j, int k);
	void makeDebrisEx(Chunk* c, int type, int i, int j, int k);

	std::list<Debris*> mDebris;
	std::list<Debris*> mSpareDebris;

	Real delay;

};

#endif
