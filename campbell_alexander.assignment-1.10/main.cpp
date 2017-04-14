
#include <stdlib.h>

#include "dungeon.hpp"
#include "io.hpp"
#include "model.hpp"

int main(int argc, char *argv[]) {
	io::init();
	atexit(io::quit);

	dungeon d;
	dungeon_generate(&d);
	io::render(&d);
	while (io::wait_for_key() != key::quit)
		;
}

