#ifndef FPSCAM_H
#define FPSCAM_H

#include "Object.hpp"
#include "Ogre.h"
#include "Message.hpp"

class FPSCam : public Object
{
public:

	FPSCam(Ogre::Camera* c); 
	virtual ~FPSCam();

	void update(Real delta);
	void look(const Message& m);

private:

	Ogre::SceneNode* camRoll;
	Ogre::SceneNode* camYaw;
	Ogre::SceneNode* camPitch;
	Ogre::SceneNode* camPos;
	Real pichLimit;
	Real pitch;
	bool cameraMotion;
    Ogre::Camera* mCamera;
	Ogre::SceneManager* mSmgr;

};

#endif
