#include "State.hpp"
#include "Game.hpp"

State::State()
	:mDone(false)
{
	mGame = Game::getPtr();
}
