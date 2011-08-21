#include "Player.hpp"
#include "Game.hpp"
#include "Chunk.hpp"

#define CROUCH_RATE 0.15f
#define ROLL_RATE 0.35f

Player::Player()
{
	mCap = Game::getPtr()->getPhysics()->createCapsule(0.4f, 1.f, Vector3(0,-2,-50));
	mCap2 = Game::getPtr()->getPhysics()->createCapsule(0.36f, 1.f, Vector3(0,0,0));
	mSph = Game::getPtr()->getPhysics()->createSphere(0.4f, Vector3(0,10000,0));

	mSph->setLinearFactor(Vector3(0,0,0));
	mSph->setAngularFactor(Vector3(0,0,0));
	mSph->setSleepingEnabled(false);

	mCap2->setLinearFactor(Vector3(0,0,0));
	mCap2->setAngularFactor(Vector3(0,0,0));
	mCap2->setPosition(Vector3(0,100,0));
	//mCap2->setSleepingEnabled(true);

	// this just gross, this is what sleep deprivation and changing plans does...

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
	backAngle = 0.f;
	userdata* dat = new userdata;
	dat->c = 0;
	dat->type = 50000;
	mSph->setUserData(dat);
	mCap->setUserData(dat);
	mCap2->setUserData(dat);

	minSpeed = 8.f;
	maxSpeed = 28.f;
	speed = 10.f;
	lastPos = Vector3(0,0,0);
	speedp = 4;
	speedprog = 0.f;
	slowtimer = 0.1f;
	spt = 3.f;
	cht = 2.f;
	cht2 = 1.5f;
	startTimer = 0.f;
	startSeq = true;
	camTurn = false;
	camReturn = false;
	camWait = false;
	wait = 2.8f;

	fov = mCam->mCamera->getFOVy();
}

Player::~Player()
{ 
	std::list<Shot*>::iterator it = mShots.begin();
	for(it; it != mShots.end(); ++it)
		delete *it;
	it = mSpareShots.begin();
	for(it; it != mSpareShots.end(); ++it)
		delete *it;
}

// here be dragons
void Player::update(Real delta)
{
	speedprog -= delta;
	slowprog -= delta;
	// wait a few frames so loading hiccups don't cause the controller to tunnel
	--skipFrame;
	if(skipFrame > 0)
	{
		mController->setPosition(Vector3(0,-1,-32));
		return;
	}

	if(startSeq)
	{
		Vector3 cpos = mController->getPosition();
		mCam->camPos->setPosition(cpos + Vector3(0,0.7f,0));

		if(!camTurn)
		{
			if(backAngle + delta*700.f >= 180.f)
			{
				Real a = 180.f - backAngle;
				mCam->camRoll->yaw(Ogre::Degree(a));
				backAngle = 180.f;
				camTurn = true;
			}
			else if(backAngle < 180.f)
			{
				backAngle += delta * 700;
				mCam->camRoll->yaw(Ogre::Degree(delta * 700));
				mCam->mCamera->setFOVy(Ogre::Degree(mCam->mCamera->getFOVy()) - Ogre::Degree(delta * 50));
			}
		}
		else if(!camWait)
		{
			wait -= delta;
			if(wait < 0.f)
			{
				camWait = true;
			}
		}
		else if(!camReturn)
		{
			if(backAngle - delta*450.f <= 0.f)
			{
				Real a = 180.f - backAngle;
				mCam->camRoll->yaw(-Ogre::Degree(backAngle));
				mCam->camRoll->setOrientation(Ogre::Quaternion::IDENTITY);
				backAngle = 0.f;
				camReturn = true;
				startSeq = false;
				mCam->mCamera->setFOVy(fov);
				mGame->getGfx()->setupViewp();
			}
			else if(backAngle > 0.f)
			{
				backAngle -= delta * 450;
				mCam->camRoll->yaw(-Ogre::Degree(delta * 450));
				mCam->mCamera->setFOVy(Ogre::Degree(mCam->mCamera->getFOVy()) + Ogre::Degree(delta * 50));
			}
		}
		return;
	}

	jumpFactor -= delta * 29;
	if(jumpFactor < 0.f)
		jumpFactor = 0.f;

	Real spd = fabs(lastPos.z - mController->getPosition().z) / delta;

	Vector3 norm = mCam->camPos->_getDerivedOrientation() * Vector3(0,-1,0);
	RaycastReport rr = Game::getPtr()->getPhysics()->raycastSimple(mController->getPosition()
		,norm, 0.93f, 0, COLLISION_GROUP_2);
	
	onGround = rr.hit && norm.angleBetween(-rr.normal) < Ogre::Radian(Ogre::Degree(2.f));

	if(spd < minSpeed)
	{
		slowtimer -= delta;
		if(slowtimer < 0.f&& speedp > 0)
		{
			--speedp;
			slowtimer = 0.1f;
			Game::getPtr()->getAudio()->play2D("media/audio/crash.wav");
			spt = 2.f;
		}
	}
	else
	{
		slowtimer += delta;
		if(slowtimer > 0.1f)
			slowtimer = 0.1f;
	}

	if(!crouched)
		spt -= delta;
	else
	{
		spt += delta / 3;
		if(spt > 3.2f)
			spt = 3.2f;
	}

	if(spt < 0.f && !crouched)
	{
		spt = 3.f;
		Game::getPtr()->getAudio()->play2D("media/audio/upspeed.wav");
		++speedp;
	}

	if(crouched)
		cht -= delta;
	else
		cht += delta;

	if(crouched && cht <= 0.f)
	{
		cht = 3.f;
		--speedp;
		Game::getPtr()->getAudio()->play2D("media/audio/lowerspeed.wav");
	}
	if(cht > 3.f)
	{
		cht = 3.f;
	}

	//std::cout<<spd<<"\n";
	//speed = spd + 0.2f;
	//

	//if(speed > spd + 4.f)
	//	speed = speed - 3.f;
	//

	// we you crash, you have to start over...
		
	lastPos = mController->getPosition();

	speed = minSpeed + 0.8f + speedp * 1.f;

	speedp = std::min(speedp, 20);

	//std::cout<<speedp<<"\n";

	if(speed < minSpeed)
		speed = minSpeed;

	if(speed > maxSpeed)
		speed = maxSpeed;

	Vector3 v = (mGame->getGfx()->mCamera->getParentSceneNode()->getParentSceneNode()->_getDerivedOrientation()) * Vector3::NEGATIVE_UNIT_Z;

	Ogre::Plane pln(mCam->camPos->_getDerivedOrientation() * Vector3::UNIT_Y, 0);	
	v = pln.projectVector(v);
	v.normalise();
	v *= speed;// * mGame->getInput()->isKeyDown("KC_UP");
	Vector3 gravity = mCam->camPos->_getDerivedOrientation() * Vector3::NEGATIVE_UNIT_Y;
	gravity.normalise();
	if(onGround)
		gravity *= 0.1f;
	if(rotating)
		gravity = Vector3::ZERO;
	v += gravity * (5-jumpFactor) * 3;
	mController->setVelocity(v);
	if(mGame->getPtr()->getInput()->wasKeyPressed("KC_P"))
		mController->setPosition(Vector3(0,0,0));


	if(onGround)
		speed += delta;

	if(rr.hit)
	{
		userdata* d = static_cast<userdata*>(rr.userData);
		if(d)
		{
			if(d->c)
			{
				int floor = d->c->getBlock(rr.hitP, rr.normal);
				if(floor == 4)
				{
					if(lastFloor != 4 && speedprog <= 0.f)
					{
						++speedp;
						speedprog = 0.3f;
						Game::getPtr()->getAudio()->play2D("media/audio/upspeed.wav");
					}
				}
				if(floor == 5)
				{
					if(lastFloor != 5 && speedprog <= 0.f)
					{
						--speedp;
						slowprog = 0.3f;
						Game::getPtr()->getAudio()->play2D("media/audio/lowerspeed.wav");
					}
				}
				lastFloor = floor;
			}
			else if(d->type)
			{
				// maybe kill it...
			}
		}
	}

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
		if(delta*260 >= rotation)
		{
			mCam->camPos->roll(Ogre::Degree(rotation * (-1 + leftrot*2)));
			rotation = 0.f;
			rotating = false;
		}
		else
		{
			rotation -= delta*260;
			mCam->camPos->roll(Ogre::Degree(delta*260 * (-1 + leftrot*2)));
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

	Vector3 cpos = mController->getPosition();

	mCam->camPos->setPosition(cpos);
	mCam->camPos2->setPosition(verticalOffset);

	if(mGame->getPtr()->getInput()->isButtonDown("MB_Left") && onGround && jumpFactor < 5.f)
		jumpFactor = 10.45f;

	if(mGame->getPtr()->getInput()->isKeyDown("KC_A") && !rotating)
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
		jumpFactor += 6.f;
	}
	else if(mGame->getPtr()->getInput()->isKeyDown("KC_D") && !rotating)
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
		jumpFactor += 6.f;
	}

	if(!rotating && !crouching && !crouched && mGame->getPtr()->getInput()->isButtonDown("MB_Right"))
	{
		crouching = true;
		crouchTime = 0.1f;
	}

	if(!rotating && !crouching && crouched && !mGame->getPtr()->getInput()->isButtonDown("MB_Right"))
	{
		crouching = true;
		crouchTime = 0.1f;
	}

	

	//if(mGame->getPtr()->getInput()->wasKeyPressed("KC_W"))
	//	shoot();

	updateShots(delta);

}

void Player::shoot()
{
	Shot* s = 0;

	if(!mSpareShots.empty())
	{
		s = mSpareShots.front();
		mSpareShots.pop_front();
	}
	else
	{
		s = new Shot();
		s->node = mGame->getGfx()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		s->entity = mGame->getGfx()->getSceneManager()->createEntity("Debris8.mesh");
		s->node->attachObject(s->entity);
	}

	mShots.push_back(s);
	s->node->setVisible(true);
	s->node->setPosition(mCam->mCamera->getDerivedPosition() + mCam->mCamera->getDerivedDirection());
	s->direction = mCam->mCamera->getDerivedDirection();
	s->done = false;
	s->timeout = 3.f;
	s->node->setScale(0.1f, 0.1f, 0.1f);
}

void Player::updateShots(Real delta)
{
	std::list<Shot*>::iterator it = mShots.begin();
	for(it; it != mShots.end(); ++it)
	{
		Shot* s = (*it);

		(*it)->timeout -= delta;


		if((*it)->done)
		{
			s->node->setScale((s->timeout*5.f) * Vector3(0.1f, 0.1f, 0.1f));
		}

		if((*it)->timeout < 0.f)
		{
			it = mShots.erase(it);
			mSpareShots.push_back(s);
			s->node->setVisible(false);
		}
		else if(!s->done)
		{
			// raycast
			RaycastReport rr = mGame->getPhysics()->raycastSimple((*it)->node->getPosition(), 
				(*it)->direction, 30 * delta, 0, COLLISION_GROUP_2);

			// if we hit, do somethin'
			if(rr.hit)
			{
				s->done = true;
				s->timeout = 0.6f;
				s->node->setPosition(rr.hitP);
				userdata* d = static_cast<userdata*>(rr.userData);

				if(d)
				{
					if(d->c)
						d->c->shootBlock(rr.hitP, rr.normal);
				}

				// get what we hit...
				//if(rr.)
				//{
					
				//}

				// change color or something
			}
			else
			{
				s->node->translate(s->direction * delta * 30);
			}
		}
	}
}

