
#pragma once

#include <stdbool.h>

// no (non-stdlib) includes allowed :)

#define DUNGEON_WIDTH  120
#define DUNGEON_HEIGHT 80
#define DUNGEON_NUM_CELLS (DUNGEON_WIDTH * DUNGEON_HEIGHT)
#define MAX_MOBS 50

// Model definitions

typedef struct dice {
	int base;
	int num;
	int sides;
} dice;

typedef enum {
	dagger,
	sword,
	axe,
} weapon_type;

typedef struct weapon {
	char *name;
	char *desc;
	dice damage;
} weapon;

typedef struct ring {
	char *name;
	char *desc;

	double damage_mul;
	double hp_mul;
} ring;

typedef enum {
	race_human,
	race_elf,
	race_orc,
	race_dwarf,
	race_reptile
} race;

typedef struct mob {
	bool is_player;
	bool is_friendly;

	int hp;
	int max_hp;

	dice unarmed_attack;
	weapon *weapon;
	ring *ring;

	race race;
} mob;

typedef enum {
	cell_grass,
	cell_tree,
	cell_rock,
	cell_tunnel,
} cell;

typedef struct dungeon {
	cell cells[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	int num_mobs;
	mob mobs[MAX_MOBS];
} dungeon;

typedef enum key_press {
	key_up,
	key_down,
	key_left,
	key_right,
} key_press;

