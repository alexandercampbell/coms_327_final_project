
#pragma once

#include <assert.h>
#include <string.h>
#include <math.h>

#include "model.hpp"
#include "mob.hpp"
#include "world.hpp"

void pc_init(Mob *pc);
bool pc_process_key(World *w, Key k);
void pc_update_memory(World *w);

