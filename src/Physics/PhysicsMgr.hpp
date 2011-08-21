#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "StdHeaders.hpp"
#include "Subsystem.hpp"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "PhysicsObject.hpp"
#include "Gfx/GfxObject.hpp"

#define BIT(x) (1<<(x))
enum CollisionTypes 
{
	COLLISION_GROUP_0 = 0,
	COLLISION_GROUP_1 = BIT(1),
	COLLISION_GROUP_2 = BIT(2),
	COLLISION_GROUP_3 = BIT(3),
	COLLISION_GROUP_4 = BIT(4),
	COLLISION_GROUP_5 = BIT(5),
	COLLISION_GROUP_6 = BIT(6),
	COLLISION_GROUP_7 = BIT(7),
	COLLISION_GROUP_8 = BIT(8),
	COLLISION_GROUP_9 = BIT(9),
	COLLISION_GROUP_10 = BIT(10),
	COLLISION_GROUP_11 = BIT(11),
	COLLISION_GROUP_12 = BIT(12),
	COLLISION_GROUP_13 = BIT(13),
	COLLISION_GROUP_14 = BIT(14),
	COLLISION_GROUP_15 = BIT(15)
};

struct	OverlapResultCallback : public btCollisionWorld::ContactResultCallback
{
	unsigned int hits;

	OverlapResultCallback()
	{
		hits = 0;
		m_collisionFilterGroup = COLLISION_GROUP_1;
		m_collisionFilterMask = COLLISION_GROUP_1;
	}

	virtual	btScalar addSingleResult(btManifoldPoint& cp,const btCollisionObject* colObj0,
		int partId0,int index0,const btCollisionObject* colObj1,int partId1,int index1)
	{
		if(cp.getDistance()<=0)
		{
			++hits;
		}
		return 0.f;
	}
};

struct Trimesh
{
	btCollisionObject* actor;
	btTriangleMesh* mesh;
	btBvhTriangleMeshShape* shape;
	void kill();
};

struct RaycastReport
{
	RaycastReport()
	{
		hit = false;
	}
	RaycastReport(Ogre::Vector3 n,Ogre::Vector3 h,short hitGroup=0,void* userD=0)
	{
		normal = n;
		hitP = h;
		hit = true;
		shapeHitGroup = hitGroup;
		userData = userD;
	}
	Ogre::Vector3 normal;
	Ogre::Vector3 hitP;
	bool hit;
	short shapeHitGroup;
	void* userData;
};

class PhysicsMgr : public Subsystem
{
public:

	PhysicsMgr();
	~PhysicsMgr();

	void init();
	void deinit();
	void update(Real delta);

	void endState()
	{
		deinit();
		init();
	};


	Trimesh createStaticTrimesh(const MeshData& d, Ogre::Vector3 pos);
	//PhysicsObject* createStaticTrimesh(GfxObject* object,Ogre::Vector3 pos);
	//PhysicsObject* createStaticTrimesh(String mesh,Ogre::Vector3 pos);
	//PhysicsObject* createStaticTrimesh(const MeshData& d,Ogre::Vector3 pos);
	PhysicsObject* createCube(Ogre::Vector3 scale,Ogre::Vector3 pos);
	PhysicsObject* createConvexHull(GfxObject* object,Ogre::Vector3 pos);
	PhysicsObject* createConvexHull(String meshname,Ogre::Vector3 pos,Ogre::Vector3 scale=Ogre::Vector3(1,1,1));
	PhysicsObject* createSphere(float radius,Ogre::Vector3 pos);
	PhysicsObject* createCapsule(float radius, float height,Ogre::Vector3 pos);

	bool isEnabled();

	btDynamicsWorld* getScene(){return mDynamicsWorld;}

	float getInterpolation();

	Ogre::Vector3 getLocalGravity(Ogre::Vector3 pos);
	Ogre::Vector3 getLocalGravityCCT(Ogre::Vector3 pos);

	Ogre::Vector3 getGravity();

	void setGravity(Ogre::Vector3 grav);

	RaycastReport raycastSimple(Ogre::Vector3 pos,Ogre::Vector3 dir,Real dist=10000.f,short group=0,short mask=0);

	bool overlap(PhysicsObject* obj, Vector3 p, Ogre::Quaternion q = Ogre::Quaternion::IDENTITY);

protected:

	btDynamicsWorld* mDynamicsWorld;
	btDefaultCollisionConfiguration* mCollisionConfiguration;
	btCollisionDispatcher* mDispatcher;
	btDbvtBroadphase* mBroadphase;
	btSequentialImpulseConstraintSolver* mSolver;

	btRigidBody* dynamicActor;

	std::vector<PhysicsObject*> mObjects;
	std::map<std::string,btCollisionShape*> mShapes;

	Ogre::Vector3 mGlobalGravity;

	bool started;
	Real mTimeStep;
	Real mAccumulation;
	Real mInterpolation;

};

#endif
