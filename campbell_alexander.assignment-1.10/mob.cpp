
#include "mob.hpp"

static int roll_dice(Dice d) {
	int total = d.base;
	for (int i = 0; i < d.num; i++) total += RAND_BETWEEN(1, d.sides);
	return total;
}

void mob_do_combat(World *w, Mob *atk, Mob *def) {
	Dice *atk_dice = atk->weapon ? &atk->weapon->damage : &atk->unarmed_attack;
	int damage = roll_dice(*atk_dice);
	def->hp -= damage;

	if (atk == w->pc) {
		string s = "You smite ";
		s += def->name;
		s += " for ";
		s += to_string(damage);
		s += " HP!";
		world_push_message(w, s, MessageSeverity::OhGodTheresBloodEverywhere);
	}

	if (def == w->pc) {
		string s = "You take ";
		s += to_string(damage);
		s += " damage from ";
		s += atk->name;
		s += ".";
		world_push_message(w, s, MessageSeverity::OhGodTheresBloodEverywhere);
	}

	if (def->hp <= 0) {
		world_kill(w, def, atk->name);
	}
}

bool mob_try_to_move(World *w, Mob *mob, Direction direction) {
	Level *l = w->cur_level;
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

	if (!CELL_IS_WALKABLE(l->cells[new_y][new_x])) return false;
	if (l->mobs[new_y][new_x]) {
		// another mob is already occupying this position
		if (l->mobs[new_y][new_x]->is_friendly != mob->is_friendly) {
			mob_do_combat(w, mob, l->mobs[new_y][new_x]);
			return true;
		}
		return false;
	}

	l->mobs[mob->y][mob->x] = nullptr;
	mob->x = new_x;
	mob->y = new_y;
	l->mobs[mob->y][mob->x] = mob;

	return true;
}

struct MobPrototype {
	int min_level;
	int max_level;

	Mob mob;
};

static MobPrototype mk_mob(int min_level, int max_level, string name, bool
		is_friendly, int hp, char symb, Dice unarmed_attack) {

	Mob m = {0};
	m.name = name;
	m.is_friendly = is_friendly;
	m.max_hp = m.hp = hp;
	m.symb = symb;
	m.unarmed_attack = unarmed_attack;

	MobPrototype p;
	p.min_level = min_level;
	p.max_level = max_level;
	p.mob = m;

	return p;
}

const static vector<MobPrototype> available_mobs = {
	mk_mob(0, 0, "a friendly rabbit", true, 10, 'r', mk_dice(0, 1, 1)),
	mk_mob(0, 0, "a deer", true, 25, 'd', mk_dice(0, 1, 4)),
	mk_mob(1, 7, "a kobold", false, 25, 'k', mk_dice(4, 1, 4)),
	mk_mob(1, 2, "a rat", false, 6, 'r', mk_dice(2, 1, 2)),
	mk_mob(0, 3, "a snake", false, 10, '~', mk_dice(3, 1, 2)),
	mk_mob(3, 6, "an evil gnome", false, 12, 'g', mk_dice(6, 1, 2)),
	mk_mob(6, 8, "a dragon", false, 120, 'D', mk_dice(6, 2, 4)),
	mk_mob(7, 9, "a vampire", false, 80, 'V', mk_dice(0, 3, 9)),
	mk_mob(10, 10, "a tomb lord", false, 80, 'T', mk_dice(7, 2, 10)),
};

Mob *mob_generate(int depth) {
	for (int attempts = 0; attempts < 150; attempts++) {
		int index = RAND_BETWEEN(0, available_mobs.size() - 1);
		const MobPrototype *p = &available_mobs[index];

		if (depth >= p->min_level && depth <= p->max_level) {
			Mob *m = new Mob;
			*m = p->mob; // copy
			return m;
		}
	}

	return NULL;
}

void mob_move_ai(World *w, Mob *mob) {
	if (mob->is_friendly) {
		if (FRAND() < 0.6) return;

		Direction dir = (Direction) RAND_BETWEEN(0, 3); // yuck :)
		mob_try_to_move(w, mob, dir);
		return;
	}

	int dist_x = w->pc->x - mob->x;
	int dist_y = w->pc->y - mob->y;
	int squared_distance = dist_x * dist_x + dist_y * dist_y;

	Direction dir;
	if (squared_distance > MONSTER_VIEW_RADIUS * MONSTER_VIEW_RADIUS) {
		// Small chance of moving randomly, even when we
		// can see the player.
		if (FRAND() < 0.93) return;
		dir = (Direction) RAND_BETWEEN(0, 3); // yuck :)
	} else {
		if (abs(dist_x) > abs(dist_y)) {
			dir = dist_x < 0 ? Direction::left : Direction::right;
		} else {
			dir = dist_y < 0 ? Direction::up : Direction::down;
		}
	}

	mob_try_to_move(w, mob, dir);
}

Mob *construct_jeremy(Level *l) {
	// Jeremy lives in the BOSS_LEVEL, and in the BOSS_LEVEL alone.
	assert(l->depth = BOSS_LEVEL);

	Mob *m = new Mob;
	memset(m, 0, sizeof(*m));
	m->is_friendly = false;
	m->x = DUNGEON_WIDTH / 2;
	m->y = DUNGEON_HEIGHT / 2 - 3;
	m->name = BOSS_NAME;
	m->symb = 'J';
	m->unarmed_attack = mk_dice(2, 4, 8);
	m->level = BOSS_LEVEL;
	m->hp = m->max_hp = 128;
	return m;
}

