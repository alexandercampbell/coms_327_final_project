
#include <stdlib.h>

#include "dungeon.hpp"
#include "io.hpp"
#include "model.hpp"

int main(int argc, char *argv[]) {
	io_init();
	atexit(io_quit);

	dungeon d;
	dungeon_generate(&d);
	io_render(&d);
	while (io_wait_for_key() != key::quit)
		;
}

