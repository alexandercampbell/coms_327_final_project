
#include "dungeon.h"

void dungeon_generate(dungeon *d) {
	memset(d, 0, sizeof(*d));
	memset(d->cells, cell_grass, DUNGEON_NUM_CELLS);
	memset(d->mobs, 0, DUNGEON_NUM_CELLS);

	mob *m = malloc(sizeof(mob));
	m->symb = '@';
	d->mobs[24][15] = m;
}

