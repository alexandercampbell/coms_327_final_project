
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

bool pc::process_key(World &w, Key k) {
	Direction dir;
	if (k == Key::up) dir = Direction::up;
	else if (k == Key::left) dir = Direction::left;
	else if (k == Key::right) dir = Direction::right;
	else if (k == Key::down) dir = Direction::down;
	else return false;

	return mob_try_to_move(w.cur_level, w.pc, dir);
}

