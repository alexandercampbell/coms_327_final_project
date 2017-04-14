
#include "world.hpp"

static void populate_trees(Level *l) {
	int num_trees = RAND_BETWEEN(10, 30);
	for (int i = 0; i < num_trees; i++) {
		int x = RAND_BETWEEN(0, DUNGEON_WIDTH - 1);
		int y = RAND_BETWEEN(0, DUNGEON_HEIGHT - 1);

		l->cells[y][x] = Cell::tree;
	}
}

static void populate_rivers(Level *l) {
	int num_rivers = RAND_BETWEEN(5, 20);
	for (int i = 0; i < num_rivers; i++) {
		int river_length = RAND_BETWEEN(35, 250);
		int x = RAND_BETWEEN(0, DUNGEON_WIDTH - 1);
		int y = RAND_BETWEEN(0, DUNGEON_HEIGHT - 1);
		for (int j = 0; j < river_length; j++) {
			x += RAND_BETWEEN(-1, 1);
			y += RAND_BETWEEN(-1, 1);
			x = CLAMPED_TO(x, 0, DUNGEON_WIDTH - 1);
			y = CLAMPED_TO(y, 0, DUNGEON_HEIGHT - 1);
			l->cells[y][x] = Cell::river;
		}
	}
}

static void populate_mountains(Level *l) {
	for (int x = 1; x < DUNGEON_WIDTH - 1; x++) {
		l->cells[0][x] = Cell::mountain;
		l->cells[DUNGEON_HEIGHT - 1][x] = Cell::mountain;
	}
	for (int y = 0; y < DUNGEON_HEIGHT; y++) {
		l->cells[y][0] = Cell::mountain;
		l->cells[y][DUNGEON_WIDTH - 1] = Cell::mountain;
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

	l->down_stair_x = entrance_x;
	l->down_stair_y = entrance_y;
}

static void generate_town(Level *l) {
	for (int y = 0; y < DUNGEON_HEIGHT; y++) {
		for (int x = 0; x < DUNGEON_WIDTH; x++) {
			l->cells[y][x] = (FRAND() > 0.9) ? Cell::grass : Cell::none;
		}
	}

	memset(l->mobs, 0, DUNGEON_NUM_CELLS * sizeof(Mob*));
	populate_trees(l);
	populate_rivers(l);
	populate_mountains(l);
	place_dungeon_entrance(l);
}

static void place_dungeon_room(Level *l, int center_x, int center_y) {
	// height is ignored if `circular == true`
	bool circular = FRAND() > 0.8;
	if (circular) {
		int radius = RAND_BETWEEN(4, 14);
		for (int y = center_y - radius; y <= center_y + radius; y++) {
			for (int x = center_x - radius; x <= center_x + radius; x++) {
				if (x < 0 || x >= DUNGEON_WIDTH || y < 0 || y >= DUNGEON_HEIGHT) continue;

				int dist_x = (x - center_x);
				int dist_y = (y - center_y);
				if (sqrt(dist_x * dist_x + dist_y * dist_y) <= radius) {
					l->cells[y][x] = Cell::tunnel;
				}
			}
		}
	} else {
		int width = RAND_BETWEEN(4, 14);
		int height = RAND_BETWEEN(4, 14);
		for (int y = center_y - height/2; y <= center_y + height/2; y++) {
			for (int x = center_x - width/2; x <= center_x + width/2; x++) {
				if (x < 0 || x >= DUNGEON_WIDTH || y < 0 || y >= DUNGEON_HEIGHT) continue;

				l->cells[y][x] = Cell::tunnel;
			}
		}
	}
}

static void generate_dungeon_level(Level *l, int above_stair_x, int above_stair_y) {
	for (int y = 0; y < DUNGEON_HEIGHT; y++) {
		for (int x = 0; x < DUNGEON_WIDTH; x++) {
			l->cells[y][x] = Cell::rock;
		}
	}

	// Set up staircases.
	l->up_stair_x = above_stair_x;
	l->up_stair_y = above_stair_y;
	l->cells[l->up_stair_y][l->up_stair_x] = Cell::stair_up;
	place_dungeon_room(l, l->up_stair_x, l->up_stair_y);

	if (l->depth == NUM_LEVELS - 1) {
		// we are the final floor
	} else {
		l->down_stair_x = RAND_BETWEEN(1, DUNGEON_WIDTH);
		l->down_stair_y = RAND_BETWEEN(1, DUNGEON_HEIGHT);
		l->cells[l->down_stair_y][l->down_stair_x] = Cell::stair_down;

		place_dungeon_room(l, l->down_stair_x, l->down_stair_y);
	}

	int num_rooms = RAND_BETWEEN(3, 5);
	for (int i = 0; i < num_rooms; i++) {
		int room_x = RAND_BETWEEN(0, DUNGEON_WIDTH-1);
		int room_y = RAND_BETWEEN(0, DUNGEON_HEIGHT-1);
		place_dungeon_room(l, room_x, room_y);
	}
}

void world_init(World *w) {
	assert(w);

	memset(w, 0, sizeof(*w));
	memset(w->levels, 0, sizeof(w->levels));

	generate_town(&w->levels[0]);
	for (int i = 1; i < NUM_LEVELS; i++) {
		w->levels[i].depth = i;
		generate_dungeon_level(
			&w->levels[i],
			w->levels[i - 1].down_stair_x,
			w->levels[i - 1].down_stair_y
		);
	}

	w->cur_level = &w->levels[0];

	w->pc = new Mob;
	pc_init(w->pc);

	w->pc->x = RAND_BETWEEN(0, DUNGEON_WIDTH - 1);
	w->pc->y = RAND_BETWEEN(0, DUNGEON_HEIGHT - 1);
	w->cur_level->mobs[w->pc->y][w->pc->x] = w->pc;

	pc_update_memory(w);
}

void world_push_message(World *w, string text, MessageSeverity severity) {
	Message msg;
	msg.text = text;
	msg.severity = severity;

	w->messages.push_back(msg);
	if (w->messages.size() > WORLD_UI_MESSAGE_HISTORY) {
		w->messages.pop_front();
	}
}

