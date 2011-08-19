#include "Game.hpp"

Game* Game::msInstance = 0;

Game::Game()
{
	assert(!msInstance);
	msInstance = this;

#if LD_PLATFORM == PLATFORM_WIN32
	QueryPerformanceCounter(&mLastTime);
#else
	gettimeofday(&mLastTime, 0);
#endif
}

Game::~Game()
{
	// nothin' to see here, move along folks...
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
			// figure out delta time
			Real delta = getDeltaTimeSeconds();

			mAudio->update(delta);
			mGfx->update(delta);
			mInput->update(delta);

			currentState->update(delta);
			for(std::map<String, Bucket*>::iterator it = mBuckets.begin(); it != mBuckets.end(); ++it)
				it->second->update(delta);
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
	if(mBuckets.find(name) == mBuckets.end())
		mBuckets[name] = new Bucket(name);
	return mBuckets[name];
}

Bucket* Game::getBucket(String name)
{
	if(mBuckets.find(name) != mBuckets.end())
		return mBuckets[name];
	return 0;
}

Real Game::getDeltaTimeSeconds()
{
	// hackish, but both seem to provide reasonable precision
#if LD_PLATFORM == PLATFORM_WIN32
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	long long tm = now.QuadPart - mLastTime.QuadPart;
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	mLastTime = now;
	return static_cast<Real>(tm)/freq.QuadPart;
#else
	struct timeval now;
	gettimeofday(&now, 0);
	long seconds  = now.tv_sec  - mLastTime.tv_sec;
	long useconds = now.tv_usec - mLastTime.tv_usec;
	mLastTime = now;
	return (seconds*1000.f+useconds/1000.f)/1000.f;
#endif
}

void Game::init()
{
	logMessage("Starting up...");
	mGfx = new GfxMgr();
	mAudio = new AudioMgr();
	mInput = new InputMgr();
	mGfx->init();
	mAudio->init();
	mInput->init();
}

void Game::deinit()
{
	logMessage("Shutting down...");
	mGfx->deinit();
	mAudio->deinit();
	mInput->deinit();
	delete mGfx;
	delete mAudio;
	delete mInput;
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
	mGfx->endState();
	mAudio->endState();
	mInput->endState();
	// delete buckets and stuff
	for(std::map<String,Bucket*>::iterator it = mBuckets.begin(); it != mBuckets.end(); ++it)
	{
		delete it->second;
	}
	mBuckets.clear();
}
