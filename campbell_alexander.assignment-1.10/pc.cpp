
#include "pc.hpp"

void pc::init(Mob *pc) {
	memset(pc, 0, sizeof(*pc));

	pc->is_player = true;
	pc->is_friendly = true;
	pc->hp = 100;
	pc->max_hp = 100;
	pc->symb = '@';
	pc->race = Race::human;

	pc->unarmed_attack.base = 3; 
	pc->unarmed_attack.num = 2;  
	pc->unarmed_attack.sides = 6;
}

static bool use_stairs(World *w, bool ascend) {
	Cell c = w->cur_level->cells[w->pc->y][w->pc->x];
	if (c == Cell::stair_up && ascend) {
		w->pc->level--;
	} else if (c == Cell::stair_down && !ascend) {
		w->pc->level++;
	} else {
		return false;
	}

	w->cur_level->mobs[w->pc->y][w->pc->x] = nullptr;
	w->cur_level = &w->levels[w->pc->level];
	w->cur_level->mobs[w->pc->y][w->pc->x] = w->pc;

	pc::update_memory(w);
	return true;
}

bool pc::process_key(World *w, Key k) {
	if (k == Key::ascend_stairs || k == Key::descend_stairs) {
		return use_stairs(w, k == Key::ascend_stairs);
	}

	Direction dir;
	if (k == Key::up) dir = Direction::up;
	else if (k == Key::left) dir = Direction::left;
	else if (k == Key::right) dir = Direction::right;
	else if (k == Key::down) dir = Direction::down;
	else return false;

	bool move_successful = mob_try_to_move(w->cur_level, w->pc, dir);
	if (move_successful) {
		pc::update_memory(w);
	}

	return move_successful;
}

void pc::update_memory(World *w) {
	Mob *pc = w->pc;

	// Can't see as far in the dungeon.
	int radius = PC_VIEW_RADIUS(w);

	for (int y = pc->y - radius; y <= pc->y + radius; y++) {
		if (y < 0 || y >= DUNGEON_HEIGHT) continue;
		for (int x = pc->x - radius; x <= pc->x + radius; x++) {
			if (x < 0 || x >= DUNGEON_WIDTH) continue;

			int dist_x = x - pc->x;
			int dist_y = y - pc->y;
			if (sqrt(dist_x * dist_x + dist_y * dist_y) <= radius) {
				w->cur_level->pc_memory[y][x] = w->cur_level->cells[y][x];
			}
		}
	}
}

