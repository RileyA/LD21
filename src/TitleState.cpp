#include "Game.hpp"
#include "TitleState.hpp"
#include "MenuState.hpp"
#include "Oyster.h"
#include "Gfx/GfxMgr.hpp"
#include "Gfx/GfxObject.hpp"
#include "Audio/AudioMgr.hpp"
#include "FPSCam.hpp"
#include "Player.hpp"
#include "MapManager.hpp"

TitleState::TitleState()
	:State()
{
	
}

TitleState::~TitleState()
{

}

void TitleState::init()
{
	mGfx = Game::getPtr()->getGfx();
	mAudio = Game::getPtr()->getAudio();
	mOyster = mGfx->getOyster();

	size_t handle = 0;
	mGfx->getWindow()->getCustomAttribute("WINDOW", &handle);

	int w = mGfx->getWindow()->getWidth();
	int h = mGfx->getWindow()->getHeight();

	// offset and center
	int hh = (h - 600)/2;
	h = 600 + hh;

	mGame->getInput()->initInput(handle,true);
	//...
	mOyster->createAtlas("Test", "TechDemo.oyster");
	Oyster::Batch* b = mOyster->createBatch("Test", "Test");
	//b->createLayer(1)->createRectangle(0, 0, 50, 50)->setSprite("logo");

	Oyster::Rectangle* r = b->createLayer(1)->createRectangle((w - 500)/2, hh+20, 500, 270);

	r->setSprite("logo");

	Oyster::Text* t1 = b->getLayer(1)->createText("Press ENTER to Play!", 0, hh + 260, w, 100);
	t1->setAlign(Oyster::HA_CENTER);

	t1 = b->getLayer(1)->createText("Press ESCAPE to Quit.", 0, h - 52, w, 100);
	t1->setAlign(Oyster::HA_CENTER);

	t1 = b->getLayer(1)->createText("Scores:", 0, hh + 300, w, 100);
	t1->setAlign(Oyster::HA_CENTER);
	t1->setColor(150,210,255, 255);

	for(int i = 0; i < 5; ++i)
	{
		t1 = b->getLayer(1)->createText(StringUtils::toString(i + 1) + ": " 
			+ StringUtils::toString((5-i) * 1432), (w - 300)/2 + 82, hh + 342 + i * 35, w, 100);
		t1->setAlign(Oyster::HA_LEFT);
		t1->setColor(150,210,255, 255);
	}

	r = b->getLayer(1)->createRectangle((w - 300)/2, hh+332, 300,1, 255, 255, 255);
	r->setColor(150,210,255, 255);
	r = b->getLayer(1)->createRectangle((w - 300)/2, hh+295, 300, 2, 255, 255, 255);
	r->setColor(150,210,255, 255);
	r = b->getLayer(1)->createRectangle((w - 300)/2, h - 70, 300, 2, 255, 255, 255);
	r->setColor(150,210,255, 255);

	mGfx->createGui(b);
}

void TitleState::deinit()
{
	//...
}

void TitleState::update(Real delta)
{
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_RETURN"))
	{
		mGame->addState(new MenuState());
		setDone(true);
	}

	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_ESCAPE"))
		setDone(true);
}
