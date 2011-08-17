#include "Game.hpp"

int main(int argc, char** argv)
{
	Game* g = new Game();
	g->go();
	delete g;
	return 0;
};
