#include "FPSCam.hpp"
#include "Game.hpp"

FPSCam::FPSCam(Ogre::Camera* c)
{
	mSmgr = Game::getPtr()->getGfx()->getSceneManager();
	mCamera = c;

	camRoll = mSmgr->createSceneNode();
	camYaw = mSmgr->createSceneNode();
	camPitch = mSmgr->createSceneNode();
	camPos = mSmgr->createSceneNode();
	camPos2 = mSmgr->createSceneNode();

	mSmgr->getRootSceneNode()->addChild(camPos);
	camPos->addChild(camPos2);
	camPos2->addChild(camYaw);
	camPos2->attachObject(mGame->getGfx()->mCamera2);
	camYaw->addChild(camPitch);
	camPitch->addChild(camRoll);
	camRoll->attachObject(mCamera);
	
	pichLimit = 80.f;
	pitch = 0.f;
	cameraMotion = false;
	invert = false;

	mGame->getInput()->getSignal("mouseMoved")->addListener(
		createSlot("look", this, &FPSCam::look));
}

FPSCam::~FPSCam()
{
	camPos->removeAllChildren();
	camPos2->removeAllChildren();
	camRoll->removeAllChildren();
	camPitch->removeAllChildren();
	camYaw->removeAllChildren();
	mSmgr->destroySceneNode(camPos);
	mSmgr->destroySceneNode(camPos2);
	mSmgr->destroySceneNode(camRoll);
	mSmgr->destroySceneNode(camPitch);
	mSmgr->destroySceneNode(camYaw);
}

void FPSCam::update(Real delta)
{ 
	Real speed = 10.f;

	/*camPos->setPosition(
		camPos->getPosition() + 
		((mGame->getInput()->isKeyDown("KC_W") - 
			mGame->getInput()->isKeyDown("KC_S")) *
				mCamera->getDerivedDirection() * speed * delta) + 
		((mGame->getInput()->isKeyDown("KC_D") - 
			mGame->getInput()->isKeyDown("KC_A")) * 
				mCamera->getDerivedRight() * speed * delta)
	);*/
}

void FPSCam::look(const Message& m)
{	
	const Vector2* v = unpackMsg<Vector2>(m);
	if(v->x * -0.35f > 5.f)
		camYaw->yaw(Ogre::Radian(Ogre::Degree(5.f)));
	else if(v->x * -0.35f < -5.f)
		camYaw->yaw(Ogre::Radian(Ogre::Degree(-5.f)));
	else
		camYaw->yaw(Ogre::Radian(Ogre::Degree(v->x*-0.35f)));

	Real tryPitch = v->y*-0.35f * (1 - 2 * invert);

	if(tryPitch > 5.f)
		tryPitch = 5.f;
	else if(tryPitch < -5.f)
		tryPitch = -5.f;

	Real actualPitch = tryPitch;

	if(pitch + tryPitch > 80.f)
		actualPitch = 80.f - pitch;
	else if(pitch + tryPitch < -80.f)
		actualPitch = -80.f - pitch;
	pitch += actualPitch;
	camPitch->pitch(Ogre::Radian(Ogre::Degree(actualPitch)));
}
