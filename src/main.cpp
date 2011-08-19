#include "Game.hpp"
#include "MenuState.hpp"
#include "Audio/AudioMgr.hpp"

int main(int argc, char** argv)
{
	Game* g = new Game();
	MenuState* m = new MenuState();

	AudioMgr am;

	am.init();

	am.play2D("boom.wav");

	Real tim = 0.f;

	while(tim < 2.f)
	{
		Real d = g->getDeltaTimeSeconds();
		tim += d;
		am.update(d);
	}

	am.play2D("boom.wav");

	tim = 0.f;

	while(tim < 5.f)
	{
		Real d = g->getDeltaTimeSeconds();
		tim += d;
		am.update(d);
	}

	am.deinit();

	g->addState(m);
	g->go();
	delete g;
	return 0;
};
