#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "StdHeaders.hpp"
#include "Subsystem.hpp"
#include "Ogre.h"

class GfxMgr : public Subsystem
{
public:

    GfxMgr();
    ~GfxMgr();

    void init();
    void init(uint resX,uint resY,bool vsync,bool fullscreen);
    void deinit();

    void renderFrame();

    void update(Real delta);

    Ogre::Root* getRoot(){return mRoot;}

	Ogre::RenderWindow* getWindow(){return mWindow;}
	Ogre::SceneManager* getSceneManager(){return mSmgr;}

	void endState();

	Ogre::SceneNode* camRoll;
	Ogre::SceneNode* camYaw;
	Ogre::SceneNode* camPitch;
	Ogre::SceneNode* camPos;
	Real pichLimit;
	Real pitch;
	bool cameraMotion;
    Ogre::Camera* mCamera;

private:

    Ogre::Root* mRoot;
    Ogre::SceneManager* mSmgr;
    Ogre::RenderWindow* mWindow;
    Ogre::Viewport* mViewport;

    bool mInitialized;
};

#endif
