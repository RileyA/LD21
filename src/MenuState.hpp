#ifndef MenuState_H
#define MenuState_H

#include "State.hpp"

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

};

#endif
