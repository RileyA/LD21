#ifndef TitleState_H
#define TitleState_H

#include "State.hpp"

class FPSCam;
class Player;

// LOL jk, this is actually the gameplay state

class TitleState : public State
{
public:

	TitleState();
	virtual ~TitleState();

	void init();
	void deinit();
	void update(Real delta);

protected:

	Real mTimeElapsed;
	Player* player;
	int maxDist;

};

#endif
