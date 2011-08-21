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
	m.texcoords[0].push_back(texcoords[direction][N].x + (1.f/ATLAS_DIM) * ((T-1)%ATLAS_DIM));\
	m.texcoords[0].push_back(texcoords[direction][N].y + (1.f/ATLAS_DIM) * ((T-1)/ATLAS_DIM));\
	lts[0]=getCube(c,x,y,z,LIGHTING_COORDS[direction][N][0]);\
	lts[1]=getCube(c,x,y,z,LIGHTING_COORDS[direction][N][1]);\
	lts[2]=getCube(c,x,y,z,LIGHTING_COORDS[direction][N][0],LIGHTING_COORDS[direction][N][1]);\
	d = 1;\
	if(lts[0] && lts[1])\
		{d = 2.5f;}\
	else\
		{d = 1.f + (lts[0] + lts[1] + lts[2]) * 0.5f;}\
	d*=0.95;\
	m.diffuse.push_back(1.f/d);\
	m.diffuse.push_back(1.f/d);\
	m.diffuse.push_back(1.f/d);\
	m.diffuse.push_back(1.f/d);\

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
	dat = new userdata;
	dat->c = this;
	dat->type = 0;
}

Chunk::~Chunk()
{
	obj->kill();
	for(int i = 0; i < exps.size(); ++i)
		delete exps[i];
	for(int i = 0; i < oexps.size(); ++i)
		delete oexps[i];
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
	tm.actor->setUserPointer(dat);
}

void Chunk::kill()
{
	tm.kill();
}

void Chunk::upe(Real delta)
{
	for(int i = 0; i < exps.size(); ++i)
		if(exps[i]->update(delta))
		{
			oexps.push_back(exps[i]);
			exps.erase(exps.begin() + i);
			--i;
		}
}

void Chunk::makeE(Vector3 pos, Real de)
{
	explosion* e  = 0;
	if(!oexps.empty())
	{
		e = oexps.back();
		oexps.pop_back();
		exps.push_back(e);
		e->reac(pos, de);
	}
	else
	{
		exps.push_back(new explosion(pos));
		e = exps.back();
		e->delay = de;
	}
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
	tm.actor->setUserPointer(dat);
}

int8 Chunk::getBlock(Vector3 hit, Vector3 normal)
{
	// transform into block space, a magical, poorly defined vector space
	Vector3 offset =Vector3(WIDTH/2-0.5f,HEIGHT/2-0.5f,DEPTH/2-0.5f); 
	Vector3 p = hit - obj->getNode()->getPosition();
	Vector3 loc = p - normal * 0.5f;
	int x = floor(loc.x + 0.5f);
	int y = floor(loc.y + 0.5f);
	int z = floor(loc.z + 0.5f);
	if(x < 7 && x >= 0 && y >= 0 && y < 7 && z >= 0 && z < 32)
		return data[x][y][z];
	return 0;
}

void explode(std::set<Chunk*>& cs, Chunk* c, int x, int y, int z, Real delay = 0.f)
{
	Chunk* cc = c;

	if(z < 0)
	{
		z += 32;
		cc = c->next;
	}
	else if(z > 31)
	{
		z -= 32;
		cc = c->prev;
	}

	if(!cc)
	{
		cs.insert(c);
		return;
	}

	if(x < 7 && x >= 0 && y >= 0 && y < 7 && z >= 0 && z < 32)
	{
		if(cc->data[x][y][z] == 2)
		{
			cc->data[x][y][z] = 0;
			cs.insert(cc);
			return;
		}
		if(cc->data[x][y][z] == 3)
		{
			cc->data[x][y][z] = 0;
			cc->makeE(Vector3(x,y,z) + cc->obj->getNode()->getPosition(), delay);
			explode(cs, cc, x-1, y, z,delay+0.0175f);
			explode(cs, cc, x+1, y, z,delay+0.0175f);
			explode(cs, cc, x, y+1, z,delay+0.0175f);
			explode(cs, cc, x, y-1, z,delay+0.0175f);
			explode(cs, cc, x, y, z+1,delay+0.0175f);
			explode(cs, cc, x, y, z-1,delay+0.0175f);
			cs.insert(cc);
			return;
		}
		else if(cc->data[x][y][z] != 6)
		{
			cc->data[x][y][z] = 0;
			cs.insert(cc);
			return;
		}
	}

	cs.insert(c);
}

void Chunk::shootBlock(Vector3 hit, Vector3 normal)
{
	// transform into block space, a magical, poorly defined vector space
	Vector3 offset =Vector3(WIDTH/2-0.5f,HEIGHT/2-0.5f,DEPTH/2-0.5f); 
	Vector3 p = hit - obj->getNode()->getPosition();
	Vector3 loc = p - (normal * 0.5f);
	int x = floor(loc.x + 0.5f);
	int y = floor(loc.y + 0.5f);
	int z = floor(loc.z + 0.5f);
	if(x < 7 && x >= 0 && y >= 0 && y < 7 && z >= 0 && z < 32)
	{
		if(data[x][y][z] == 2)
		{
			data[x][y][z] = 0;
			update();
		}
		else if(data[x][y][z] == 3)
		{
			data[x][y][z] = 0;
			std::set<Chunk*> c;

			makeE(Vector3(x,y,z) + obj->getNode()->getPosition(), 0.f);
			explode(c, this, x+1, y, z,0.0175f);
			explode(c, this, x-1, y, z,0.0175f);
			explode(c, this, x, y+1, z,0.0175f);
			explode(c, this, x, y-1, z,0.0175f);
			explode(c, this, x, y, z+1,0.0175f);
			explode(c, this, x, y, z-1,0.0175f);

			//c.insert(tryKill(this, x+1, y, z));
			//c.insert(tryKill(this, x-1, y, z));
			//c.insert(tryKill(this, x, y+1, z));
			//c.insert(tryKill(this, x, y-1, z));
			//c.insert(tryKill(this, x, y, z+1));
			//c.insert(tryKill(this, x, y, z-1));

			std::set<Chunk*>::iterator it = c.begin();
			for(it; it != c.end(); ++it) 
				(*it)->update();
		}
	}
}


Chunk::explosion::explosion(Vector3 pos)
{
	node = Game::getPtr()->getGfx()->getSceneManager()->createSceneNode();
	Game::getPtr()->getGfx()->getSceneManager()->getRootSceneNode()->addChild(node);
	ent = Game::getPtr()->getGfx()->getSceneManager()->createEntity("Debris11.mesh");
	node->attachObject(ent);
	ent->setMaterialName("boom");
	node->setPosition(pos);
	life = 0.2f;
}
bool Chunk::explosion::update(Real delta)
{
	delay -= delta;
	if(delay < 0.f)
	{
		life -= delta;
		if(life <= 0.f)
		{
			node->setVisible(false);
			return true;
		}
		else
		{
			node->setScale(Vector3(1,1,1) * (0.4-life)/0.2);
		}
	}
	return false;
}
void Chunk::explosion::reac(Vector3 pos, Real dela)
{
	node->setPosition(pos);
	node->setVisible(true);
	node->setScale(Vector3(1,1,1));
	delay = dela;
	life = 0.2f;
}
Chunk::explosion::~explosion()
{
	node->getCreator()->destroyEntity(ent);
	node->getCreator()->destroySceneNode(node);
}
