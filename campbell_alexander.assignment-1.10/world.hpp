
#pragma once

#include <vector>
using namespace std;

#include <string.h>
#include <assert.h>

#include "model.hpp"
#include "util.hpp"
#include "pc.hpp"
#include "item.hpp"

void world_init(World *w);
void world_push_message(World *w, string text,
		MessageSeverity severity = MessageSeverity::Info);
void world_update_mobs(World *w);
void world_destroy(World *w);

// Return true if the location is walkable, free of items and mobs, and within
// the bounds of the level.
bool level_location_clear(Level *l, int x, int y);

