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

};

#endif
