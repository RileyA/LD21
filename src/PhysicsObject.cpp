#include "StdHeaders.hpp"
#include "PhysicsObject.hpp"

//-----------------------------------------------------------------------
PhysicsObject::PhysicsObject(btRigidBody* actor,btDynamicsWorld* dynWorld)
    :mReadyForDelete(0)
{
	mDynamicsWorld = dynWorld;
	mActor = actor;
	mRigidBody = true;
	mPos[0] = Ogre::Vector3(mActor->getWorldTransform().getOrigin().x(),
			mActor->getWorldTransform().getOrigin().y(),
			mActor->getWorldTransform().getOrigin().z());
	mPos[1] = mPos[0];
	mOrient[0] = Ogre::Quaternion(1,0,0,0);
	mOrient[1] = Ogre::Quaternion(1,0,0,0);
	gravLast = Ogre::Vector3(0,0,0);
}

//-----------------------------------------------------------------------
PhysicsObject::PhysicsObject(btCollisionObject* actor,btDynamicsWorld* dynWorld)
    :mReadyForDelete(0)
{
	mActor = 0;
	mDynamicsWorld = dynWorld;
	mActor = actor;
	mRigidBody = false;
	mPos[0] = Ogre::Vector3(mActor->getWorldTransform().getOrigin().x(),
			mActor->getWorldTransform().getOrigin().y(),
			mActor->getWorldTransform().getOrigin().z());
	mPos[1] = mPos[0];
	mOrient[0] = Ogre::Quaternion(1,0,0,0);
	mOrient[1] = Ogre::Quaternion(1,0,0,0);
}

//-----------------------------------------------------------------------
PhysicsObject::~PhysicsObject()
{
	if(mRigidBody)
		mDynamicsWorld->removeRigidBody(dynamic_cast<btRigidBody*>(mActor));
	else
		mDynamicsWorld->removeCollisionObject(mActor);
	mActor = 0;
}

//-----------------------------------------------------------------------
btRigidBody* PhysicsObject::getActor()
{
	return dynamic_cast<btRigidBody*>(mActor);
}

//-----------------------------------------------------------------------
void PhysicsObject::setPosition(Ogre::Vector3 pos)
{
	if(mRigidBody)
	{
		btTransform trans = dynamic_cast<btRigidBody*>(mActor)->getCenterOfMassTransform();
		trans.setOrigin(btVector3(pos.x,pos.y,pos.z));
		dynamic_cast<btRigidBody*>(mActor)->setCenterOfMassTransform(trans);
	}
	else
	{
		btTransform trans = mActor->getWorldTransform();
		trans.setOrigin(btVector3(pos.x,pos.y,pos.z));
		mActor->setWorldTransform(trans);
	}
	mPos[0] = pos;
	mPos[1] = pos;
}

//-----------------------------------------------------------------------
void PhysicsObject::setOrientation(Ogre::Quaternion ori)
{
	if(mRigidBody)
	{
		btTransform trans = dynamic_cast<btRigidBody*>(mActor)->getCenterOfMassTransform();
		trans.setRotation(btQuaternion(ori.x,ori.y,ori.z,ori.w));
		dynamic_cast<btRigidBody*>(mActor)->setCenterOfMassTransform(trans);
	}
	else
	{
		btTransform trans = mActor->getWorldTransform();
		trans.setRotation(btQuaternion(ori.x,ori.y,ori.z,ori.w));
		mActor->setWorldTransform(trans);
	}
}

//-----------------------------------------------------------------------
Ogre::Vector3 PhysicsObject::getPosition()
{
	// pos[0] is the 'current' pos, [1] is the previous
	return mPos[0]*mInterpolation+mPos[1]*(1-mInterpolation);
}

//-----------------------------------------------------------------------
Ogre::Quaternion PhysicsObject::getOrientation()
{
	// TOBEFIXED add interpolation here (once slerp is implemented?)
	return mOrient[0];
}

//-----------------------------------------------------------------------
void PhysicsObject::addForce(Ogre::Vector3 force,Ogre::Vector3 pos)
{
	dynamic_cast<btRigidBody*>(mActor)->applyForce(btVector3(force.x,force.y,force.z),btVector3(pos.x,pos.y,pos.z));
}

//-----------------------------------------------------------------------
void PhysicsObject::addImpulse(Ogre::Vector3 impulse,Ogre::Vector3 pos)
{
	dynamic_cast<btRigidBody*>(mActor)->applyImpulse(btVector3(impulse.x,impulse.y,impulse.z),btVector3(pos.x,pos.y,pos.z));
}

//-----------------------------------------------------------------------
void PhysicsObject::setVelocity(Ogre::Vector3 velocity)
{
	dynamic_cast<btRigidBody*>(mActor)->setLinearVelocity(btVector3(velocity.x,velocity.y,velocity.z));
}

//-----------------------------------------------------------------------
Ogre::Vector3 PhysicsObject::getVelocity()
{
	return Ogre::Vector3(dynamic_cast<btRigidBody*>(mActor)->getLinearVelocity().x(),
		dynamic_cast<btRigidBody*>(mActor)->getLinearVelocity().y(),
		dynamic_cast<btRigidBody*>(mActor)->getLinearVelocity().z());
}

//-----------------------------------------------------------------------
void PhysicsObject::setMass(float mass)
{
	if(mRigidBody)
	{
		// we have to remove it from the world and add it back:
		mDynamicsWorld->removeRigidBody(dynamic_cast<btRigidBody*>(mActor));
		btVector3 localInertia(0,0,0);
		mActor->getCollisionShape()->calculateLocalInertia(10.f,localInertia);
		dynamic_cast<btRigidBody*>(mActor)->setMassProps(10.f,localInertia);
		mDynamicsWorld->addRigidBody(dynamic_cast<btRigidBody*>(mActor));
	}
}

//-----------------------------------------------------------------------
void PhysicsObject::wakeup()
{
	mActor->setActivationState(ACTIVE_TAG);
}

//-----------------------------------------------------------------------
void PhysicsObject::toSleep()
{
	mActor->setActivationState(ISLAND_SLEEPING);
}

//-----------------------------------------------------------------------
void PhysicsObject::setSleepingEnabled(bool enabled)
{
	if(enabled)
	{
		// only set it is its disabled already
		if(mActor->getActivationState()==DISABLE_DEACTIVATION)
		{
			mActor->setActivationState(ACTIVE_TAG);
		}
	}
	else
	{
		mActor->setActivationState(DISABLE_DEACTIVATION);
	}
}

//-----------------------------------------------------------------------
void PhysicsObject::setLinearFactor(Ogre::Vector3 factor)
{
	dynamic_cast<btRigidBody*>(mActor)->setLinearFactor(btVector3(factor.x,factor.y,factor.z));
}

//-----------------------------------------------------------------------
void PhysicsObject::setAngularFactor(Ogre::Vector3 factor)
{
	dynamic_cast<btRigidBody*>(mActor)->setAngularFactor(btVector3(factor.x,factor.y,factor.z));
}

//-----------------------------------------------------------------------
void PhysicsObject::setCollisionGroup(short group)
{
	mActor->getBroadphaseHandle()->m_collisionFilterGroup = group;
}

//-----------------------------------------------------------------------
void PhysicsObject::setCollisionMask(short mask)
{
	mActor->getBroadphaseHandle()->m_collisionFilterMask = mask;
}

//-----------------------------------------------------------------------
void PhysicsObject::setKinematic(bool kinematic)
{
	if(mRigidBody)
	{
		if(kinematic)
		{
			mDynamicsWorld->removeRigidBody(dynamic_cast<btRigidBody*>(mActor));
			dynamic_cast<btRigidBody*>(mActor)->setMassProps(0,btVector3(0,0,0));
			mActor->setCollisionFlags(mActor->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			mDynamicsWorld->addRigidBody(dynamic_cast<btRigidBody*>(mActor));
		}
		else
		{
			mDynamicsWorld->removeRigidBody(dynamic_cast<btRigidBody*>(mActor));
			btVector3 localInertia(0,0,0);
			dynamic_cast<btRigidBody*>(mActor)->getCollisionShape()->calculateLocalInertia(10.f,localInertia);
			dynamic_cast<btRigidBody*>(mActor)->setMassProps(10.f,localInertia);
			mActor->setCollisionFlags(mActor->getCollisionFlags() ^ btCollisionObject::CF_KINEMATIC_OBJECT);
			mDynamicsWorld->addRigidBody(dynamic_cast<btRigidBody*>(mActor));
		}
	}
}

//-----------------------------------------------------------------------
void PhysicsObject::update(bool frame,float interpolation,Ogre::Vector3 gravity)
{
	mInterpolation = interpolation;
	if(mRigidBody)
	{
		if(frame&&!mActor->isStaticObject())
		{
			// only set if gravity has changed (since it may keep the actor awake)
			if(gravLast!=gravity)
			{
				mActor->activate(true);
				dynamic_cast<btRigidBody*>(mActor)->setGravity(btVector3(gravity.x*1.75f,gravity.y*1.75f,gravity.z*1.75f));
			}
			
			gravLast = gravity;
			mPos[1] = mPos[0];
			mPos[0] = Ogre::Vector3(dynamic_cast<btRigidBody*>(mActor)->getCenterOfMassPosition().x(),
				dynamic_cast<btRigidBody*>(mActor)->getCenterOfMassPosition().y(),
				dynamic_cast<btRigidBody*>(mActor)->getCenterOfMassPosition().z());
			mOrient[1] = mOrient[0];
			mOrient[0] = Ogre::Quaternion(dynamic_cast<btRigidBody*>(mActor)->getOrientation().x(),
				dynamic_cast<btRigidBody*>(mActor)->getOrientation().y(),
				dynamic_cast<btRigidBody*>(mActor)->getOrientation().z(),
				dynamic_cast<btRigidBody*>(mActor)->getOrientation().w());
		}
	}
}

//-----------------------------------------------------------------------
void PhysicsObject::setUserData(void* data)
{
	// pass it right along to bullet
	mActor->setUserPointer(data);
}

//-----------------------------------------------------------------------
void* PhysicsObject::getUserData()
{
	return mActor->getUserPointer();
}

//-----------------------------------------------------------------------
bool PhysicsObject::readyForDelete()
{
    return mReadyForDelete;
}

//-----------------------------------------------------------------------
void PhysicsObject::_kill()
{
    mReadyForDelete = true;
}

