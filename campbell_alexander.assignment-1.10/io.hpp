
#pragma once

#include <ncurses.h>

#include "model.hpp"

void io_init();
void io_quit(); // idempotent

void io_render(dungeon *d);

key io_wait_for_key();

