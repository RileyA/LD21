#include "Player.hpp"
#include "Game.hpp"

#define CROUCH_RATE 0.15f
#define ROLL_RATE 0.35f

Player::Player()
{
	mCap = Game::getPtr()->getPhysics()->createCapsule(0.4f, 1.f, Vector3(0,0,0));
	mCap2 = Game::getPtr()->getPhysics()->createCapsule(0.36f, 1.f, Vector3(0,0,0));
	mSph = Game::getPtr()->getPhysics()->createSphere(0.4f, Vector3(0,10000,0));

	mSph->setLinearFactor(Vector3(0,0,0));
	mSph->setAngularFactor(Vector3(0,0,0));
	mSph->setSleepingEnabled(false);

	mCap2->setLinearFactor(Vector3(0,0,0));
	mCap2->setAngularFactor(Vector3(0,0,0));
	mCap2->setPosition(Vector3(0,100,0));
	//mCap2->setSleepingEnabled(true);

	mController = mCap;
	mCam = new FPSCam(Game::getPtr()->getGfx()->mCamera);
	mController->setAngularFactor(Vector3(0.f,0.f,0.f));
	mController->setSleepingEnabled(false);
	crouched = false;
	crouching = false;
	crouchTime = 0.f;
	jumpFactor = 0.f;
	offset = Vector3(0,0,0);
	rotating = false;
	rotation = 0.f;
	leftrot = false;
	skipFrame = 5;
}

Player::~Player()
{ 

}

// here be dragons
void Player::update(Real delta)
{
	// wait a few frames so loading hiccups don't cause the controller to tunnel
	--skipFrame;
	if(skipFrame > 0)
	{
		mController->setPosition(Vector3(0,0,0));
		return;
	}
	jumpFactor -= delta * 29;
	if(jumpFactor < 0.f)
		jumpFactor = 0.f;

	Real speed = 10.f;
	Vector3 v = (mGame->getGfx()->mCamera->getDerivedDirection());

	Ogre::Plane pln(mCam->camPos->_getDerivedOrientation() * Vector3::UNIT_Y, 0);	
	v = pln.projectVector(v);
	v.normalise();
	v *= speed;// * mGame->getInput()->isKeyDown("KC_Q");
	Vector3 gravity = mCam->camPos->_getDerivedOrientation() * Vector3::NEGATIVE_UNIT_Y;
	gravity.normalise();
	v += gravity * (5-jumpFactor);
	mController->setVelocity(v);
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_P"))
		mController->setPosition(Vector3(0,0,0));

	Vector3 norm = mCam->camPos->_getDerivedOrientation() * Vector3(0,-1,0);
	RaycastReport rr = Game::getPtr()->getPhysics()->raycastSimple(mController->getPosition()
		,norm, 0.93f, 0, COLLISION_GROUP_2);
	
	onGround = rr.hit && norm.angleBetween(-rr.normal) < Ogre::Radian(Ogre::Degree(2.f));

	crouchTime -= delta;
	if(crouchTime < 0.f)
		crouchTime = 0.f;

	// crouch is a funny word... and I've typed it so many times it's lost all meaning...
	if(crouchTime <= 0.f && crouching && !rotating)// make sure shape state doesn't change suring rotation
	{
		crouchTime = 0.f;
		// swap out physics object here...
		Vector3 pos = mController->getPosition();

		if(!crouched)
		{
			//it can always shrink
			crouching = false;
			crouched = !crouched;
			if(mController != mSph)
			{
				mController->setPosition(Vector3(0,0,0));
				mController->setLinearFactor(Vector3(0,0,0));
				mController = mSph;
				mController->setPosition(pos + mCam->camPos->_getDerivedOrientation() * (Vector3(0,-0.5f,0) + offset));
				offset = Vector3(0,0.5,0);
				mController->setLinearFactor(Vector3(1,1,1));
			}	

		}
		else
		{
			// gotta make sure there's room for the cap
			if(!mGame->getPhysics()->overlap(mCap2, pos + mCam->camPos->_getDerivedOrientation() * offset, mCam->camPos->_getDerivedOrientation()))
			{
				crouching = false;
				mController->setPosition(Vector3(0,0,0));
				mController->setLinearFactor(Vector3(0,0,0));
				mController = mCap;
				mController->setOrientation(mCam->camPos->_getDerivedOrientation());
				crouched = !crouched;
				mController->setPosition(pos + mCam->camPos->_getDerivedOrientation() * offset);
				offset = Vector3(0,0,0);
				mController->setLinearFactor(Vector3(1,1,1));
			}
		}
	}

	if(!crouching && !crouched && !rotating && mController == mSph)
	{
		Vector3 pos = mController->getPosition();
		if(!mGame->getPhysics()->overlap(mCap2, pos + mCam->camPos->_getDerivedOrientation() * offset, mCam->camPos->_getDerivedOrientation()))
		{
			crouching = false;
			mController->setPosition(Vector3(0,0,0));
			mController = mCap;
			mController->setOrientation(mCam->camPos->_getDerivedOrientation());
			mController->setLinearFactor(Vector3(0,0,0));
			crouched = false;
			mController->setPosition(pos + mCam->camPos->_getDerivedOrientation() * offset);
			offset = Vector3(0,0,0);
			mController->setLinearFactor(Vector3(1,1,1));
		}
	}

	Vector3 verticalOffset = Vector3(0,0,0);

	if(!crouching)
	{
		if(mController == mCap)
			verticalOffset = Vector3(0,0.7f,0);
		else if(mController == mSph && !crouched)
			verticalOffset = Vector3(0,1.2f,0);
		else
			verticalOffset = Vector3(0,0.3f,0);
	}
	else
	{
		float interp = crouchTime/0.1f;
		interp = std::max(0.f, interp);
		interp = std::min(1.f, interp);

		Vector3 start;
		Vector3 end;

		if(!crouched && mController == mSph)
		{
			start = Vector3(0,1.2f,0);	
			end = Vector3(0,0.3f,0);	
		}
		else
		{
			start = mController != mCap ? Vector3(0,0.3f,0) : Vector3(0,0.7f,0);
			end = mController != mCap ? Vector3(0,1.2f,0) : Vector3(0,-0.2f,0);
		}

		Vector3 pos = start * interp + end * (1-interp);
		verticalOffset = pos;
	}

	if(rotating)
	{
		// advance cam animation
		if(delta*180 >= rotation)
		{
			mCam->camPos->roll(Ogre::Degree(rotation * (-1 + leftrot*2)));
			rotation = 0.f;
			rotating = false;
		}
		else
		{
			rotation -= delta*180;
			mCam->camPos->roll(Ogre::Degree(delta*180 * (-1 + leftrot*2)));
		}
	}

	if((crouching && crouched) || rotating)
	{
		// raycast to ensure the camera stays outta the geometry
		Vector3 d = verticalOffset;
		d = mCam->camPos->_getDerivedOrientation() * d;
		Real len = d.normalise();
		rr = mGame->getPhysics()->raycastSimple(mController->getPosition(), d, len, 0, 
			COLLISION_GROUP_2);
		if(rr.hit)
		{
			Vector3 delt = rr.hitP - mController->getPosition();
			verticalOffset.normalise();
			verticalOffset *= delt.length() * 0.9f;
			Real dist = delt.length() * 0.9f;
			if(crouching && !rotating)
				crouchTime = dist * 0.1f;
		}
	}

	mCam->camPos->setPosition(mController->getPosition());
	mCam->camPos2->setPosition(verticalOffset);

	if(mGame->getPtr()->getInput()->isKeyDown("KC_W") && onGround && jumpFactor < 5.f)
		jumpFactor = 14.f;

	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_A") && !rotating)
	{
		//mCam->camPos->roll(Ogre::Degree(90.f));
		Vector3 pos = mController->getPosition();
		leftrot = false;
		rotating = true;
		rotation = 90.f;
		mController->setPosition(Vector3(0,0,0));
		mController->setLinearFactor(Vector3(0,0,0));
		mController = mSph;
		mController->setPosition(pos + mCam->camPos->_getDerivedOrientation() * (Vector3(0,-0.5f,0) + offset));
		offset = Vector3(0,0.5,0);
		mController->setLinearFactor(Vector3(1,1,1));
	}
	else if(mGame->getPtr()->getInput()->wasKeyPressed("KC_D") && !rotating)
	{
		Vector3 pos = mController->getPosition();
		leftrot = true;
		rotating = true;
		rotation = 90.f;
		mController->setPosition(Vector3(0,0,0));
		mController->setLinearFactor(Vector3(0,0,0));
		mController = mSph;
		mController->setPosition(pos + mCam->camPos->_getDerivedOrientation() * (Vector3(0,-0.5f,0) + offset));
		offset = Vector3(0,0.5,0);
		mController->setLinearFactor(Vector3(1,1,1));
	}

	if(!crouching && mGame->getPtr()->getInput()->wasKeyPressed("KC_S"))
	{
		crouching = true;
		crouchTime = 0.1f;
	}
 
}
