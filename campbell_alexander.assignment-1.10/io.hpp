
#pragma once

#include <ncurses.h>
#include <assert.h>

#include "world.hpp"
#include "model.hpp"
#include "util.hpp"

const int RENDER_WIDTH = 80;
const int RENDER_HEIGHT = 32;

void io_init();
void io_quit(); // idempotent

// return true if the player decides to play
bool io_main_menu(World *w);

void io_render(World *w);

Key io_wait_for_key();

