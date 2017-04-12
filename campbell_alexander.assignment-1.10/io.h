
#pragma once

#include <ncurses.h>

#include "model.h"

void io_init();
void io_quit(); // idempotent

key_press io_wait_for_key();

