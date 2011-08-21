#ifndef Player_H
#define Player_H
 
#include "FPSCam.hpp"
#include "PhysicsObject.hpp"

class Player : public Object
{
public:

	Player();
	virtual ~Player();

	void update(Real delta);

	FPSCam* mCam;
	PhysicsObject* mController;
	PhysicsObject* mCap;
	PhysicsObject* mCap2;
	PhysicsObject* mSph;

	Real jumpFactor;
	bool onGround;

	bool crouching;
	bool crouched;
	Real crouchTime;

	Vector3 offset;
	bool rotating;
	Real rotation;
	bool leftrot; 

	float minSpeed;
	float maxSpeed;
	float speed;
	Vector3 lastPos;

	int skipFrame;


	bool startSeq;
	bool camTurn;
	bool camWait;
	Real wait;
	Real camReturn;

	Ogre::Radian fov;

	Real slowtimer;

	Real backAngle;

	Real spt;
	Real cht;
	Real cht2;

	Real startTimer;

	std::list<float> spds; 
	std::list<Vector3> cp; 

	struct Shot
	{
		Ogre::SceneNode* node;
		Ogre::Entity* entity;
		Vector3 direction;
		bool done;
		Real timeout;
		~Shot()
		{
			node->getCreator()->destroyEntity(entity);
			node->getCreator()->destroySceneNode(node);
		}
	};

	void shoot();
	void updateShots(Real delta);

	int speedp;
	Real speedprog;
	Real slowprog;
	int lastFloor;

	std::list<Shot*> mShots;
	std::list<Shot*> mSpareShots;

};

#endif
