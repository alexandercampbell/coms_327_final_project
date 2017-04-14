
#include <stdlib.h>
#include <time.h>

#include "world.hpp"
#include "io.hpp"
#include "model.hpp"
#include "pc.hpp"

int main(int argc, char *argv[]) {
	srand(time(0));

	io_init();
	atexit(io_quit);

	World w;
	world_init(&w);
	while (true) {
		io_render(&w);

		Key key = io_wait_for_key();
		if (key == Key::quit) break;

		pc_process_key(&w, key);
	}
}

