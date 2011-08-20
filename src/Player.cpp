#include "Player.hpp"
#include "Game.hpp"

Player::Player()
{
	mController = Game::getPtr()->getPhysics()->createCapsule(0.4f, 1.f, Vector3(0,0,0));
	mCam = new FPSCam(Game::getPtr()->getGfx()->mCamera);
	mController->setAngularFactor(Vector3(0.f,0.f,0.f));
	mController->setSleepingEnabled(false);
}

Player::~Player()
{

}

void Player::update(Real delta)
{
	mCam->camPos->setPosition(mController->getPosition() + Vector3(0,0.7f,0));

	Real speed = 30.f;
	Vector3 v = ((mGame->getInput()->isKeyDown("KC_W") - 
		mGame->getInput()->isKeyDown("KC_S")) *
			mGame->getGfx()->mCamera->getDerivedDirection()) + 
	((mGame->getInput()->isKeyDown("KC_D") - 
		mGame->getInput()->isKeyDown("KC_A")) * 
			mGame->getGfx()->mCamera->getDerivedRight());
	v.y = 0;
	v.normalise();
	v *= speed;
	v -= Vector3(0,5,0);
	mController->setVelocity(v);
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_P"))
		mController->setPosition(Vector3(0,0,0));
}
