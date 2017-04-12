
#include "io.h"

void io_init() {
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	set_escdelay(0);

	init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void io_quit() {
	endwin();
}

static struct {
	int ncurses_key;
	key_press result;
} key_bindings[] = {
	{KEY_LEFT, key_left},
	{KEY_DOWN, key_down},
	{KEY_UP, key_up},
	{KEY_RIGHT, key_right},
	{0, 0},
};

key_press io_wait_for_key() {
	while (true) {
		int k = getch();
		for (int i = 0; key_bindings[i].ncurses_key != 0; i++) {
			if (key_bindings[i].ncurses_key == k) {
				return key_bindings[i].result;
			}
		}
	}
}

