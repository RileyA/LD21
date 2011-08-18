#ifndef Subsystem_H
#define Subsystem_H

#include "StdHeaders.hpp" 
#include "EventHandler.hpp" 

class Game;

/** Abstract game state */
class Subsystem : public EventHandler
{
public:

	Subsystem(){}
	virtual ~Subsystem(){}

	virtual void init() = 0;
	virtual void deinit() = 0;
	virtual void update(Real delta) = 0;
	virtual void endState() = 0;

};

#endif

