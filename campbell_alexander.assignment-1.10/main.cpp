
#include <stdlib.h>
#include <time.h>

#include "dungeon.hpp"
#include "io.hpp"
#include "model.hpp"
#include "pc.hpp"

int main(int argc, char *argv[]) {
	srand(time(0));

	io::init();
	atexit(io::quit);

	Dungeon d;
	dungeon_generate(&d);
	while (true) {
		io::render(&d);

		Key key = io::wait_for_key();
		if (key == Key::quit) break;

		pc::process_key(d, key);
	}
}

