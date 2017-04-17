
#pragma once

#include <vector>
using namespace std;

#include "assert.h"

#include "model.hpp"
#include "util.hpp"

bool mob_try_to_move(Level *level, Mob *mob, Direction direction);
Mob *mob_generate(int depth);

