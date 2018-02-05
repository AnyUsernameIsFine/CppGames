#include "Main.h"

int main(int argc, char* argv[])
{
	Game::Game game;
	return game.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}
