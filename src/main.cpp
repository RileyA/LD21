#include "Game.hpp"
#include "MenuState.hpp"
#include "Audio/AudioMgr.hpp"
#include "Gfx/GfxMgr.hpp"

int main(int argc, char** argv)
{
	Game* g = new Game();
	MenuState* m = new MenuState();

	AudioMgr am;
	GfxMgr gm;

	am.init();
	gm.init();

	am.play2D("boom.wav");

	Real tim = 0.f;

	while(tim < 2.f)
	{
		Real d = g->getDeltaTimeSeconds();
		tim += d;
		am.update(d);
		gm.update(d);
	}

	am.play2D("boom.wav");

	tim = 0.f;

	while(tim < 5.f)
	{
		Real d = g->getDeltaTimeSeconds();
		tim += d;
		am.update(d);
		gm.update(d);
	}

	gm.deinit();
	am.deinit();

	g->addState(m);
	g->go();
	delete g;
	return 0;
};
