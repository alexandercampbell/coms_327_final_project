
#include "pc.hpp"

void pc_init(Mob *pc) {
	memset(pc, 0, sizeof(*pc));

	pc->name = "Adventurer";
	pc->is_player = true;
	pc->is_friendly = true;
	pc->hp = 100;
	pc->max_hp = 100;
	pc->symb = '@';

	pc->unarmed_attack.base = 2; 
	pc->unarmed_attack.num = 2;  
	pc->unarmed_attack.sides = 1;
	pc->view_radius = 6;
}

static bool use_stairs(World *w, bool ascend) {
	Cell c = w->cur_level->cells[w->pc->y][w->pc->x];
	if (c == Cell::stair_up && ascend) {
		w->pc->level--;
		if (w->pc->level == 0) {
			world_push_message(w, "You emerge from the dungeon.",
					MessageSeverity::Warning);
			world_push_message(w, "The sunlight is blinding.",
					MessageSeverity::Warning);
		} else {
			world_push_message(w, "You climb the stairs, a few steps closer to the surface.",
					MessageSeverity::Warning);
		}
	} else if (c == Cell::stair_down && !ascend) {
		if (w->pc->level == TOWN_LEVEL) {
			world_push_message(w, "You descend into the dungeon.",
					MessageSeverity::Warning);
			world_push_message(w, "After a few steps, you can no longer see the sun.",
					MessageSeverity::Warning);
		} else if (w->pc->level == BOSS_LEVEL - 1) {
			for (int i = 0; i < 3; i++) {
				world_push_message(w, "...",
						MessageSeverity::OhGodTheresBloodEverywhere);
			}
			world_push_message(w, "You climb down into the pit.",
					MessageSeverity::OhGodTheresBloodEverywhere);
			world_push_message(w, "A sense of doom envelops you.",
					MessageSeverity::OhGodTheresBloodEverywhere);
			world_push_message(w, "A booming voice echos through the chamber.",
					MessageSeverity::OhGodTheresBloodEverywhere);
			world_push_message(w, "\"To win the game, you must kill me, Jeremy Sheaffer.\"",
					MessageSeverity::OhGodTheresBloodEverywhere);
		} else {
			world_push_message(w, "You descend a little deeper into the dungeon.",
					MessageSeverity::Warning);
			world_push_message(w, "Dungeon Floor " + to_string(w->pc->level + 2),
					MessageSeverity::Warning);
		}
		w->pc->level++;
	} else {
		return false;
	}

	w->cur_level->mobs[w->pc->y][w->pc->x] = nullptr;
	w->cur_level = &w->levels[w->pc->level];
	if (w->pc->level == BOSS_LEVEL) {
		// When descending into the boss level, the player always lands
		// in the center of the room.
		w->pc->y = DUNGEON_HEIGHT / 2;
		w->pc->x = DUNGEON_WIDTH / 2;
	}

	if (w->cur_level->mobs[w->pc->y][w->pc->x]) {
		// a monster is on the staircase
		// telefrag
		world_kill(w, w->cur_level->mobs[w->pc->y][w->pc->x], "telefrag");
	}
	w->cur_level->mobs[w->pc->y][w->pc->x] = w->pc;

	pc_update_memory(w);
	return true;
}

static bool trade_item(World *w) {
	Item *i = w->cur_level->items[w->pc->y][w->pc->x];
	if (!i) return false;

	Item **pc_item = (i->type == ItemType::weapon) ?
		&w->pc->weapon : &w->pc->ring;

	if (*pc_item) {
		string s = string("You equip ") + i->name + string(", dropping ") +
			(*pc_item)->name + ".";
		world_push_message(w, s);

		Item *temp = *pc_item;
		*pc_item = i;
		w->cur_level->items[w->pc->y][w->pc->x] = temp;
	} else {
		string s = string("You equip ") + i->name + ".";
		world_push_message(w, s);

		*pc_item = i;
		w->cur_level->items[w->pc->y][w->pc->x] = NULL;
	}

	world_push_message(w, (*pc_item)->desc.c_str(), MessageSeverity::Good);
	return true;
}

static bool use_ring(World *w) {
	if (!w->pc->ring) {
		world_push_message(w, "You have no ring equipped.");
		return false;
	}

	assert(w->pc->ring->type == ItemType::ring);

	world_push_message(w, "You activate the power contained in your ring.",
			MessageSeverity::Good);
	world_push_message(w, "It shatters into pieces and falls to the ground.",
			MessageSeverity::Good);

	if (w->pc->ring->ability == RingAbility::teleport_to_town) {
		w->cur_level->mobs[w->pc->y][w->pc->x] = nullptr;
		w->pc->level = TOWN_LEVEL;
		w->cur_level = &w->levels[w->pc->level];
		do {
			w->pc->x = RAND_BETWEEN(0, DUNGEON_WIDTH);
			w->pc->y = RAND_BETWEEN(0, DUNGEON_HEIGHT);
		} while (!level_location_clear(w->cur_level, w->pc->x, w->pc->y));
		w->cur_level->mobs[w->pc->y][w->pc->x] = w->pc;

		pc_update_memory(w);

		world_push_message(w, "In a flash of green light, you teleport to the surface.",
				MessageSeverity::Good);
	} else if (w->pc->ring->ability == RingAbility::sacrifice) {
		int initial_hp = w->pc->hp;
		w->pc->hp /= 2;
		w->pc->hp += 1;
		int damage_done = initial_hp - w->pc->hp;

		world_push_message(w, string("You feel a sudden sharp pain in your chest. (-") +
				to_string(damage_done) + string(" HP)"),
				MessageSeverity::OhGodTheresBloodEverywhere);

		for (Mob *m : w->cur_level->mob_turns) {
			if (m == w->pc) continue;
			m->hp /= 4;
			m->hp += 1;
		}

		world_push_message(w, "Every living creature on this floor has been badly injured.",
				MessageSeverity::OhGodTheresBloodEverywhere);
	} else if (w->pc->ring->ability == RingAbility::boost_hp) {
		world_push_message(w, "You immediately feel stronger. (+20 HP, +10 MAX HP)",
				MessageSeverity::Good);
		w->pc->max_hp += 10;
		w->pc->hp += 20;
	} else if (w->pc->ring->ability == RingAbility::reveal) {
		world_push_message(w, "You have a sudden sense of omniscience.",
				MessageSeverity::Good);

		for (int y = 0; y < DUNGEON_HEIGHT; y++) {
			for (int x = 0; x < DUNGEON_WIDTH; x++) {
				w->cur_level->pc_memory[y][x] =
					w->cur_level->cells[y][x];
			}
		}
	}

	w->pc->ring = nullptr;
	return true;
}

bool pc_process_key(World *w, Key k) {
	if (k == Key::ascend_stairs || k == Key::descend_stairs) {
		return use_stairs(w, k == Key::ascend_stairs);
	}

	if (k == Key::trade_item) {
		return trade_item(w);
	}

	if (k == Key::use_ring) {
		return use_ring(w);
	}

	Direction dir;
	if (k == Key::up) dir = Direction::up;
	else if (k == Key::left) dir = Direction::left;
	else if (k == Key::right) dir = Direction::right;
	else if (k == Key::down) dir = Direction::down;
	else return false;

	bool move_successful = mob_try_to_move(w, w->pc, dir);
	if (move_successful) {
		Cell *c = &w->cur_level->cells[w->pc->y][w->pc->x];
		if (*c == Cell::river) {
			if (FRAND() < 0.03) {
				world_push_message(w, "You see a minnow swim by.");
			}
		} else if (*c == Cell::health_fountain) {
			world_push_message(w, "You step onto the fountain of health and immediately feel refreshed.",
					MessageSeverity::Good);
			w->pc->hp += 40;
			if (w->pc->hp > w->pc->max_hp) {
				w->pc->hp = w->pc->max_hp;
			}
		}
		pc_update_memory(w);

		Item *i = w->cur_level->items[w->pc->y][w->pc->x];
		if (i) {
			string s = string("On the ground you see ") + i->name + ".";
			world_push_message(w, s.c_str());
			s = "(Trade for currently equipped ";
			s += (i->type == ItemType::weapon) ? "weapon" : "ring";
			s += " with [space].)";
			world_push_message(w, s.c_str());
		}
	}

	return move_successful;
}

void pc_update_memory(World *w) {
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

