
#include <iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>

#include "world.hpp"
#include "io.hpp"
#include "model.hpp"
#include "pc.hpp"

int main(int argc, char *argv[]) {
	srand(time(0));

	cout << "Generating dungeon..." << endl;

	World *w = new World;
	world_init(w);

	io_init();

	if (io_main_menu(w)) {
		pc_update_memory(w);

		while (true) {
			io_render(w);

			Key key = io_wait_for_key();
			if (key == Key::quit) break;
			pc_process_key(w, key);

			world_update_mobs(w);
			if (w->pc->hp <= 0 || w->boss->hp <= 0) {
				io_render(w);
				while (io_wait_for_key() != Key::quit)
					;
				break;
			}
		}
	}

	io_quit();

	world_destroy(w);
	delete w;

	cout << "Thanks for playing!" << endl;

	return 0;
}

