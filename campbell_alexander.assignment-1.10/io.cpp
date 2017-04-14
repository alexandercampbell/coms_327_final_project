
#include "io.hpp"

void io::init() {
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

void io::quit() {
	endwin();
}

static struct {
	int ncurses_key;
	Key result;
} key_bindings[] = {
	{KEY_LEFT, Key::left},
	{KEY_DOWN, Key::down},
	{KEY_UP, Key::up},
	{KEY_RIGHT, Key::right},
	{'q', Key::quit},
	{0, Key(0)},
};

Key io::wait_for_key() {
	while (true) {
		int k = getch();
		for (int i = 0; key_bindings[i].ncurses_key != 0; i++) {
			if (key_bindings[i].ncurses_key == k) {
				return key_bindings[i].result;
			}
		}
	}
}

static void draw_with_color(char c, int x, int y, int color) {
	attron(COLOR_PAIR(color));
	mvaddch(y, x, c);
	attroff(COLOR_PAIR(color));
}

void io::render(World *w) {
	clear();

	int render_start_x = w->pc->x - RENDER_WIDTH / 2;
	int render_start_y = w->pc->y - RENDER_HEIGHT / 2;
	int render_stop_x = render_start_x + RENDER_WIDTH;
	int render_stop_y = render_start_y + RENDER_HEIGHT;

	for (int x = render_start_x; x < render_stop_x; x++) {
		if (x < 0 || x >= DUNGEON_WIDTH) continue;

		for (int y = render_start_y; y < render_stop_y; y++) {
			if (y < 0 || y >= DUNGEON_HEIGHT) continue;

			int draw_x = x - render_start_x;
			int draw_y = y - render_start_y;

			Mob *m = w->cur_level->mobs[y][x];
			if (m) {
				draw_with_color(m->symb, draw_x, draw_y, m->is_friendly ?
						COLOR_GREEN : COLOR_RED);
				continue;
			}

			Cell c = w->cur_level->cells[y][x];
			char ch;
			int color;

			if (c == Cell::grass)       { ch = ','; color = COLOR_YELLOW; }
			else if (c == Cell::tree)   { ch = 'T'; color = COLOR_GREEN; }
			else if (c == Cell::rock)   { ch = '#'; color = COLOR_BLACK; }
			else if (c == Cell::tunnel) { ch = '.'; color = COLOR_BLUE; }
			else if (c == Cell::river)  { ch = '~'; color = (FRAND() > 0.5) ? COLOR_BLUE : COLOR_WHITE; }
			else                        { ch = '?'; color = COLOR_MAGENTA; }

			draw_with_color(ch, draw_x, draw_y, color);
		}
	}

	refresh();
}

