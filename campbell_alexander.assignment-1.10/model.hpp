
#pragma once

// no (non-stdlib) includes allowed :)

#define DUNGEON_WIDTH  120
#define DUNGEON_HEIGHT 80
#define DUNGEON_NUM_CELLS (DUNGEON_WIDTH * DUNGEON_HEIGHT)
#define MAX_MOBS 50

// Model definitions

struct dice {
	int base;
	int num;
	int sides;
};

enum class weapon_type {
	dagger,
	sword,
	axe,
};

struct weapon {
	char *name;
	char *desc;
	dice damage;
};

struct ring {
	char *name;
	char *desc;

	double damage_mul;
	double hp_mul;
};

enum class race {
	race_human,
	race_elf,
	race_orc,
	race_dwarf,
	race_reptile
};

struct mob {
	bool is_player;
	bool is_friendly;

	int hp;
	int max_hp;

	char symb;
	dice unarmed_attack;
	weapon *weapon;
	ring *ring;
	race race;
};

enum class cell {
	grass,
	tree,
	rock,
	tunnel,
	river,
};

struct dungeon {
	cell cells[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	mob *mobs[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	int num_mobs;
};

enum class key {
	none,
	up,
	left,
	down,
	right,
	quit,
};

