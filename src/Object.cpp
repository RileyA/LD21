#include "Game.hpp"

Object::Object(String bcuket) // I mispelled that...
{
	mGame = Game::getPtr();
	mGame->getBucket(bcuket)->pushObject(this);
	bucket = bcuket;
}

Object::~Object()
{
}

void Object::kill()
{
	mGame->getBucket(bucket)->removeObject(this);
}
