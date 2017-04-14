
#include "dungeon.hpp"

void dungeon_generate(dungeon *d) {
	memset(d, 0, sizeof(*d));
	memset(d->cells, int(cell::grass), DUNGEON_NUM_CELLS);
	memset(d->mobs, 0, DUNGEON_NUM_CELLS);

	mob *m = (mob*) malloc(sizeof(mob));
	m->symb = '@';
	d->mobs[24][15] = m;
}

