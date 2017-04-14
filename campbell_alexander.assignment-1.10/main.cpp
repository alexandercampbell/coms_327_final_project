
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
	world_push_message(&w, "You see some bones here.");
	world_push_message(&w, "You heal yourself [+10 HP]", MessageSeverity::Good);
	world_push_message(&w, "You hear scratching in the distance.", MessageSeverity::Warning);
	world_push_message(&w, "The kobold strikes you. [-5 HP]", MessageSeverity::OhGodTheresBloodEverywhere);
	while (true) {
		io::render(&w);

		Key key = io::wait_for_key();
		if (key == Key::quit) break;

		pc::process_key(&w, key);
	}
}

