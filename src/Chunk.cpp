#include "Chunk.hpp"
#include "Game.hpp"
#include "Physics/PhysicsMgr.hpp"

typedef signed char int8;

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

const int8 LIGHTING_COORDS[6][4][2] = {
	{{2,4},{5,3},{2,5},{4,3}},//+x
	{{3,5},{4,2},{2,5},{3,4}},//-x
	{{1,5},{4,0},{5,0},{4,1}},//+y
	{{0,4},{5,1},{0,5},{1,4}},//-y
	{{2,0},{1,3},{3,0},{2,1}},//+z
	{
		{1,3},//0
		{2,0},//1
		{0,3},//2
		{1,2}//3
	}//-z
};

const int8 offsets[6][3] =
{
	{1,0,0},
	{-1,0,0},
	{0,1,0},
	{0,-1,0},
	{0,0,1},
	{0,0,-1}
};

bool getCube(Chunk* c, int i, int j, int k, int d1, int8 d2)
{
	i += offsets[d1][0];
	i += offsets[d2][0];
	j += offsets[d1][1];
	j += offsets[d2][1];
	k += offsets[d1][2];
	k += offsets[d2][2];

	// outta bounds
	if(i < 0 || j < 0 || i > WIDTH-1 || j > HEIGHT-1)
		return false;
	
	if(k < 0)
	{
		k += 32;
		c = c->next;
	}
	else if(k > 31)
	{
		k -= 32;
		c = c->prev;
	}

	if(!c)
		return 0; // outta bounds

	return c->data[i][j][k];
}

bool getCube(Chunk* c, int i, int j, int k, int8 d1)
{
	i += offsets[d1][0];
	j += offsets[d1][1];
	k += offsets[d1][2];

	// outta bounds
	if(i < 0 || j < 0 || i > WIDTH-1 || j > HEIGHT-1)
		return false;
	
	if(k < 0)
	{
		k += 32;
		c = c->next;
	}
	else if(k > 31)
	{
		k -= 32;
		c = c->prev;
	}

	if(!c)
		return 0; // outta bounds

	return c->data[i][j][k];
}

bool getCube(Chunk* c, int i, int j, int k)
{
	// outta bounds
	if(i < 0 || j < 0 || i > WIDTH-1 || j > HEIGHT-1)
		return false;
	
	if(k < 0)
	{
		k += 32;
		c = c->next;
	}
	else if(k > 31)
	{
		k -= 32;
		c = c->prev;
	}

	if(!c)
		return 0; // outta bounds

	return c->data[i][j][k];
}

// I'm going to programmer hell for this, aren't I?...
#define VERT(N, T) m.vertices.push_back(verts[direction][N].x+x);\
	m.vertices.push_back(verts[direction][N].y+y);\
	m.vertices.push_back(verts[direction][N].z+z);\
	m.texcoords[0].push_back(texcoords[direction][N].x + (1.f/ATLAS_DIM) * (T%ATLAS_DIM));\
	m.texcoords[0].push_back(texcoords[direction][N].y + (1.f/ATLAS_DIM) * (T/ATLAS_DIM));\
	lts[0]=getCube(c,x,y,z,LIGHTING_COORDS[direction][N][0]);\
	lts[1]=getCube(c,x,y,z,LIGHTING_COORDS[direction][N][1]);\
	lts[2]=getCube(c,x,y,z,LIGHTING_COORDS[direction][N][0],LIGHTING_COORDS[direction][N][1]);\
	d = 1;\
	if(lts[0] && lts[1])\
		{d = 2.5f;}\
	else\
		{d = 1.f + (lts[0] + lts[1] + lts[2]) * 0.5f;}\
	d*=0.75;\
	m.diffuse.push_back(1.f);\
	m.diffuse.push_back(1.f);\
	m.diffuse.push_back(1.f);\
	m.diffuse.push_back(1.f);\

void makeQuad(MeshData& m, int direction, byte x, byte y, byte z, byte t, Chunk* c)
{
	bool lts[3];
	float d;
	VERT(0,t)
	VERT(1,t)
	VERT(2,t)
	VERT(0,t)
	VERT(3,t)
	VERT(1,t)
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
			makeQuad(d, 1, i, j, k, data[i+1][j][k], this);
		if(i > 0 && data[i-1][j][k])
			makeQuad(d, 0, i, j, k, data[i-1][j][k], this);
		if(j < HEIGHT-1 && data[i][j+1][k])
			makeQuad(d, 3, i, j, k, data[i][j+1][k], this);
		if(j > 0 && data[i][j-1][k])
			makeQuad(d, 2, i, j, k, data[i][j-1][k], this);
		if(k < DEPTH-1 && data[i][j][k+1])
			makeQuad(d, 5, i, j, k, data[i][j][k+1], this);
		if(k > 0 && data[i][j][k-1])
			makeQuad(d, 4, i, j, k, data[i][j][k-1], this);
		if(k == 0 && next && next->data[i][j][DEPTH-1])
			makeQuad(d, 4, i, j, k, next->data[i][j][DEPTH-1], this);
		if(k == DEPTH-1 && prev && prev->data[i][j][0])
			makeQuad(d, 5, i, j, k, prev->data[i][j][0], this);
	}
	
	obj->load(d); 
	tm = Game::getPtr()->getPhysics()->createStaticTrimesh(d, obj->getNode()->getPosition());
}

void Chunk::kill()
{
	tm.kill();
}

void Chunk::update()
{
	tm.kill();
	obj->unload();
	
	d.vertices.clear();
	d.texcoords[0].clear();
	d.diffuse.clear();

	for(int i = 0; i < WIDTH; ++i)
		for(int j = 0; j < HEIGHT; ++j)
			for(int k = 0; k < DEPTH; ++k)
	{
		if(data[i][j][k])
			continue;

		if(i < WIDTH-1 && data[i+1][j][k])
			makeQuad(d, 1, i, j, k, data[i+1][j][k], this);
		if(i > 0 && data[i-1][j][k])
			makeQuad(d, 0, i, j, k, data[i-1][j][k], this);
		if(j < HEIGHT-1 && data[i][j+1][k])
			makeQuad(d, 3, i, j, k, data[i][j+1][k], this);
		if(j > 0 && data[i][j-1][k])
			makeQuad(d, 2, i, j, k, data[i][j-1][k], this);
		if(k < DEPTH-1 && data[i][j][k+1])
			makeQuad(d, 5, i, j, k, data[i][j][k+1], this);
		if(k > 0 && data[i][j][k-1])
			makeQuad(d, 4, i, j, k, data[i][j][k-1], this);
		if(k == 0 && next && next->data[i][j][DEPTH-1])
			makeQuad(d, 4, i, j, k, next->data[i][j][DEPTH-1], this);
		if(k == DEPTH-1 && prev && prev->data[i][j][0])
			makeQuad(d, 5, i, j, k, prev->data[i][j][0], this);
	}
		
	obj->load(d); 
	tm = Game::getPtr()->getPhysics()->createStaticTrimesh(d, obj->getNode()->getPosition());
}
