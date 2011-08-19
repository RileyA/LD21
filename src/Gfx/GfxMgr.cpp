#include "StdHeaders.hpp"
#include "GfxMgr.hpp"
#include "Game.hpp"

GfxMgr::GfxMgr()
    :mRoot(0),
    mSmgr(0),
    mWindow(0),
    mInitialized(false)
{

}
//---------------------------------------------------------------------------

GfxMgr::~GfxMgr()
{
    deinit();
}
//---------------------------------------------------------------------------

void GfxMgr::init()
{
	// you didn't see anything...
	init(800, 600, false, false);
}
//---------------------------------------------------------------------------

void GfxMgr::init(uint resX,uint resY,bool vsync,bool fullscreen)
{
    if(!mInitialized)
    {
        mRoot = new Ogre::Root("plugins.cfg","");

		//Ogre::RenderSystem* rs = mRoot->getRenderSystemByName(
        //    "Direct3D9 Rendering Subsystem");

		Ogre::RenderSystem* rs = mRoot->getRenderSystemByName(
            "OpenGL Rendering Subsystem");
		mRoot->setRenderSystem(rs);

		mRoot->initialise(false);

		Ogre::NameValuePairList miscP;

        if(vsync)
			miscP["vsync"] = "true";
		else
			miscP["vsync"] = "false";
		miscP["FSAA"] = "0";
		miscP["gamma"] = "false";
		miscP["border"] = "fixed";
		miscP["colourDepth"] = "32";
		miscP["monitorIndex"] = "0";

		mWindow = mRoot->createRenderWindow(
                "... - Ludum Dare 21 Entry - Copyright Riley Adams 2010",
                resX,resY,fullscreen,&miscP);

        mWindow->setActive(true);

        mSmgr = mRoot->createSceneManager(Ogre::ST_GENERIC,"mSceneManager");
        mCamera = mSmgr->createCamera("mCamera");
        mCamera->setPosition(0,0,0);
        mCamera->setDirection(0,0,-1);
        mCamera->setFarClipDistance(300);
        mCamera->setNearClipDistance(0.01f);
        mCamera->setFOVy(Ogre::Radian(Ogre::Degree(60)));

		mCamera->setPosition(0,0,-1.5f);

		camRoll = mSmgr->createSceneNode();
		camYaw = mSmgr->createSceneNode();
		camPitch = mSmgr->createSceneNode();
		camPos =
		mSmgr->createSceneNode();

		mSmgr->getRootSceneNode()->addChild(camPos);
		camPos->addChild(camYaw);
		camYaw->addChild(camPitch);
		camPitch->addChild(camRoll);
		camRoll->attachObject(mCamera);
		
		// for external use...
		pichLimit = 80.f;
		pitch = 0.f;
		cameraMotion = false;

        mViewport = mWindow->addViewport(mCamera);
		mViewport->setBackgroundColour(Ogre::ColourValue(44.f/455.f,90.f/455.f,93.f/455.f));
        mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth())
                                / Ogre::Real(mViewport->getActualHeight()));

        Ogre::ResourceGroupManager* resourceMgr = Ogre::ResourceGroupManager::getSingletonPtr();
        resourceMgr->addResourceLocation("","FileSystem","General");
        resourceMgr->addResourceLocation("media","FileSystem","General");
        resourceMgr->addResourceLocation("media/models","FileSystem","General");
        resourceMgr->addResourceLocation("media/materials","FileSystem","General");
        resourceMgr->addResourceLocation("media/textures","FileSystem","General");
        resourceMgr->addResourceLocation("media/audio","FileSystem","General");
		resourceMgr->addResourceLocation("media/gui","FileSystem","General");
        resourceMgr->addResourceLocation("script","FileSystem","General");
		resourceMgr->initialiseAllResourceGroups();

        mInitialized = true;
    }
}
//---------------------------------------------------------------------------

void GfxMgr::deinit()
{
    if(mInitialized)
    {
		endState();
        delete mRoot;
        mRoot = 0;

        mInitialized = false;
    }
}
//---------------------------------------------------------------------------

void GfxMgr::renderFrame()
{
	Ogre::WindowEventUtilities::messagePump();
	mRoot->renderOneFrame();
}
//---------------------------------------------------------------------------

void GfxMgr::update(Real delta)
{
	renderFrame();
}
//---------------------------------------------------------------------------

void GfxMgr::endState()
{
}
