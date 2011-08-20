#include "Game.hpp"
#include "MenuState.hpp"
#include "Oyster.h"
#include "Gfx/GfxMgr.hpp"
#include "Gfx/GfxObject.hpp"
#include "Audio/AudioMgr.hpp"
#include "FPSCam.hpp"

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
	b->createLayer(1)->createRectangle(0, 0, 50, 50)->setSprite("logo");
	b->getLayer(1)->createText("hello, world!", 100, 100, 500, 500);
	mGfx->createGui(b);

	GfxObject* g = new GfxObject("Cube.mesh");
	g->getNode()->setPosition(0,0,-50);

	new FPSCam(mGfx->mCamera);
}

void MenuState::deinit()
{
	//...
}

void MenuState::update(Real delta)
{
	//mTimeElapsed += delta;
	//std::cout<<mTimeElapsed<<"\n";

	//if(mTimeElapsed > 5.f)
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_ESCAPE"))
		setDone(true);
}
