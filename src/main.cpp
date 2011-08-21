#include "Game.hpp"
#include "MenuState.hpp"
#include "TitleState.hpp"
#include "Audio/AudioMgr.hpp"
#include "Gfx/GfxMgr.hpp"

int main(int argc, char** argv)
{
	Game* g = new Game();
	TitleState* m = new TitleState();
	//MenuState* m = new MenuState();
	g->addState(m);
	//MenuState* mm = new MenuState();
	//g->addState(mm);
	try
	{
		g->go();
		delete g;
	}
	catch(std::runtime_error e)
	{
		//....
	}

	return 0;
};
