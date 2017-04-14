
#include "pc.hpp"

bool pc::process_key(Dungeon &d, Key k) {
	Direction dir;
	if (k == Key::up) dir = Direction::up;
	else if (k == Key::left) dir = Direction::left;
	else if (k == Key::right) dir = Direction::right;
	else if (k == Key::down) dir = Direction::down;
	else return false;

	return mob_try_to_move(d, d.pc, dir);
}

