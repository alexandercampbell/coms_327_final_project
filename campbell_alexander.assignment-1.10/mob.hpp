
#pragma once

#include <vector>
using namespace std;

#include "assert.h"
#include "string.h"

#include "model.hpp"
#include "util.hpp"
#include "world.hpp"

void mob_move_ai(World *w, Mob *mob);
bool mob_try_to_move(World *w, Mob *mob, Direction direction);
Mob *mob_generate(int depth);

// Sets up a final boss Mob with valid coordinates, but does not place the mob
// in the level.
Mob *construct_jeremy(Level *l);

