
#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include "Game.h"

int main(int argc, char **argv) {

	Game("Shogun", 1280, 720).Run();

	return 0;
}
