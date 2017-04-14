
#pragma once

#include <ncurses.h>

#include "model.hpp"
#include "util.hpp"

namespace io {
	void init();
	void quit(); // idempotent

	void render(Dungeon *d);

	Key wait_for_key();
}

