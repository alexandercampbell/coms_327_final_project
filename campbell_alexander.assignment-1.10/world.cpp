
#include "world.hpp"

bool level_location_clear(Level *l, int x, int y) {
	if (x < 0 || x >= DUNGEON_WIDTH || y < 0 || y >= DUNGEON_HEIGHT)
		return false;

	if (!CELL_IS_WALKABLE(l->cells[y][x]))
		return false;

	if (l->mobs[y][x])
		return false;

	if (l->items[y][x])
		return false;

	return true;
}

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

	l->cells[entrance_y + 4][entrance_x] = Cell::tunnel;
	l->cells[entrance_y + 4][entrance_x + 1] = Cell::tunnel;
	l->cells[entrance_y + 4][entrance_x - 1] = Cell::tunnel;

	l->down_stair_x = entrance_x;
	l->down_stair_y = entrance_y - 2;
	l->cells[l->down_stair_y][l->down_stair_x] = Cell::stair_down;

	l->cells[entrance_y][entrance_x - 2] = Cell::health_fountain;
}

static void place_circle(Level *l, Cell cell, int center_x, int center_y, int radius) {
	for (int y = center_y - radius; y <= center_y + radius; y++) {
		for (int x = center_x - radius; x <= center_x + radius; x++) {
			if (x < 0 || x >= DUNGEON_WIDTH || y < 0 || y >= DUNGEON_HEIGHT) continue;

			int dist_x = (x - center_x);
			int dist_y = (y - center_y);
			if (sqrt(dist_x * dist_x + dist_y * dist_y) <= radius) {
				l->cells[y][x] = cell;
			}
		}
	}
}

static void place_dungeon_room(Level *l, int center_x, int center_y) {
	// height is ignored if `circular == true`
	bool circular = FRAND() > 0.8;
	if (circular) {
		int radius = RAND_BETWEEN(4, 9);
		place_circle(l, Cell::tunnel, center_x, center_y, radius);
	} else {
		int width = RAND_BETWEEN(4, 10);
		int height = RAND_BETWEEN(4, 6);
		for (int y = center_y - height/2; y <= center_y + height/2; y++) {
			for (int x = center_x - width/2; x <= center_x + width/2; x++) {
				if (x < 0 || x >= DUNGEON_WIDTH || y < 0 || y >= DUNGEON_HEIGHT) continue;

				l->cells[y][x] = Cell::tunnel;
			}
		}
	}

	// chance to spawn a healing fountain in the room
	if (FRAND() > 0.95) {
		l->cells[center_y][center_x] = Cell::health_fountain;
	}
}

static void generate_tunnel_between(Level *l, int start_x, int start_y, int stop_x, int stop_y) {
	int x = start_x;
	int y = start_y;

	while (x != stop_x || y != stop_y) {
		int x_distance = stop_x - x;
		int y_distance = stop_y - y;
		bool x_farther = abs(x_distance) > abs(y_distance);

		if (x_farther) {
			if (FRAND() > 0.2) {
				x += (x_distance > 0) ? 1 : -1;
			} else {
				y += (y_distance > 0) ? 1 : -1;
			}
		} else {
			if (FRAND() > 0.2) {
				y += (y_distance > 0) ? 1 : -1;
			} else {
				x += (x_distance > 0) ? 1 : -1;
			}
		}

		x = CLAMPED_TO(x, 0, DUNGEON_WIDTH - 2);
		y = CLAMPED_TO(y, 0, DUNGEON_HEIGHT - 2);

		if (l->cells[y][x] == Cell::rock) l->cells[y][x] = Cell::tunnel;
		if (l->cells[y][x+1] == Cell::rock) l->cells[y][x+1] = Cell::tunnel;
		if (l->cells[y+1][x] == Cell::rock) l->cells[y+1][x] = Cell::tunnel;
		if (l->cells[y+1][x+1] == Cell::rock) l->cells[y+1][x+1] = Cell::tunnel;
	}
}

static void generate_and_place_mobs(Level *l) {
	int num_mobs = RAND_BETWEEN(6, 9);
	for (int i = 0; i < num_mobs; i++) {
		Mob *m = mob_generate(l->depth);
		if (!m) continue;

		while (true) {
			int x = RAND_BETWEEN(1, DUNGEON_WIDTH - 2);
			int y = RAND_BETWEEN(1, DUNGEON_HEIGHT - 2);

			if (level_location_clear(l, x, y)) {
				m->x = x;
				m->y = y;
				m->level = l->depth;

				l->mobs[y][x] = m;
				l->mob_turns.push_back(m);
				break;
			}
		}
	}
}

static void generate_and_place_items(Level *l) {
	int num_items = RAND_BETWEEN(3, 5);
	for (int i = 0; i < num_items; i++) {
		Item *item = item_generate(l->depth);

		while (true) {
			int x = RAND_BETWEEN(1, DUNGEON_WIDTH - 2);
			int y = RAND_BETWEEN(1, DUNGEON_HEIGHT - 2);

			if (level_location_clear(l, x, y)) {
				l->items[y][x] = item;
				break;
			}
		}
	}
}

void world_increase_loot(World *w) {
	for (int i = TOWN_LEVEL; i < BOSS_LEVEL; i++) {
		generate_and_place_items(&w->levels[i]);
	}
}

void world_kill(World *w, Mob *m, string cause) {
	Level *l = &w->levels[m->level];
	if (m == w->pc) {
		world_push_message(w, "---",
				MessageSeverity::OhGodTheresBloodEverywhere);
		world_push_message(w, "You have died.",
				MessageSeverity::OhGodTheresBloodEverywhere);
		world_push_message(w, string("You were killed by ") + cause + ".",
				MessageSeverity::OhGodTheresBloodEverywhere);
		world_push_message(w, string("Press [q] to return to reality, loser."),
				MessageSeverity::Warning);
		return;
	} else if (m == w->boss) {
		world_push_message(w, "---",
				MessageSeverity::Good);
		world_push_message(w, "You have defeated the final boss!");
		world_push_message(w, "Congratulations!",
				MessageSeverity::Good);
		world_push_message(w, "Press [q] to quit.",
				MessageSeverity::Good);
		return;
	} else {
		world_push_message(w, capitalized(m->name) +
				string(" dies."));
	}

	l->mobs[m->y][m->x] = nullptr;

	bool found_in_mob_turns = false;
	for (int i = 0; i < l->mob_turns.size(); i++) {
		if (l->mob_turns[i] == m) {
			assert(!found_in_mob_turns);
			found_in_mob_turns = true;
			l->mob_turns.erase(l->mob_turns.begin() + i);
		}
	}
	assert(found_in_mob_turns);
	delete m;
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

	generate_and_place_items(l);
	generate_and_place_mobs(l);
}

static void create_outer_wall_layer(Level *l) {
	// Ensure a 1-cell thick border of wall around the edge of the Dungeon.
	for (int x = 0; x < DUNGEON_WIDTH; x++) {
		l->cells[0][x] = Cell::rock;
		l->cells[DUNGEON_HEIGHT-1][x] = Cell::rock;
	}
	for (int y = 0; y < DUNGEON_HEIGHT; y++) {
		l->cells[y][0] = Cell::rock;
		l->cells[y][DUNGEON_WIDTH-1] = Cell::rock;
	}
}

static void level_fill_with(Level *l, const Cell c) {
	for (int y = 0; y < DUNGEON_HEIGHT; y++) {
		for (int x = 0; x < DUNGEON_WIDTH; x++) {
			l->cells[y][x] = c;
		}
	}
}

static void generate_dungeon_level(Level *l, int above_stair_x, int above_stair_y) {
	level_fill_with(l, Cell::rock);

	vector<int> room_x_positions;
	vector<int> room_y_positions;

	int num_rooms = RAND_BETWEEN(5, 8);
	for (int i = 0; i < num_rooms; i++) {
		int room_x = RAND_BETWEEN(4, DUNGEON_WIDTH-5);
		int room_y = RAND_BETWEEN(4, DUNGEON_HEIGHT-5);
		place_dungeon_room(l, room_x, room_y);

		room_x_positions.push_back(room_x);
		room_y_positions.push_back(room_y);
	}

	// Set up staircases.
	l->up_stair_x = above_stair_x;
	l->up_stair_y = above_stair_y;
	place_dungeon_room(l, l->up_stair_x, l->up_stair_y);
	room_x_positions.push_back(l->up_stair_x);
	room_y_positions.push_back(l->up_stair_y);

	if (l->depth == NUM_LEVELS - 1) {
		// we are the final floor
	} else {
		// Wrapped in a do-while just in case the staircases happen to
		// be generated on top of eachother.
		do {
			l->down_stair_x = RAND_BETWEEN(4, DUNGEON_WIDTH-5);
			l->down_stair_y = RAND_BETWEEN(4, DUNGEON_HEIGHT-5);
		} while (l->down_stair_x == l->up_stair_x &&
				l->down_stair_y == l->down_stair_y);

		place_dungeon_room(l, l->down_stair_x, l->down_stair_y);
		room_x_positions.push_back(l->down_stair_x);
		room_y_positions.push_back(l->down_stair_y);
	}

	l->cells[l->up_stair_y][l->up_stair_x] = Cell::stair_up;
	l->cells[l->down_stair_y][l->down_stair_x] = Cell::stair_down;

	assert(room_x_positions.size() == room_y_positions.size());

	for (int i = 0; i < room_x_positions.size() - 1; i++) {
		generate_tunnel_between(l, room_x_positions[i], room_y_positions[i],
				room_x_positions[i+1], room_y_positions[i+1]);
	}

	create_outer_wall_layer(l);

	generate_and_place_items(l);
	generate_and_place_mobs(l);
}

static void generate_boss_level(World *w) {
	// The boss level has no staircase up.
	// It contains only the boss and a series of minions.
	Level *l = &w->levels[BOSS_LEVEL];
	l->depth = BOSS_LEVEL;

	level_fill_with(l, Cell::lava);
	place_circle(l, Cell::none, DUNGEON_WIDTH / 2, DUNGEON_HEIGHT / 2, 8);

	w->boss = construct_jeremy(l);

	l->mobs[w->boss->y][w->boss->x] = w->boss;
	l->mob_turns.push_back(w->boss);
}

void world_update_mobs(World *w) {
	for (Mob *m : w->cur_level->mob_turns) {
		mob_move_ai(w, m);

		// no need to make more moves if the player is dead
		if (w->pc->hp <= 0) break;
	}
}

void world_init(World *w) {
	assert(w);

	memset(w, 0, sizeof(*w));
	memset(w->levels, 0, sizeof(w->levels));

	// Fixes a crash on certain platforms.
	// Must initialize the deque for whatever reason.
	w->messages = deque<Message>();

	generate_town(&w->levels[TOWN_LEVEL]);
	for (int i = 1; i < BOSS_LEVEL; i++) {
		w->levels[i].depth = i;
		generate_dungeon_level(
			&w->levels[i],
			w->levels[i - 1].down_stair_x,
			w->levels[i - 1].down_stair_y
		);
	}
	generate_boss_level(w);

	w->cur_level = &w->levels[PLAYER_START_LEVEL];

	w->pc = new Mob;
	pc_init(w->pc);
	w->pc->level = PLAYER_START_LEVEL;
	do {
		w->pc->x = RAND_BETWEEN(1, DUNGEON_WIDTH - 2);
		w->pc->y = RAND_BETWEEN(1, DUNGEON_HEIGHT - 2);
	} while (!level_location_clear(w->cur_level, w->pc->x, w->pc->y));

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

void world_destroy(World *w) {
	for (int i = 0; i < NUM_LEVELS; i++) {
		Level *l = &w->levels[i];

		for (int y = 0; y < DUNGEON_HEIGHT; y++) {
			for (int x = 0; x < DUNGEON_WIDTH; x++) {
				if (l->mobs[y][x]) {
					delete l->mobs[y][x];
					l->mobs[y][x] = nullptr;
				}
			}
		}

		l->mob_turns.clear();
	}

	w->pc = nullptr;
	w->boss = nullptr;
}

