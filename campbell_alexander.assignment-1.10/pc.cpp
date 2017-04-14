
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

bool pc::process_key(World *w, Key k) {
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
	for (int y = pc->y - PC_VIEW_RADIUS; y <= pc->y + PC_VIEW_RADIUS; y++) {
		if (y < 0 || y >= DUNGEON_HEIGHT) continue;
		for (int x = pc->x - PC_VIEW_RADIUS; x <= pc->x + PC_VIEW_RADIUS; x++) {
			if (x < 0 || x >= DUNGEON_WIDTH) continue;

			int dist_x = x - pc->x;
			int dist_y = y - pc->y;
			if (sqrt(dist_x * dist_x + dist_y * dist_y) <= PC_VIEW_RADIUS) {
				w->cur_level->pc_memory[y][x] = w->cur_level->cells[y][x];
			}
		}
	}
}

