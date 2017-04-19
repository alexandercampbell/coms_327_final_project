
#pragma once

#include <deque>
#include <string>
#include <vector>
using namespace std;

// no (non-stdlib) includes allowed :)

#define DUNGEON_WIDTH  92
#define DUNGEON_HEIGHT 64
#define DUNGEON_NUM_CELLS (DUNGEON_WIDTH * DUNGEON_HEIGHT)
#define MAX_MOBS 50
// 1 town level, 4 dungeon levels, 1 final boss level
#define NUM_LEVELS (1 + 4 + 1)
#define TOWN_LEVEL 0
#define BOSS_LEVEL (NUM_LEVELS - 1)
#define BOSS_NAME "Professor Sheaffer"
#define PLAYER_START_LEVEL TOWN_LEVEL

// Number of messages kept in memory at one time. When the UI message queue
// fills, we will evict old messages.
#define WORLD_UI_MESSAGE_HISTORY 18

/**
 * Macros
 */

// PC_VIEW_RADIUS increases when we are above ground (i.e. in town).
#define PC_VIEW_RADIUS(world) ((world->pc->level == 0) ? \
		world->pc->view_radius + 6 : world->pc->view_radius)

#define MONSTER_VIEW_RADIUS 6

/**
 * Model definitions.
 */

struct Dice {
	int base;
	int num;
	int sides;
};
inline Dice mk_dice(int base, int num, int sides) {
	Dice d = {base, num, sides};
	return d;
}

enum class ItemType {
	weapon,
	ring,
};

enum class RingAbility {
	none,
	invisibility,
	teleport_to_town,
	sacrifice,
};

struct Item {
	string name;
	string desc;
	char symb;

	ItemType type;

	// attributes
	Dice damage;
	float damage_mul;
	float hp_mul;

	// if ring
	RingAbility ability;
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

	string name;
	char symb;
	Dice unarmed_attack;
	Item *weapon;
	Item *ring;
	int view_radius;
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
	health_fountain,
};
#define CELL_IS_WALKABLE(c) (\
	(c) == Cell::none             || \
	(c) == Cell::grass            || \
	(c) == Cell::tunnel           || \
	(c) == Cell::stair_up         || \
	(c) == Cell::stair_down       || \
	(c) == Cell::river            || \
	(c) == Cell::health_fountain     \
)

struct Level {
	int depth;
	bool is_dungeon;

	int down_stair_x, down_stair_y;
	int up_stair_x, up_stair_y; // may be -1

	Cell cells[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	Cell pc_memory[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	Mob *mobs[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	Item *items[DUNGEON_HEIGHT][DUNGEON_WIDTH];

	int turn;
	vector<Mob*> mob_turns;
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
	Mob *boss;

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
	descend_stairs,
	trade_item,
};

enum class Direction {
	up,
	left,
	down,
	right,
};

