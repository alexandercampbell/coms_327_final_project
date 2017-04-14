
#pragma once

#include <assert.h>
#include <string.h>
#include <math.h>

#include "model.hpp"
#include "mob.hpp"

namespace pc {
	void init(Mob *pc);
	bool process_key(World *w, Key k);
	void update_memory(World *w);
};

