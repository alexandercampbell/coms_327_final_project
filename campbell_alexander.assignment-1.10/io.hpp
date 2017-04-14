
#pragma once

#include <ncurses.h>

#include "model.hpp"

namespace io {
	void init();
	void quit(); // idempotent

	void render(dungeon *d);

	key wait_for_key();
}

