#ifndef GfxObject_H
#define GfxObject_H

#include "StdHeaders.hpp"
#include "Ogre.h"
#include "MeshData.hpp"
#include "Game.hpp"

class GfxObject
{
public:

	GfxObject(const MeshData& md, String material);
	GfxObject(String mesh); 
	GfxObject(String mesh, String name);
	virtual ~GfxObject();

	void kill();

	Ogre::Entity* getEntity(){return mEntity;}
	Ogre::SceneNode* getNode(){return mNode;}

private:

	Ogre::SceneNode* mNode;
	Ogre::Entity* mEntity;
	Ogre::SceneManager* mSmgr;

	static int mAutoNameIndex;

};

#endif
