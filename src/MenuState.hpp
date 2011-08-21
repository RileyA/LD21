#ifndef MenuState_H
#define MenuState_H

#include "State.hpp"

class FPSCam;
class Player;
class MapManager;

extern int lastScore;

// LOL jk, this is actually the gameplay state

class MenuState : public State
{
public:

	MenuState();
	virtual ~MenuState();

	void init();
	void deinit();
	void update(Real delta);

protected:

	Real mTimeElapsed;
	Player* player;
	Oyster::Text* dist;
	int maxDist;
	MapManager* mm;
	Oyster::Rectangle* rects[20];
	bool text;

};

#endif
