#include "Game.hpp"
#include "MenuState.hpp"
#include "Audio/AudioMgr.hpp"
#include "Gfx/GfxMgr.hpp"

int main(int argc, char** argv)
{
	Game* g = new Game();
	MenuState* m = new MenuState();
	g->addState(m);
	g->go();
	delete g;
	return 0;
};
