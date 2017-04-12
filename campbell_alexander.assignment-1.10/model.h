
#pragma once

// no (non-stdlib) includes allowed :)

#define DUNGEON_WIDTH  200
#define DUNGEON_HEIGHT 100
#define DUNGEON_NUM_CELLS (DUNGEON_WIDTH * DUNGEON_HEIGHT)

// Model definitions

typedef enum {
	cell_grass = 0,
	cell_tree,
	cell_rock,
	cell_tunnel,
} cell;

typedef struct dungeon {
	cell cells[DUNGEON_HEIGHT][DUNGEON_WIDTH];
} dungeon;

typedef enum key_press {
	key_up,
	key_down,
	key_left,
	key_right,
} key_press;

