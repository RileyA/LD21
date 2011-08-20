#ifndef GfxObject_H
#define GfxObject_H

#include "StdHeaders.hpp"
#include "Ogre.h"
#include "MeshData.hpp"

using Ogre::Vector3;

class GfxObject
{
public:

	GfxObject(const MeshData& md, String material);
	GfxObject(String material, Vector3 pos);
	GfxObject(String mesh); 
	GfxObject(String mesh, String name);
	virtual ~GfxObject();

	void kill();
	void unload();
	void load(const MeshData& d); 

	Ogre::Entity* getEntity(){return mEntity;}
	Ogre::SceneNode* getNode(){return mNode;}

private:

	Ogre::SceneNode* mNode;
	Ogre::Entity* mEntity;
	Ogre::SceneManager* mSmgr;

	static int mAutoNameIndex;
	String nombre;
	String mat;

};

#endif
