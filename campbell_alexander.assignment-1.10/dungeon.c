
#include "dungeon.h"

void dungeon_generate(dungeon *d) {
	memset(d, 0, sizeof(*d));
	memset(d->cells, cell_grass, DUNGEON_NUM_CELLS);
}

