
#pragma once

#include <ncurses.h>

#include "model.hpp"
#include "util.hpp"

namespace io {
	const int RENDER_WIDTH = 80;
	const int RENDER_HEIGHT = 24;

	void init();
	void quit(); // idempotent

	void render(Dungeon *d);

	Key wait_for_key();
}

