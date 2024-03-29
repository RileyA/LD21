#ifndef State_H
#define State_H

#include "StdHeaders.hpp" 

class Game;
class GfxMgr;
class AudioMgr;

namespace Oyster
{
	class Oyster;
}

/** Abstract game state */
class State
{
public:

	State();
	virtual ~State(){}

	virtual void init() = 0;
	virtual void deinit() = 0;
	virtual void update(Real delta) = 0;

	bool isDone(){return mDone;}
	void setDone(bool done = true){mDone = done;}

protected:

	bool mDone;
	Game* mGame;
	GfxMgr* mGfx;
	AudioMgr* mAudio;
	Oyster::Oyster* mOyster;

};

#endif

