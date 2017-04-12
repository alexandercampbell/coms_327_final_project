
#pragma once

#include <ncurses.h>

#include "model.h"

void io_init();
void io_quit(); // idempotent

void io_render(dungeon *d);

key_press io_wait_for_key();

