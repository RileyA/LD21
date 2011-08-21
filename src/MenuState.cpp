#include "Game.hpp"
#include "MenuState.hpp"
#include "Oyster.h"
#include "Gfx/GfxMgr.hpp"
#include "Gfx/GfxObject.hpp"
#include "Audio/AudioMgr.hpp"
#include "FPSCam.hpp"
#include "Player.hpp"
#include "MapManager.hpp"
#include "TitleState.hpp"

MenuState::MenuState()
	:State()
{
	
}

MenuState::~MenuState()
{

}

void MenuState::init()
{
	mGfx = Game::getPtr()->getGfx();
	mAudio = Game::getPtr()->getAudio();
	mOyster = mGfx->getOyster();

	size_t handle = 0;
	mGfx->getWindow()->getCustomAttribute("WINDOW", &handle);

	mGame->getInput()->initInput(handle,true);
	//...
	mOyster->createAtlas("Test", "TechDemo.oyster");
	Oyster::Batch* b = mOyster->createBatch("Test", "Test");
	//b->createLayer(1)->createRectangle(0, 0, 50, 50)->setSprite("logo");
	dist = b->createLayer(1)->createText("Distance: Not Much", 10, 10, 500, 500);

	int w = mGfx->getWindow()->getWidth();
	int h = mGfx->getWindow()->getHeight();

	b->getLayer(1)->createRectangle((w-47)/2,(h-47)/2,47,47)->setSprite("reticle");
	mGfx->createGui(b);

	mm = new MapManager(mGfx->mCamera);

	player = new Player();

	mGfx->getSceneManager()->setFog(Ogre::FOG_EXP,Ogre::ColourValue(0,0,0),
		0.03f,15.f,75.f);
	maxDist = 0.f;

}

void MenuState::deinit()
{
	//...
}

void MenuState::update(Real delta)
{
	int d = -mGfx->mCamera->getDerivedPosition().z;
	if(d > maxDist)
	{
		maxDist = d;
		dist->setCaption("Distance: "+StringUtils::toString(maxDist) + "m");
	}
	
	if(fabs(mGfx->mCamera->getDerivedPosition().x) > 7.75f ||
		fabs(mGfx->mCamera->getDerivedPosition().y) > 7.75f ||
		mGfx->mCamera->getDerivedPosition().z > mm->destruction + 50)
	{
		mGame->addState(new TitleState());
		setDone(true);
	}

	//if(mTimeElapsed > 5.f)
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_ESCAPE"))
		setDone(true);

	
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_U"))
		mGfx->screenshot();
}
