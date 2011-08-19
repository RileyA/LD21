#include "Game.hpp"
#include "MenuState.hpp"
#include "Oyster.h"
#include "Gfx/GfxMgr.hpp"
#include "Audio/AudioMgr.hpp"

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
	//...
	mOyster->createAtlas("Test", "TechDemo.oyster");
	Oyster::Batch* b = mOyster->createBatch("Test", "Test");
	b->createLayer(1)->createRectangle(0, 0, 50, 50)->setSprite("logo");
	b->getLayer(1)->createText("hello, world!", 100, 100, 500, 500);
	mGfx->createGui(b);
}

void MenuState::deinit()
{
	//...
}

void MenuState::update(Real delta)
{
	mTimeElapsed += delta;
	std::cout<<mTimeElapsed<<"\n";

	if(mTimeElapsed > 5.f)
		setDone(true);
}
