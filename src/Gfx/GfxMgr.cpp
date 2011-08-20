#include "StdHeaders.hpp"
#include "GfxMgr.hpp"
#include "Game.hpp"
#include "GfxObject.hpp"

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
	mWidth = 800;
	mHeight = 600;
	init(mWidth, mHeight, false, false);
	mOyster = new Oyster::Oyster(mWidth,mHeight);
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
                "... - Ludum Dare 21 Entry - Copyright Riley Adams 2011",
                resX,resY,fullscreen,&miscP);

        mWindow->setActive(true);

        mSmgr = mRoot->createSceneManager(Ogre::ST_GENERIC,"mSceneManager");
        mCamera = mSmgr->createCamera("mCamera");
        mCamera->setPosition(0,0,0);
        mCamera->setDirection(0,0,-1);
        mCamera->setFarClipDistance(300);
        mCamera->setNearClipDistance(0.01f);
        mCamera->setFOVy(Ogre::Radian(Ogre::Degree(60)));

		//mCamera->setPosition(0,0,-1.5f);


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
		for(int i = 0; i < mGuis.size(); ++i)
			delete mGuis[i];
		for(int i = 0; i < mObjs.size(); ++i)
			delete mObjs[i];
		delete mOyster;
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
	for(int i = 0; i < mGuis.size(); ++i)
		mGuis[i]->update();
	renderFrame();
}
//---------------------------------------------------------------------------

void GfxMgr::endState()
{
	delete mOyster;
	mOyster = new Oyster::Oyster(mWidth,mHeight);
}

Gui* GfxMgr::createGui(Oyster::Batch* b)
{
	mGuis.push_back(new Gui(b, "GUI"));
	return mGuis.back();
}

void GfxMgr::removeGfxObject(GfxObject* obj) 
{
	for(int i = 0; i < mObjs.size(); ++i)
	{
		if(mObjs[i] == obj)
		{
			GfxObject* ob = mObjs[i];
			mObjs.erase(mObjs.begin() + i);
			delete ob;
			break;
		}
	}
}
