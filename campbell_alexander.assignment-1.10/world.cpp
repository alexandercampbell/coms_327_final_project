
#include "world.hpp"

static void populate_rivers(Level *l) {
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
			l->cells[y][x] = Cell::river;
		}
	}
}

static void generate_town(Level *l) {
	memset(l->cells, int(Cell::grass), DUNGEON_NUM_CELLS);
	memset(l->mobs, 0, DUNGEON_NUM_CELLS);
	populate_rivers(l);
}

static void generate_dungeon_level(Level *l) {
}

void world_init(World *w) {
	assert(w);

	memset(w, 0, sizeof(*w));
	memset(w->levels, 0, sizeof(w->levels));

	generate_town(&w->levels[0]);
	for (int i = 1; i < NUM_LEVELS; i++) {
		w->levels[i].depth = i;
		generate_dungeon_level(&w->levels[i]);
	}

	w->cur_level = &w->levels[0];

	w->pc = new Mob;
	pc::init(w->pc);

	w->pc->x = RAND_BETWEEN(0, DUNGEON_WIDTH);
	w->pc->y = RAND_BETWEEN(0, DUNGEON_HEIGHT);
	w->cur_level->mobs[w->pc->y][w->pc->x] = w->pc;
}

