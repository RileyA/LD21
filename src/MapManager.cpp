#include "MapManager.hpp"
#include "Game.hpp"

typedef signed char int8;


MapManager::MapManager(Ogre::Camera* cam)
{
	mCam = cam;
	genTo = 5;
	buildTo = 4;
	gennedTo = 0;
	destruction = 5.f;
	delay = 1.f;
	ds = 12.f;
}

MapManager::~MapManager()
{
	std::deque<Chunk*>::iterator it = mChunks.begin();
	for(it; it != mChunks.end(); ++it)
		delete *it;
	mChunks.clear();


	std::list<Debris*>::iterator i = mDebris.begin();
	for(i; i != mDebris.end(); ++i)
		delete *i;
	mDebris.clear();

	i = mSpareDebris.begin();
	for(i; i != mSpareDebris.end(); ++i)
		delete *i;
	mSpareDebris.clear();
}

// get a "standard" block to put somewhere
int8 getNormalBlock()
{
	int a = rand()%100;
	if(a < 80)
		return 1;
	else if(a < 93)
		return 2;
	else if(a < 95)
		return 6;
	else
		return 6;
}

int8 getTrackBlock(int seed)
{
	if(seed < 150)
		return 1;
	else if(seed < 375)
		return getNormalBlock();
	else if(seed < 455)
		return rand()%3 == 1 ? getNormalBlock() : 2;
	else if(seed < 475)
		return 4;
	else if(seed < 495)
		return 5;
	else if(seed < 499)
		return 6;
	else
		return 6;
}

void MapManager::update(Real delta)
{
	Chunk* p = mChunks.empty() ? 0 : mChunks.front();

	while(p)
	{
		p->upe(delta);
		p = p->next;
	}
	
	int dist = -mCam->getDerivedPosition().z;
	dist /= 32;
	if(dist + genTo > gennedTo)
		gen(dist);

	destruction -= delta * ds;

	if(mGame->getGfx()->mCamera->getDerivedPosition().z - destruction <= -100)
	{
		destruction = mGame->getGfx()->mCamera->getDerivedPosition().z + 25;
		ds += 0.5f;
	}

	delay -=delta;

	if(delay < 0.f)
	{
		delay = 0.4f;
	//std::cout<<destruction<<"\n";

		// only do this every second or so
		Chunk* c = mChunks.empty() ? 0 : mChunks.front();


		while(c)
		{
			bool updatePrev = false;
			int ch = 0;
			if(destruction < c->obj->getNode()->getPosition().z + 16)
			{
				int dd = (c->obj->getNode()->getPosition().z + 16) - destruction;
				//std::cout<<dd<<"\n";
				for(int i = 0; i < WIDTH; ++i)
					for(int j = 0; j < HEIGHT; ++j)
						for(int k = DEPTH - 1; k > 32 - dd && k >= 0; --k)
				{
					if(c->data[i][j][k])
					{
						if(k > (32-dd) + 25)
						{
							makeDebrisEx(c,c->data[i][j][k],i,j,k);
							c->data[i][j][k] = 0;
							++ch;
						}
						else if(rand()%20 < 4)		
						{
							makeDebris(c,c->data[i][j][k],i,j,k);
							c->data[i][j][k] = 0;
							++ch;
							//if(k == 31)
							//	updatePrev = true;
						}
					}
				}
			}

			//if(updatePrev && c->prev)
			//{
			//	c->prev->update();
			//}

			if(ch)
			{
				c->update();
				c = c->next;
			}
			else if(destruction < c->obj->getNode()->getPosition().z - 10)
			{
				Chunk* cc = c;
				c = c->next;
				cc->kill();
				c->prev = 0;
				delete cc;
				mChunks.pop_front();
			}
			else
			{
				c = c->next;
			}

		}
	}

	/*if(mGame->getInput()->wasKeyPressed("KC_V"))
	{
		for(int i = 0; i < WIDTH; ++i)
			for(int j = 0; j < HEIGHT; ++j)
				for(int k = 0; k < DEPTH; ++k)
		{
			if(rand()%100 == 4 && mChunks.front()->data[i][j][k])
			{
				makeDebris(mChunks.front(),mChunks.front()->data[i][j][k],i,j,k);
				mChunks.front()->data[i][j][k] = 0;
			}
		}

		mChunks.front()->update();
	}*/

	//std::cout<<"debz: "<<mDebris.size()<<"\n";
	for(std::list<Debris*>::iterator it = mDebris.begin(); it != mDebris.end(); ++it)
	{
		if((*it)->delay > 0.f)
		{
			(*it)->delay -= delta;
			continue;
		}
		(*it)->node->translate((*it)->dir * delta * 1.5f);
		(*it)->box->setPosition((*it)->node->getPosition());
		(*it)->dist += delta * 0.75f;
		(*it)->life -= delta;
		if((*it)->dist > 1.2f && !(*it)->changedCourse)
		{
			(*it)->dir = Vector3(
				(*it)->node->getPosition().x + Rand::randFloat(-2.f,2.f),
				(*it)->node->getPosition().y + Rand::randFloat(-2.f,2.f),
				8 + Rand::randFloat(0.f,0.8f));
			(*it)->dir.normalise();
			(*it)->dir *= 1.75f;
			(*it)->changedCourse = true;
		}
		if((*it)->life <= 0.f)
		{
			(*it)->node->setVisible(false);
			(*it)->node->getParentSceneNode()->removeChild((*it)->node);
			mSpareDebris.push_back((*it));
			it = mDebris.erase(it);
		}
	}
	//for(int i = 0; i < mDebris.size(); ++i)
	//{
	//}
}

void MapManager::gen(int d)
{
	// TODO: cleanup old chunks
	
	if(!mChunks.empty())
	{
		Chunk* c = mChunks.front();
		while(c && ((c->obj->getNode()->getPosition().z - 15.5)/-32.f) < gennedTo - 5)
		{
			mChunks.pop_front();
			Chunk* cc = c;
			c = c->next;
			cc->kill();
			delete cc;
		}
	}

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

		// do some magic procedural nonsense 

		Chunk* ch = mChunks.back();
		int tp[4] = {0,0,0,0};

		int obs = 0;

		for(int k = 0; k < DEPTH; ++k)
		{
			if(k % 4 == 0)
			{
				for(int i = 0; i < 4; ++i)
					tp[i] = rand()%500;
			};

			// corners are always normal
			ch->data[0][0][k] = getNormalBlock();
			ch->data[6][6][k] = getNormalBlock();
			ch->data[0][6][k] = getNormalBlock();
			ch->data[6][0][k] = getNormalBlock();

			// empty out the center
			for(int i = 1; i < 6; ++i)
				for(int j = 1; j < 6; ++j)
			{
				ch->data[i][j][k] = 0;
			}

			// generate tracks
			for(int j = 1; j < 6; ++j)
			{
				ch->data[j][0][k] = getTrackBlock(tp[0]);
				ch->data[j][6][k] = getTrackBlock(tp[1]);
				ch->data[0][j][k] = getTrackBlock(tp[2]);
				ch->data[6][j][k] = getTrackBlock(tp[3]);
			}

			obs -= 1;

			if(obs <= 0 && gennedTo > 3)
			{
				obs = rand()%8+5;
				int typ = rand()%4;
				// column
				{
					int axis = rand()%2;
					int spot = rand()%5 + 1;
					if(axis)
						for(int i = 1; i < 6; ++i)
							ch->data[spot][i][k] = 1;
					else
						for(int i = 1; i < 6; ++i)
							ch->data[i][spot][k] = 1;
				}
			}
		}


		/*for(int i = 0; i < WIDTH; ++i)
			for(int j = 0; j < HEIGHT; ++j)
				for(int k = 0; k < DEPTH; ++k)
		{
			// fill in a tunnel
			if(i == 0 || i == WIDTH-1 || j == 0 || j== HEIGHT-1)
			{
				if((i == 0 && j == 0) || (i == 6 && j == 6)
					(i == 6 && j == 0) || (i == 0 && j == 6))
				{
					mChunks.back()->data[i][j][k] = 1;
				}
				else
				{
					
				}
			}
			//else
			//	mChunks.back()->data[i][j][k] = 0;//!(rand()%100);
		}*/

		if(gennedTo > 0)
		{
			mChunks.back()->prev->next = mChunks.back();
			mChunks.back()->prev->build();
		}
		++gennedTo;
	}
}

void MapManager::makeDebris(Chunk* c, int type, int i, int j, int k)
{
	Debris* d = 0;
	if(!mSpareDebris.empty())
	{
		std::list<Debris*>::iterator it = mSpareDebris.begin();
		for(it; it != mSpareDebris.end(); ++it)
		{
			if((*it)->type == type)
			{
				d = (*it);
				mSpareDebris.erase(it);
				break;
			}
		}
	}
	if(!d)
	{
		d = new Debris();
		d->node = mGame->getGfx()->getSceneManager()->createSceneNode();
		d->entity = mGame->getGfx()->getSceneManager()->createEntity("Debris" + StringUtils::toString(type)  + ".mesh");
		d->type = type;
		d->node->attachObject(d->entity);
		d->box = mGame->getPhysics()->createCube(Vector3(0.5f,0.5f,0.5f), Vector3(0,0,0));
		d->box->setKinematic(true);
		d->dat = new userdata;
		d->dat->c = 0;
		d->dat->type = type;
		d->box->setUserData(d->dat);
	}

	mDebris.push_back(d);
	Vector3 p = c->obj->getNode()->getPosition() + Vector3(i,j,k);
	d->node->setPosition(p);
	d->box->setPosition(p);

	d->dir = Vector3(0,0,1);

	if(i == 0)
		d->dir = Vector3(-1,0,0);
	else if(i == 6)
		d->dir = Vector3(1,0,0);
	if(j == 0)
		d->dir = Vector3(0,-1,0);
	else if(j == 6)
		d->dir = Vector3(0,1,0);

	mGame->getGfx()->getSceneManager()->getRootSceneNode()->addChild(d->node);
	d->node->setVisible(true);
	d->changedCourse = false;
	d->dist = 0.f;
	d->delay = Rand::randFloat(0.f,0.375f);
	d->life = 0.5f;
}

void MapManager::makeDebrisEx(Chunk* c, int type, int i, int j, int k)
{
	Debris* d = 0;
	if(!mSpareDebris.empty())
	{
		std::list<Debris*>::iterator it = mSpareDebris.begin();
		for(it; it != mSpareDebris.end(); ++it)
		{
			if((*it)->type == type)
			{
				d = (*it);
				mSpareDebris.erase(it);
				break;
			}
		}
	}
	if(!d)
	{
		d = new Debris();
		d->node = mGame->getGfx()->getSceneManager()->createSceneNode();
		d->type = type;
		d->entity = mGame->getGfx()->getSceneManager()->createEntity("Debris" + StringUtils::toString(type)  + ".mesh");
		d->node->attachObject(d->entity);
		d->box = mGame->getPhysics()->createCube(Vector3(0.5f,0.5f,0.5f), Vector3(0,0,0));
		d->box->setKinematic(true);
		d->dat = new userdata;
		d->dat->c = 0;
		d->dat->type = type;
		d->box->setUserData(d->dat);
	}

	mDebris.push_back(d);
	Vector3 p = c->obj->getNode()->getPosition() + Vector3(i,j,k);
	d->node->setPosition(p);
	d->box->setPosition(p);

	d->dir = d->node->getPosition();
	d->dir.x *= Rand::randFloat(0.2f, 2.f);
	d->dir.y *= Rand::randFloat(0.2f, 2.f);
	d->dir.z = Rand::randFloat(5.f, 10.f);
	d->dir.normalise();
	d->dir *= Rand::randFloat(1.5f, 3.5f);

	mGame->getGfx()->getSceneManager()->getRootSceneNode()->addChild(d->node);
	d->node->setVisible(true);
	d->changedCourse = true;
	d->dist = 5.f;
	d->life = Rand::randFloat(0.5f,1.25f);
	d->delay = Rand::randFloat(0.f,0.375f);
}
