
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
	while (true) {
		io_render(w);

		Key key = io_wait_for_key();
		if (key == Key::quit) break;
		pc_process_key(w, key);
	}
	io_quit();

	world_destroy(w);
	delete w;

	cout << "Thanks for playing!" << endl;

	return 0;
}

