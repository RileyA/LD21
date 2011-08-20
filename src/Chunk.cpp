#include "Chunk.hpp"
#include "Game.hpp"
#include "Physics/PhysicsMgr.hpp"


// hacky constants...
const static Vector3 normals[6] = {
	Vector3(1,0,0),
	Vector3(-1,0,0),
	Vector3(0,1,0),
	Vector3(0,-1,0),
	Vector3(0,0,1),
	Vector3(0,0,-1)};


const static Vector3 verts[6][4] =
{
	{
		Vector3(-0.5,0.5,0.5),
		Vector3(-0.5,-0.5,-0.5),
		Vector3(-0.5,0.5,-0.5),
		Vector3(-0.5,-0.5,0.5)
	},
	{
		Vector3(0.5,-0.5,-0.5),
		Vector3(0.5,0.5,0.5),
		Vector3(0.5,0.5,-0.5),
		Vector3(0.5,-0.5,0.5)
	},
	{
		Vector3(-0.5,-0.5,-0.5),
		Vector3(0.5,-0.5,0.5),
		Vector3(0.5,-0.5,-0.5),
		Vector3(-0.5,-0.5,0.5)
	},
	{
		Vector3(0.5,0.5,0.5),
		Vector3(-0.5,0.5,-0.5),
		Vector3(0.5,0.5,-0.5),
		Vector3(-0.5,0.5,0.5)
	},
	{
		Vector3(0.5,0.5,-0.5),
		Vector3(-0.5,-0.5,-0.5),
		Vector3(0.5,-0.5,-0.5),
		Vector3(-0.5,0.5,-0.5)
	},
	{
		Vector3(-0.5,-0.5,0.5),
		Vector3(0.5,0.5,0.5),
		Vector3(0.5,-0.5,0.5),
		Vector3(-0.5,0.5,0.5)
	}
};

const static int ATLAS_DIM = 4;

const static Vector2 texcoords[6][4] =
{
	{
		Vector2(0.f/ATLAS_DIM, 1.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 0.f/ATLAS_DIM),
		Vector2(0.f/ATLAS_DIM, 0.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 1.f/ATLAS_DIM)
	},
	{
		Vector2(0.f/ATLAS_DIM, 0.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 1.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 0.f/ATLAS_DIM),
		Vector2(0.f/ATLAS_DIM, 1.f/ATLAS_DIM)
	},
	{
		Vector2(0.f/ATLAS_DIM, 0.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 1.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 0.f/ATLAS_DIM),
		Vector2(0.f/ATLAS_DIM, 1.f/ATLAS_DIM)
	},
	{
		Vector2(0.f/ATLAS_DIM, 1.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 0.f/ATLAS_DIM),
		Vector2(0.f/ATLAS_DIM, 0.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 1.f/ATLAS_DIM)
	},
	{
		Vector2(1.f/ATLAS_DIM, 0.f/ATLAS_DIM),
		Vector2(0.f/ATLAS_DIM, 1.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 1.f/ATLAS_DIM),
		Vector2(0.f/ATLAS_DIM, 0.f/ATLAS_DIM)
	},
	{
		Vector2(0.f/ATLAS_DIM, 1.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 0.f/ATLAS_DIM),
		Vector2(1.f/ATLAS_DIM, 1.f/ATLAS_DIM),
		Vector2(0.f/ATLAS_DIM, 0.f/ATLAS_DIM)
	}
};

// I'm going to programmer hell for this, aren't I?...
#define VERT(N, T) m.vertices.push_back(verts[direction][N].x+x);\
	m.vertices.push_back(verts[direction][N].y+y);\
	m.vertices.push_back(verts[direction][N].z+z);\
	m.texcoords[0].push_back(texcoords[direction][N].x + (1.f/ATLAS_DIM) * (T%ATLAS_DIM));\
	m.texcoords[0].push_back(texcoords[direction][N].y + (1.f/ATLAS_DIM) * (T/ATLAS_DIM));

void makeQuad(MeshData& m, int direction, byte x, byte y, byte z, byte t, byte c[6])
{
	int atlas = t;
	VERT(0,atlas)
	VERT(1,atlas)
	VERT(2,atlas)
	VERT(0,atlas)
	VERT(3,atlas)
	VERT(1,atlas)
}

Chunk::Chunk(Vector3 pos)
{
	next = 0;
	prev = 0;
	// create node, but not mesh just yet...
	obj = new GfxObject("test", pos - Vector3(WIDTH/2-0.5f,HEIGHT/2-0.5f,DEPTH/2-0.5f));
}

Chunk::~Chunk()
{
	obj->kill();
}

void Chunk::build()
{
	d.addTexcoordSet();
	for(int i = 0; i < WIDTH; ++i)
		for(int j = 0; j < HEIGHT; ++j)
			for(int k = 0; k < DEPTH; ++k)
	{
		if(data[i][j][k])
			continue;
		if(i < WIDTH-1 && data[i+1][j][k])
			makeQuad(d, 1, i, j, k, data[i+1][j][k], 0);
		if(i > 0 && data[i-1][j][k])
			makeQuad(d, 0, i, j, k, data[i-1][j][k], 0);
		if(j < HEIGHT-1 && data[i][j+1][k])
			makeQuad(d, 3, i, j, k, data[i][j+1][k], 0);
		if(j > 0 && data[i][j-1][k])
			makeQuad(d, 2, i, j, k, data[i][j-1][k], 0);
		if(k < DEPTH-1 && data[i][j][k+1])
			makeQuad(d, 5, i, j, k, data[i][j][k+1], 0);
		if(k > 0 && data[i][j][k-1])
			makeQuad(d, 4, i, j, k, data[i][j][k-1], 0);
		if(k == 0 && next && next->data[i][j][DEPTH-1])
			makeQuad(d, 4, i, j, k, next->data[i][j][DEPTH-1], 0);
		if(k == DEPTH-1 && prev && prev->data[i][j][0])
			makeQuad(d, 5, i, j, k, prev->data[i][j][0], 0);
	}
	
	obj->load(d); 
	tm = Game::getPtr()->getPhysics()->createStaticTrimesh(d, obj->getNode()->getPosition());
}
