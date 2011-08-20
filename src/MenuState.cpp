#include "Game.hpp"
#include "MenuState.hpp"
#include "Oyster.h"
#include "Gfx/GfxMgr.hpp"
#include "Gfx/GfxObject.hpp"
#include "Audio/AudioMgr.hpp"
#include "FPSCam.hpp"
#include "MapManager.hpp"

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
	b->createLayer(1)->createText("blah", 10, 10, 500, 500);
	mGfx->createGui(b);

	fps = new FPSCam(mGfx->mCamera);
	new MapManager(mGfx->mCamera);

	ob = mGame->getPhysics()->createCapsule(0.4f, 1.f, Vector3(0,0,0));
	ob->setAngularFactor(Vector3(0.f,0.f,0.f));
	ob->setSleepingEnabled(false);
}

void MenuState::deinit()
{
	//...
}

void MenuState::update(Real delta)
{
	//mTimeElapsed += delta;
	//std::cout<<mTimeElapsed<<"\n";
		
	//RaycastReport rr;
	//rr = mGame->getPhysics()->raycastSimple(mGfx->mCamera->getDerivedPosition(), mGfx->mCamera->getDerivedDirection(),
	//	10000, 0, 0);
	//if(rr.hit)
	//	std::cout<<rr.hitP.x<<" "<<rr.hitP.y<<" "<<rr.hitP.z<<"\n";

	//std::cout<<ob->getPosition().y<<"\n";
	fps->camPos->setPosition(ob->getPosition()+Vector3(0,0.7f,0));


	Real speed = 8.f;
	Vector3 v = ((mGame->getInput()->isKeyDown("KC_W") - 
		mGame->getInput()->isKeyDown("KC_S")) *
			mGfx->mCamera->getDerivedDirection()) + 
	((mGame->getInput()->isKeyDown("KC_D") - 
		mGame->getInput()->isKeyDown("KC_A")) * 
			mGfx->mCamera->getDerivedRight());
	v.y = 0;
	v.normalise();
	v *= speed;
	ob->setVelocity(v);

	//if(mTimeElapsed > 5.f)
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_ESCAPE"))
		setDone(true);
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_P"))
		ob->setPosition(Vector3(0,0,0));
}
