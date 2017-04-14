
#include <stdlib.h>
#include <time.h>

#include "world.hpp"
#include "io.hpp"
#include "model.hpp"
#include "pc.hpp"

int main(int argc, char *argv[]) {
	srand(time(0));

	io::init();
	atexit(io::quit);

	World w;
	world_init(&w);
	while (true) {
		io::render(&w);

		Key key = io::wait_for_key();
		if (key == Key::quit) break;

		pc::process_key(w, key);
	}
}

