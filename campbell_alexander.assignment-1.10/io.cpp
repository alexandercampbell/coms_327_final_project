
#include "io.hpp"

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
	key result;
} key_bindings[] = {
	{KEY_LEFT, key::left},
	{KEY_DOWN, key::down},
	{KEY_UP, key::up},
	{KEY_RIGHT, key::right},
	{'q', key::quit},
	{0, key(0)},
};

key io_wait_for_key() {
	while (true) {
		int k = getch();
		for (int i = 0; key_bindings[i].ncurses_key != 0; i++) {
			if (key_bindings[i].ncurses_key == k) {
				return key_bindings[i].result;
			}
		}
	}
}

static void io_draw_with_color(char c, int x, int y, int color) {
	attron(COLOR_PAIR(color));
	mvaddch(y, x, c);
	attroff(COLOR_PAIR(color));
}

void io_render(dungeon *d) {
	clear();

	for (int x = 0; x < DUNGEON_WIDTH; x++) {
		for (int y = 0; y < DUNGEON_HEIGHT; y++) {
			mob *m = d->mobs[y][x];
			if (m) {
				io_draw_with_color(m->symb, x, y, m->is_friendly ?
						COLOR_GREEN : COLOR_RED);
				continue;
			}

			cell c = d->cells[y][x];
			char ch;
			int color;

			if (c == cell::grass)       { ch = ','; color = COLOR_YELLOW; }
			else if (c == cell::tree)   { ch = 'T'; color = COLOR_GREEN; }
			else if (c == cell::rock)   { ch = '#'; color = COLOR_BLACK; }
			else if (c == cell::tunnel) { ch = '.'; color = COLOR_BLUE; }
			else                       { ch = '?'; color = COLOR_MAGENTA; }

			io_draw_with_color(ch, x, y, color);
		}
	}

	refresh();
}

