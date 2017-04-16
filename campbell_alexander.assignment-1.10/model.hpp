
#pragma once

#include <deque>
#include <string>
using namespace std;

// no (non-stdlib) includes allowed :)

#define DUNGEON_WIDTH  92
#define DUNGEON_HEIGHT 64
#define DUNGEON_NUM_CELLS (DUNGEON_WIDTH * DUNGEON_HEIGHT)
#define MAX_MOBS 50
// 10 dungeon floors, 1 town
#define NUM_LEVELS (1 + 10)

// Number of messages kept in memory at one time. When the UI message queue
// fills, we will evict old messages.
#define WORLD_UI_MESSAGE_HISTORY 20

/**
 * Macros
 */

// PC_VIEW_RADIUS increases when we are above ground (i.e. in town).
#define PC_VIEW_RADIUS(world) ((world->pc->level == 0) ? 12 : 6)

/**
 * Model definitions.
 */

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
	none,
	grass,
	tree,
	rock,
	tunnel,
	river,
	mountain,
	stair_up,
	stair_down,
};

struct Level {
	int depth;
	bool is_dungeon;

	int down_stair_x, down_stair_y;
	int up_stair_x, up_stair_y; // may be -1

	Cell cells[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	Cell pc_memory[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	Mob *mobs[DUNGEON_HEIGHT][DUNGEON_WIDTH];
};

/**
 * MessageSeverity will be used by the IO module to distinguish messages with
 * color and bolding.
 */
enum class MessageSeverity {
	Info,
	Good,
	Warning,
	OhGodTheresBloodEverywhere,
};

struct Message {
	string text;
	MessageSeverity severity;
};

struct World {
	Level levels[NUM_LEVELS];
	Level *cur_level; // &levels[pc->level]
	Mob *pc;

	deque<Message> messages;
};

enum class Key {
	none,
	up,
	left,
	down,
	right,
	quit,
	ascend_stairs,
	descend_stairs
};

enum class Direction {
	up,
	left,
	down,
	right,
};

