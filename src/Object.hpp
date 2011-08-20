#ifndef Object_H
#define Object_H

#include "StdHeaders.hpp"
#include "EventHandler.hpp"

class Game;

class Object : public EventHandler
{
public:

	Object(String bucket = "Default");
	virtual ~Object();

	virtual void update(Real delta) = 0;

	void kill();

	Game* mGame;

private:

	String bucket;

};

#endif
