#ifndef Chunk_H
#define Chunk_H

#include "Object.hpp"
#include "Gfx/GfxObject.hpp"
#include "Physics/PhysicsMgr.hpp"
 
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

	Chunk* next;
	Chunk* prev;
	byte data[WIDTH][HEIGHT][DEPTH];
	MeshData d;
	Vector3 position;
	GfxObject* obj;
	Trimesh tm;

};

#endif
