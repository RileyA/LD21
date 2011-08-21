#include "StdHeaders.hpp"
#include "PhysicsMgr.hpp"
#include "Game.hpp"

PhysicsMgr::PhysicsMgr()
:started(0),
mInterpolation(0.f),
mTimeStep(1.f/100.f),
mAccumulation(0.f)
{
	
}

PhysicsMgr::~PhysicsMgr()
{
	deinit();
}

void PhysicsMgr::init()
{
	mDynamicsWorld = NULL;

	mCollisionConfiguration = new btDefaultCollisionConfiguration();

	mDispatcher = new	btCollisionDispatcher(mCollisionConfiguration);
	mBroadphase = new btDbvtBroadphase();

	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	mSolver = sol;

	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher,mBroadphase,mSolver,mCollisionConfiguration);

	mDynamicsWorld->setGravity(btVector3(0,0,0));
	mGlobalGravity = Ogre::Vector3(0,0,0);

	mTimeStep = 1.f/250.f;
	mAccumulation = 0.f;
	mInterpolation = 0.f;
	started = true;
}

void PhysicsMgr::deinit()
{
	if(started)
	{
		int i;

		for(unsigned int i=0;i<mObjects.size();++i)
		{
			delete mObjects[i];
			mObjects[i] = 0;
		}

		for (i=mDynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
		{
			btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			mDynamicsWorld->removeCollisionObject( obj );
			delete obj;
		}

		mObjects.clear();

		std::map<String,btCollisionShape*>::iterator iter = mShapes.begin();
		while(iter!=mShapes.end())
		{
			delete iter->second;
			++iter;
		}
		mShapes.clear();

		delete mDynamicsWorld;
		delete mSolver;
		delete mBroadphase;
		delete mDispatcher;
		delete mCollisionConfiguration;

		started = false;
	}
}

void PhysicsMgr::update(Real delta)
{
	if(started)
	{
		mAccumulation += delta;

		while(mAccumulation>=mTimeStep)
		{
			mDynamicsWorld->stepSimulation(mTimeStep,0);

			for(unsigned int i=0;i<mObjects.size();++i)
			{
				if(!mObjects[i]->readyForDelete())
				{
					mObjects[i]->update(true,0,getLocalGravity(mObjects[i]->getPosition()));
				}
				else
				{
					delete mObjects[i];
					mObjects[i] = 0;
					mObjects.erase(mObjects.begin()+i);
					--i;
				}
			}
			mAccumulation -= mTimeStep;
		}

		mInterpolation = mAccumulation/mTimeStep;

		for(unsigned int i=0;i<mObjects.size();++i)
		{
			mObjects[i]->update(false,mInterpolation,mGlobalGravity);
		}
	}
}

float PhysicsMgr::getInterpolation()
{
	return mInterpolation;
}
 
Trimesh PhysicsMgr::createStaticTrimesh(const MeshData& d, Ogre::Vector3 pos)
{
	btTriangleMesh *triMesh = new btTriangleMesh();

	for(int i=0;i<d.vertices.size();i+=9)
	{
		triMesh->addTriangle(
			btVector3(
				d.vertices[i],
				d.vertices[i+1],
				d.vertices[i+2]),
			btVector3(
				d.vertices[i+3],
				d.vertices[i+4],
				d.vertices[i+5]),
			btVector3(
				d.vertices[i+6],
				d.vertices[i+7],
				d.vertices[i+8]));
	}

	btBvhTriangleMeshShape* mShape = new btBvhTriangleMeshShape(triMesh,true);
	btVector3 localInertia(0,0,0);
	btCollisionObject* actor = new btCollisionObject();
	actor->setCollisionShape(mShape);
	actor->setWorldTransform(btTransform(btQuaternion::getIdentity(),btVector3(pos.x,pos.y,pos.z)));
	actor->setRestitution(0.f);
	actor->setFriction(0.f);
	mDynamicsWorld->addCollisionObject(actor,COLLISION_GROUP_1);
	//mObjects.push_back(new PhysicsObject(actor,mDynamicsWorld));
	Trimesh out = {actor, triMesh, mShape};
	return out;
}

//PhysicsObject* PhysicsMgr::createStaticTrimesh(GfxObject* object,Ogre::Vector3 pos)
//{
//	return createStaticTrimesh(object->getEntity()->getMesh()->getName(),pos);
//}

PhysicsObject* PhysicsMgr::createCube(Ogre::Vector3 scale,Ogre::Vector3 pos)
{
	if(mShapes.find("BOX"+Ogre::StringConverter::toString(scale))==mShapes.end())
	{
		btBoxShape *boxShape = new btBoxShape(btVector3(scale.x,scale.y,scale.z));
		mShapes["BOX"+Ogre::StringConverter::toString(scale)] = (boxShape);
	}

	btVector3 localInertia(0,0,0);
	mShapes["BOX"+Ogre::StringConverter::toString(scale)]->calculateLocalInertia(18.f,localInertia);

	btRigidBody* actor = new btRigidBody(18.f,0,mShapes["BOX"+Ogre::StringConverter::toString(scale)],localInertia);	
	actor->setRestitution(0.f);
	actor->setFriction(0.f);
	actor->setAnisotropicFriction(btVector3(0.f,0.f,0.f));
	actor->setWorldTransform(btTransform(btQuaternion::getIdentity(),btVector3(pos.x,pos.y,pos.z)));

	dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld)->addRigidBody(actor,COLLISION_GROUP_1,COLLISION_GROUP_1);
	mObjects.push_back(new PhysicsObject(actor,mDynamicsWorld));

	return mObjects[mObjects.size()-1];
}

PhysicsObject* PhysicsMgr::createSphere(float radius,Ogre::Vector3 pos)
{
	if(mShapes.find("SPHERE"+Ogre::StringConverter::toString(radius))==mShapes.end())
	{
		btSphereShape *sphereShape = new btSphereShape(radius);
		mShapes["SPHERE"+Ogre::StringConverter::toString(radius)] = (sphereShape);
	}

	btVector3 localInertia(0,0,0);
	mShapes["SPHERE"+Ogre::StringConverter::toString(radius)]->calculateLocalInertia(18.f,localInertia);

	btRigidBody* actor = new btRigidBody(18.f,0,mShapes["SPHERE"+Ogre::StringConverter::toString(radius)],localInertia);	
	actor->setRestitution(0.f);
	actor->setFriction(0.f);
	actor->setAnisotropicFriction(btVector3(0.9f,0.9f,0.9f));
	actor->setWorldTransform(btTransform(btQuaternion::getIdentity(),btVector3(pos.x,pos.y,pos.z)));

	//dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld)->addRigidBody(actor,COLLISION_GROUP_2,COLLISION_GROUP_2|COLLISION_GROUP_3|COLLISION_GROUP_1);
	dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld)->addRigidBody(actor,COLLISION_GROUP_1,COLLISION_GROUP_1); //if I need collision filtering..

	mObjects.push_back(new PhysicsObject(actor,mDynamicsWorld));

	return mObjects[mObjects.size()-1];
}


PhysicsObject* PhysicsMgr::createCapsule(float radius, float height,Ogre::Vector3 pos)
{
	if(mShapes.find("CAPS"+Ogre::StringConverter::toString(radius))==mShapes.end())
	{
		btCapsuleShape *sphereShape = new btCapsuleShape(radius, height);
		mShapes["CAPS"+Ogre::StringConverter::toString(radius)] = (sphereShape);
	}

	btVector3 localInertia(0,0,0);
	mShapes["CAPS"+Ogre::StringConverter::toString(radius)]->calculateLocalInertia(18.f,localInertia);

	btRigidBody* actor = new btRigidBody(18.f,0,mShapes["CAPS"+Ogre::StringConverter::toString(radius)],localInertia);	
	actor->setRestitution(0.f);
	actor->setFriction(0.f);
	actor->setAnisotropicFriction(btVector3(0.f,0.f,0.f));
	actor->setWorldTransform(btTransform(btQuaternion::getIdentity(),btVector3(pos.x,pos.y,pos.z)));

	//dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld)->addRigidBody(actor,COLLISION_GROUP_2,COLLISION_GROUP_2|COLLISION_GROUP_3|COLLISION_GROUP_1);
	dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld)->addRigidBody(actor,COLLISION_GROUP_2,COLLISION_GROUP_1); //if I need collision filtering..

	mObjects.push_back(new PhysicsObject(actor,mDynamicsWorld));

	return mObjects[mObjects.size()-1];
}

PhysicsObject* PhysicsMgr::createConvexHull(String meshname,Ogre::Vector3 pos,Ogre::Vector3 scale)
{
	bool newshape = false;
	size_t vertex_count;
	float* vertices;
	size_t index_count;
	vertex_count = index_count = 0;

	bool added_shared = false;
	size_t current_offset = vertex_count;
	size_t shared_offset = vertex_count;
	size_t next_offset = vertex_count;
	size_t index_offset = index_count;
	size_t prev_vert = vertex_count;
	size_t prev_ind = index_count;
	std::vector<Ogre::Vector3> vertVect;
	btConvexShape *convexShape;
	//btShapeHull* hull;

	if(meshname=="CUBE"||meshname=="BOX")
	{
		return createCube(scale,pos);
	}
	if(mShapes.find(meshname)==mShapes.end())
	{
		newshape = true;

		Ogre::MeshPtr mesh = Ogre::MeshManager::getSingletonPtr()->load(meshname,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		// Calculate how many vertices and indices we're going to need
		for(int i = 0;i < mesh->getNumSubMeshes();i++)
		{
			Ogre::SubMesh* submesh = mesh->getSubMesh(i);

			// We only need to add the shared vertices once
			if(submesh->useSharedVertices)
			{
				if(!added_shared)
				{
					Ogre::VertexData* vertex_data = mesh->sharedVertexData;
					vertex_count += vertex_data->vertexCount;
					added_shared = true;
				}
			}
			else
			{
				Ogre::VertexData* vertex_data = submesh->vertexData;
				vertex_count += vertex_data->vertexCount;
			}

			// Add the indices
			Ogre::IndexData* index_data = submesh->indexData;
			index_count += index_data->indexCount;
		}

		// Allocate space for the vertices and indices
		int a = vertex_count;

		vertices = new float[vertex_count*3];

		added_shared = false;

		// Run through the submeshes again, adding the data into the arrays
		for(int i = 0;i < mesh->getNumSubMeshes();i++)
		{
			Ogre::SubMesh* submesh = mesh->getSubMesh(i);

			Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
			if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
			{
				if(submesh->useSharedVertices)
				{
					added_shared = true;
					shared_offset = current_offset;
				}

				const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
				Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
				unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
				Ogre::Real* pReal;

				for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
				{
					posElem->baseVertexPointerToElement(vertex, &pReal);

					Ogre::Vector3 pt;

					pt.x = (*pReal++);
					pt.y = (*pReal++);
					pt.z = (*pReal++);

					bool skip = false;

					// ignore duped verts
					for(unsigned int i=0;i<vertVect.size();++i)
					{
						if(vertVect[i]==pt)
						{
							skip = true;
							//std::cout<<"IGNORED!\n";
						}
					}

					// skipping duped verts caused some kind of f-up, TOBEFIXED eventually
					if(!skip)
					{
						vertices[current_offset + (j*3)] = pt.x*0.92f;
						vertices[current_offset + (j*3) + 1] = pt.y*0.92f;
						vertices[current_offset + (j*3) + 2] = pt.z*0.92f;
						vertVect.push_back(pt);
					}
				}
				vbuf->unlock();
				next_offset += vertex_data->vertexCount;
			}
		}


		convexShape = new btConvexHullShape(static_cast<btScalar*>(vertices),vertVect.size(),3*sizeof(btScalar));
		//create a hull approximation
		btConvexShape *finalShape = 0;
		//std::cout<<"size: "<<vertVect.size()<<"\n";
		// seemed kinda iffy?
		//if(vertVect.size()>75)
		//{
		//	hull = new btShapeHull(convexShape);
		//	btScalar margin = convexShape->getMargin();
		//	hull->buildHull(margin);
		//	//btConvexHullShape* simplifiedConvexShape = new btConvexHullShape(hull->getVertexPointer(),hull->numVertices());
		//	
		//	btConvexHullShape* simplifiedConvexShape = new btConvexHullShape();
		//	for (int i=0;i<hull->numVertices();i++)
		//	{
		//		btVector3 vect = hull->getVertexPointer()[i]*0.9;
		//		std::cout<<"Vert: "<<vect.x()<<" "<<vect.y()<<" "<<vect.z()<<"\n";
		//		simplifiedConvexShape->addPoint(vect);
		//	}
		//	mShapes[meshname] = (simplifiedConvexShape);
		//	finalShape = simplifiedConvexShape;
		//}
		//else
		//{
			mShapes[meshname] = (convexShape);
			finalShape = convexShape;
		//}
	}
	//finalShape->setMargin(0.f);

	btVector3 localInertia(0,0,0);
	mShapes[meshname]->calculateLocalInertia(180.f,localInertia);

	btRigidBody* actor = new btRigidBody(180.f,0,mShapes[meshname],localInertia);	
	actor->setWorldTransform(btTransform(btQuaternion::getIdentity(),btVector3(pos.x,pos.y,pos.z)));
	actor->setRestitution(0.f);
	actor->setFriction(0.8f);
	//actor->setAnisotropicFriction(btVector3(0.3f,0.3f,0.3f));
	actor->setDamping(0.2f,0.7f);

	dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld)->addRigidBody(actor,COLLISION_GROUP_1,COLLISION_GROUP_1);
	mObjects.push_back(new PhysicsObject(actor,mDynamicsWorld));

	if(newshape)
	{
		delete[] vertices;
		//if(vertVect.size()>75)
		//{
		//	delete hull;
		//	delete convexShape;
		//}
	}

	return mObjects[mObjects.size()-1];
}

PhysicsObject* PhysicsMgr::createConvexHull(GfxObject* object,Ogre::Vector3 pos)
{
	return createConvexHull(object->getEntity()->getMesh()->getName(),pos);
}

Ogre::Vector3 PhysicsMgr::getLocalGravity(Ogre::Vector3 pos)
{
	return mGlobalGravity;
}

Ogre::Vector3 PhysicsMgr::getLocalGravityCCT(Ogre::Vector3 pos)
{
	return mGlobalGravity;
}

Ogre::Vector3 PhysicsMgr::getGravity()
{
	return mGlobalGravity;
}

void PhysicsMgr::setGravity(Ogre::Vector3 grav)
{
	mGlobalGravity = grav;
}

RaycastReport PhysicsMgr::raycastSimple(Ogre::Vector3 pos,Ogre::Vector3 dir,Real dist,short group,short mask)
{
	dir.normalise();
	dir*=dist;

	btCollisionWorld::ClosestRayResultCallback cb(btVector3(pos.x,pos.y,pos.z), btVector3(pos.x+dir.x,pos.y+dir.y,pos.z+dir.z));
	// everythang:
	cb.m_collisionFilterGroup = COLLISION_GROUP_0|COLLISION_GROUP_1|COLLISION_GROUP_3|COLLISION_GROUP_4|COLLISION_GROUP_5|COLLISION_GROUP_6|
		COLLISION_GROUP_7|COLLISION_GROUP_8|COLLISION_GROUP_9|COLLISION_GROUP_10|COLLISION_GROUP_11|COLLISION_GROUP_12|
		COLLISION_GROUP_13|COLLISION_GROUP_14|COLLISION_GROUP_15;
	cb.m_collisionFilterMask = COLLISION_GROUP_0|COLLISION_GROUP_1|COLLISION_GROUP_3|COLLISION_GROUP_4|COLLISION_GROUP_5|COLLISION_GROUP_6|
		COLLISION_GROUP_7|COLLISION_GROUP_8|COLLISION_GROUP_9|COLLISION_GROUP_10|COLLISION_GROUP_11|COLLISION_GROUP_12|
		COLLISION_GROUP_13|COLLISION_GROUP_14|COLLISION_GROUP_15;

	cb.m_collisionFilterGroup = cb.m_collisionFilterGroup^group;
	cb.m_collisionFilterMask = cb.m_collisionFilterMask^mask;

	mDynamicsWorld->rayTest(btVector3(pos.x,pos.y,pos.z),btVector3(pos.x+dir.x,pos.y+dir.y,pos.z+dir.z),cb);
	
	if(cb.hasHit())
	{
		return RaycastReport(Ogre::Vector3(cb.m_hitNormalWorld.x(),cb.m_hitNormalWorld.y(),cb.m_hitNormalWorld.z()),Ogre::Vector3(cb.m_hitPointWorld.x(),cb.m_hitPointWorld.y(),cb.m_hitPointWorld.z()),cb.m_collisionObject->getBroadphaseHandle()->m_collisionFilterGroup,cb.m_collisionObject->getUserPointer());
	}
	else
	{
		return RaycastReport();
	}
}

void Trimesh::kill()
{
	Game::getPtr()->getPhysics()->getScene()->removeCollisionObject(actor);
	delete actor;
	delete shape;
	delete mesh;
}


bool PhysicsMgr::overlap(PhysicsObject* obj, Vector3 pos, Ogre::Quaternion q)
{
	Vector3 oldPos = obj->getPosition();
	Ogre::Quaternion oldQ = obj->getOrientation();
	OverlapResultCallback overlapCall;
	obj->setPosition(pos);
	obj->setOrientation(q);
	mDynamicsWorld->contactTest(obj->getCObj(),overlapCall);
	obj->setPosition(oldPos);
	obj->setOrientation(oldQ);
	return overlapCall.hits > 1;
}

