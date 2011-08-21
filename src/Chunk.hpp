#ifndef Chunk_H
#define Chunk_H

#include "Object.hpp"
#include "Gfx/GfxObject.hpp"
#include "Physics/PhysicsMgr.hpp"
 
typedef signed char int8;

#define WIDTH 7
#define HEIGHT 7
#define DEPTH 32

class Chunk
{
public:

	Chunk(Vector3 pos);
	virtual ~Chunk();

	void kill();

	void build();

	void update();

	void setPosition(Vector3 p);

	int8 getBlock(Vector3 hit, Vector3 normal);
	void shootBlock(Vector3 hit, Vector3 normal);

	void upe(Real delta);
	void makeE(Vector3 pos, Real de);

	struct explosion
	{
		Ogre::SceneNode* node;
		Ogre::Entity* ent;
		Real life;
		Real delay;
		explosion(Vector3 pos);
		bool update(Real delta);
		void reac(Vector3 pos, Real dela);
		~explosion();
	};

	std::vector<explosion*> exps;
	std::vector<explosion*> oexps;

	Chunk* next;
	Chunk* prev;
	byte data[WIDTH][HEIGHT][DEPTH];
	MeshData d;
	Vector3 position;
	GfxObject* obj;
	Trimesh tm;
	userdata* dat;

};

#endif
