
#include "mob.hpp"

static bool cell_is_walkable(Cell c) {
	return c == Cell::none ||
		c == Cell::grass ||
		c == Cell::tunnel ||
		c == Cell::stair_up ||
		c == Cell::stair_down ||
		c == Cell::river;
}

bool mob_try_to_move(Level *l, Mob *mob, Direction direction) {
	assert(l->mobs[mob->y][mob->x] == mob);

	int delta_x = 0, delta_y = 0;
	if (direction == Direction::left) delta_x = -1;
	else if (direction == Direction::right) delta_x = 1;
	else if (direction == Direction::up) delta_y = -1;
	else if (direction == Direction::down) delta_y = 1;

	int new_x = mob->x + delta_x;
	int new_y = mob->y + delta_y;

	if (new_x < 0 || new_x >= DUNGEON_WIDTH) return false;
	if (new_y < 0 || new_y >= DUNGEON_HEIGHT) return false;

	if (!cell_is_walkable(l->cells[new_y][new_x])) return false;

	l->mobs[mob->y][mob->x] = nullptr;
	mob->x = new_x;
	mob->y = new_y;
	l->mobs[mob->y][mob->x] = mob;

	return true;
}

