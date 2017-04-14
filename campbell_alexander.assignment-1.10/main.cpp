
#include <stdlib.h>
#include <time.h>

#include "dungeon.hpp"
#include "io.hpp"
#include "model.hpp"

int main(int argc, char *argv[]) {
	srand(time(0));

	io::init();
	atexit(io::quit);

	Dungeon d;
	dungeon_generate(&d);
	do {
		io::render(&d);
	} while (io::wait_for_key() != Key::quit);
}

