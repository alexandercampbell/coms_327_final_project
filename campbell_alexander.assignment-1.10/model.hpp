
#pragma once

// no (non-stdlib) includes allowed :)

#define DUNGEON_WIDTH  120
#define DUNGEON_HEIGHT 80
#define DUNGEON_NUM_CELLS (DUNGEON_WIDTH * DUNGEON_HEIGHT)
#define MAX_MOBS 50

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

	int x;
	int y;

	int hp;
	int max_hp;

	char symb;
	Dice unarmed_attack;
	Weapon *weapon;
	Ring *ring;
	Race race;
};

enum class Cell {
	grass,
	tree,
	rock,
	tunnel,
	river,
};

struct Dungeon {
	Cell cells[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	Mob *mobs[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	Mob *pc;
	int num_mobs;
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

