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
	numShots = 0;
	mViewport2 = 0;;
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
	mWidth = 1024;
	mHeight = 768;
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

		/*Ogre::RenderSystem* rs = mRoot->getRenderSystemByName(
            "OpenGL Rendering Subsystem");
		mRoot->setRenderSystem(rs);

		mRoot->initialise(false);

		Ogre::NameValuePairList miscP;

        if(vsync)
			miscP["vsync"] = "true";
		else
			miscP["vsync"] = "false";
		miscP["FSAA"] = "4";
		miscP["gamma"] = "false";
		miscP["border"] = "fixed";
		miscP["colourDepth"] = "32";
		miscP["monitorIndex"] = "0";*/
	
		if(!mRoot->showConfigDialog())	
			throw std::runtime_error("ohno");// ... 

		mWindow = mRoot->initialise(true);

		//mWindow = mRoot->createRenderWindow(
         //       "Fragmentation - Ludum Dare 21 Entry - Riley Adams",
           //     resX,resY,fullscreen,&miscP);

        //mWindow->setActive(true);

        mSmgr = mRoot->createSceneManager(Ogre::ST_GENERIC,"mSceneManager");
        mCamera = mSmgr->createCamera("mCamera");
        mCamera->setPosition(0,0,0);
        mCamera->setDirection(0,0,-1);
        mCamera->setFarClipDistance(300);
        mCamera->setNearClipDistance(0.01f);
        mCamera->setFOVy(Ogre::Radian(Ogre::Degree(60)));

        mCamera2 = mSmgr->createCamera("mCamera2");
        mCamera2->setPosition(0,0,0);
        mCamera2->setDirection(0,0,1);
        mCamera2->setFarClipDistance(300);
        mCamera2->setNearClipDistance(0.01f);
        mCamera2->setFOVy(Ogre::Radian(Ogre::Degree(60)));
		//mCamera->setPosition(0,0,-1.5f);


        mViewport = mWindow->addViewport(mCamera);
		//mViewport->setBackgroundColour(Ogre::ColourValue(44.f/455.f,90.f/455.f,93.f/455.f));
		mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
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

void GfxMgr::setupViewp()
{
	mViewport2 = mWindow->addViewport(mCamera2, 2, 0.65, 0.0, 0.35, 0.35);
	mViewport2->setMaterialScheme("win");
}

void GfxMgr::deinit()
{
    if(mInitialized)
    {
		endState();
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
	for(int i = 0; i < mObjs.size(); ++i)
		delete mObjs[i];
	for(int i = 0; i < mGuis.size(); ++i)
		delete mGuis[i];
	mGuis.clear();
	mObjs.clear();
	delete mOyster;
	mOyster = new Oyster::Oyster(mWidth,mHeight);
	if(mViewport2)
		mWindow->removeViewport(2);
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

void GfxMgr::screenshot()
{
	++numShots;
	mWindow->writeContentsToFile("LD21_"+StringUtils::toString(numShots)+".png");
}

