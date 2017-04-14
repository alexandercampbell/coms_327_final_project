
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

static void place_dungeon_entrance(Level *l) {
	int entrance_x = RAND_BETWEEN(16, DUNGEON_WIDTH - 1 - 16);
	int entrance_y = RAND_BETWEEN(16, DUNGEON_HEIGHT - 1 - 16);

	for (int y = -7; y <= 7; y++) {
		for (int x = -7; x <= 7; x++) {
			l->cells[entrance_y + y][entrance_x + x] = Cell::tunnel;
		}
	}

	for (int y = -3; y <= 3; y++) {
		for (int x = -3; x <= 3; x++) {
			l->cells[entrance_y + y][entrance_x + x] = Cell::tunnel;
		}
	}

	for (int i = -4; i <= 4; i++) {
		l->cells[entrance_y + i][entrance_x - 4] = Cell::rock;
		l->cells[entrance_y + i][entrance_x + 4] = Cell::rock;
		l->cells[entrance_y + 4][entrance_x + i] = Cell::rock;
		l->cells[entrance_y - 4][entrance_x + i] = Cell::rock;
	}

	l->cells[entrance_y][entrance_x + 4] = Cell::tunnel;
	l->cells[entrance_y][entrance_x] = Cell::stair_down;
}

static void generate_town(Level *l) {
	for (int y = 0; y < DUNGEON_HEIGHT; y++) {
		for (int x = 0; x < DUNGEON_WIDTH; x++) {
			l->cells[y][x] = (FRAND() > 0.9) ? Cell::grass : Cell::dirt;
		}
	}

	memset(l->mobs, 0, DUNGEON_NUM_CELLS * sizeof(Mob*));
	populate_rivers(l);
	place_dungeon_entrance(l);
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

