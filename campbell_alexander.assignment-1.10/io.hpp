
#pragma once

#include <ncurses.h>

#include "model.hpp"
#include "util.hpp"

const int RENDER_WIDTH = 80;
const int RENDER_HEIGHT = 24;

void io_init();
void io_quit(); // idempotent

void io_render(World *w);

Key io_wait_for_key();

