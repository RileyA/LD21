#ifndef Game_H
#define Game_H

#include "StdHeaders.hpp"
#include "Bucket.hpp"
#include "State.hpp"

/** Root of the whole thang, singleton (though it must be explicitly constructed) */
class Game
{
public:

	Game();
	~Game();
	
	/** Encapsulates the whole run */
	void go();

	/** Adds a gamestate to the deque */
	void addState(State* s);

	/** Does simple logging */
	void logMessage(String message);

	Bucket* createBucket(String name);
	Bucket* getBucket(String name);

	static Game& getRef(){return *msInstance;}
	static Game* getPtr(){return msInstance;}

private:

	void init();
	void deinit();

	void startState();
	void endState();

	std::deque<State*> mStates;

	std::map<String,Bucket*> mBuckets;

	// singleton stuffs (yuck)
	Game(const Game &);
	Game& operator=(const Game &);
	static Game* msInstance;
};

#endif
