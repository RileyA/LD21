#include "Game.hpp"

Game* Game::msInstance = 0;

Game::Game()
{
	assert(!msInstance);
	msInstance = this;
}

Game::~Game()
{
	//...
}

void Game::go()
{
	init();

	while(!mStates.empty())
	{
		State* currentState = mStates.front();
		startState();

		while(!currentState->isDone())
		{
			currentState->update(0.1f);
			for(std::map<String, Bucket*>::iterator it = mBuckets.begin(); it != mBuckets.end(); ++it)
				it->second->update(0.1f);
		}

		endState();
		delete currentState;
		mStates.pop_front();
	}

	deinit();
}

void Game::addState(State* s)
{
	mStates.push_back(s);
}

void Game::logMessage(String message)
{
	std::cout<<"::"<<message<<"\n";
}

Bucket* Game::createBucket(String name)
{
	if(mBuckets.find(name) != mBuckets.end())
		return mBuckets[name];
	mBuckets[name] = new Bucket(name);
}

Bucket* Game::getBucket(String name)
{
	if(mBuckets.find(name) != mBuckets.end())
		return mBuckets[name];
	return 0;
}

void Game::init()
{
	logMessage("Starting up...");
}

void Game::deinit()
{
	logMessage("Shutting down...");
}

void Game::startState()
{
	logMessage("Starting state...");
	createBucket("Default");
	mStates.front()->init();
}

void Game::endState()
{
	logMessage("Ending state...");
	mStates.front()->deinit();
	// delete buckets and stuff
	for(std::map<String,Bucket*>::iterator it = mBuckets.begin(); it != mBuckets.end(); ++it)
	{
		delete it->second;
	}
	mBuckets.clear();
}
