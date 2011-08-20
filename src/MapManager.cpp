#include "MapManager.hpp"


MapManager::MapManager(Ogre::Camera* cam)
{
	mCam = cam;
	genTo = 5;
	buildTo = 4;
	gennedTo = 0;
}

MapManager::~MapManager()
{
	std::deque<Chunk*>::iterator it = mChunks.begin();
	for(it; it != mChunks.end(); ++it)
		delete *it;
	mChunks.clear();
}

void MapManager::update(Real delta)
{
	int dist = -mCam->getDerivedPosition().z;
	dist /= 32;
	if(dist + genTo > gennedTo)
		gen(dist);
}

void MapManager::gen(int d)
{
	// TODO: cleanup old chunks
	while(gennedTo < d + genTo)
	{
		if(mChunks.size())
		{
			Chunk* c = mChunks.back();
			c->next = new Chunk(Vector3(0,0,-gennedTo*32));
			mChunks.push_back(c->next);
			mChunks.back()->prev = c;
		}
		else // first one
		{
			mChunks.push_back(new Chunk(Vector3(0,0,-d*32)));
			mChunks.back()->prev = 0;
		}

		// do some magic procedural nonsense...
		for(int i = 0; i < WIDTH; ++i)
			for(int j = 0; j < HEIGHT; ++j)
				for(int k = 0; k < DEPTH; ++k)
		{
			// fill in a tunnel
			if(i == 0 || i == WIDTH-1 || j == 0 || j== HEIGHT-1)
				mChunks.back()->data[i][j][k] = rand()%15+1;
			else
				mChunks.back()->data[i][j][k] = !(rand()%100);
		}

		if(gennedTo > 0)
		{
			mChunks.back()->prev->next = mChunks.back();
			mChunks.back()->prev->build();
		}
		++gennedTo;
	}
}
