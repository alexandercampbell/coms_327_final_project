
#include "dungeon.hpp"

static void populate_rivers(dungeon *d) {
	int num_rivers = RAND_BETWEEN(5, 20);
	for (int i = 0; i < num_rivers; i++) {
		int river_length = RAND_BETWEEN(35, 250);
		int x = RAND_BETWEEN(0, DUNGEON_WIDTH);
		int y = RAND_BETWEEN(0, DUNGEON_HEIGHT);
		for (int j = 0; j < river_length; j++) {
			x += RAND_BETWEEN(-1, 1);
			y += RAND_BETWEEN(-1, 1);
			x = CLAMPED_TO(x, 0, DUNGEON_WIDTH - 1);
			y = CLAMPED_TO(y, 0, DUNGEON_HEIGHT - 1);
			d->cells[y][x] = cell::river;
		}
	}
}

void dungeon_generate(dungeon *d) {
	memset(d, 0, sizeof(*d));
	memset(d->cells, int(cell::grass), DUNGEON_NUM_CELLS);
	memset(d->mobs, 0, DUNGEON_NUM_CELLS);

	populate_rivers(d);

	// TODO: remove this memory leak
	mob *m = new mob();
	m->symb = '@';
	d->mobs[24][15] = m;
}

