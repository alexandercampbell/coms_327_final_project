
#pragma once

// no (non-stdlib) includes allowed :)

#define DUNGEON_WIDTH  120
#define DUNGEON_HEIGHT 80
#define DUNGEON_NUM_CELLS (DUNGEON_WIDTH * DUNGEON_HEIGHT)
#define MAX_MOBS 50
// 10 dungeon floors, 1 town
#define NUM_LEVELS 11

// Model definitions

struct Dice {
	int base;
	int num;
	int sides;
};

enum class WeaponType {
	dagger,
	sword,
	axe,
};

struct Weapon {
	char *name;
	char *desc;
	Dice damage;
};

struct Ring {
	char *name;
	char *desc;

	double damage_mul;
	double hp_mul;
};

enum class Race {
	human,
	elf,
	orc,
	dwarf,
	reptile
};

struct Mob {
	bool is_player;
	bool is_friendly;

	// World->levels[level]->mobs[y][x] is a pointer to this Mob.
	int x;
	int y;
	int level;

	int hp;
	int max_hp;

	char symb;
	Dice unarmed_attack;
	Weapon *weapon;
	Ring *ring;
	Race race;
};

enum class Cell {
	dirt,
	grass,
	tree,
	rock,
	tunnel,
	river,
	stair_up,
	stair_down,
};

struct Level {
	int depth;
	bool is_dungeon;

	Cell cells[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	Mob *mobs[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	int num_mobs;
};

struct World {
	Level levels[NUM_LEVELS];
	Level *cur_level; // &levels[pc->level]
	Mob *pc;
};

enum class Key {
	none,
	up,
	left,
	down,
	right,
	quit,
};

enum class Direction {
	up,
	left,
	down,
	right,
};

