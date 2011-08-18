#include "MenuState.hpp"

MenuState::MenuState()
	:State()
{
	
}

MenuState::~MenuState()
{

}

void MenuState::init()
{
	//...
}

void MenuState::deinit()
{
	//...
}

void MenuState::update(Real delta)
{
	mTimeElapsed += delta;
	std::cout<<mTimeElapsed<<"\n";

	if(mTimeElapsed > 5.f)
		setDone(true);
}
