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

int lastScore = 0;

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
	//b->createLayer(1)->createRectangle(0, 0, 50, 50)->setSprite("logo");

	int w = mGfx->getWindow()->getWidth();
	int h = mGfx->getWindow()->getHeight();

	mOyster->createAtlas("Test", "TechDemo.oyster");
	Oyster::Batch* b = mOyster->createBatch("Test", "Test");
	dist = b->createLayer(1)->createText("Distance: Not Much", 10, 10, 500, 500);
	//b->createLayer(1)->createRectangle((w-47)/2,(h-47)/2,47,47)->setSprite("reticle");
	b->getLayer(1)->createRectangle((w-47)/2,(h-47)/2,47,47)->setSprite("reticle");
	mGfx->createGui(b);

	mOyster->createAtlas("Test2", "TechDemo.oyster");
	b = mOyster->createBatch("Test2", "Test");
	mGfx->createGui(b);
	b->createLayer(1);

	for(int i = 0; i < 20; ++i)
	{
		rects[i] = b->getLayer(1)->createRectangle((w-(20*12 + 19*6))/2 + i*18, h - 42, 12, 25, 50,50,50);
	}

	player = new Player();

	for(int i = 0; i < 20; ++i)
	{
		if(player->speedp >= i)
			rects[i]->setColor(60,255,120, 150);
		else
			rects[i]->setColor(100,100,100,150);
	}


	mm = new MapManager(mGfx->mCamera);


	mGfx->getSceneManager()->setFog(Ogre::FOG_EXP,Ogre::ColourValue(0,0,0),
		0.03f,15.f,75.f);
	maxDist = 0.f;

	text = true;

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
		dist->setColor(255,255,255,255);
	}
	for(int i = 0; i < 20; ++i)
	{
		if(player->speedp >= i)
			rects[i]->setColor(60,255,120, 150);
		else
			rects[i]->setColor(100,100,100,150);
	}

	text = !text;
	
	if(fabs(mGfx->mCamera->getDerivedPosition().x) > 7.75f ||
		fabs(mGfx->mCamera->getDerivedPosition().y) > 7.75f ||
		mGfx->mCamera->getDerivedPosition().z > mm->destruction + 50)
	{
		Game::getPtr()->getAudio()->play2D("media/audio/fail.wav");
		lastScore = maxDist;
		mGame->addState(new TitleState());
		setDone(true);
	}


	//if(mTimeElapsed > 5.f)
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_ESCAPE"))
		setDone(true);

	
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_U"))
		mGfx->screenshot();
}
