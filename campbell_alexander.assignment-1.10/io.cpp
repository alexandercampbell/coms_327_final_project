
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
	Key result;
} key_bindings[] = {
	{KEY_LEFT, Key::left},
	{KEY_DOWN, Key::down},
	{KEY_UP, Key::up},
	{KEY_RIGHT, Key::right},
	{'h', Key::left},
	{'j', Key::down},
	{'k', Key::up},
	{'l', Key::right},
	{'q', Key::quit},
	{'>', Key::descend_stairs},
	{'<', Key::ascend_stairs},
	{0, Key(0)},
};

Key io_wait_for_key() {
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

void io_render(World *w) {
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

			Cell c = w->cur_level->pc_memory[y][x];
			char ch;
			int color;

			if (c == Cell::none) continue;
			else if (c == Cell::grass)      { ch = ','; color = COLOR_GREEN; }
			else if (c == Cell::tree)       { ch = 'T'; color = COLOR_GREEN; }
			else if (c == Cell::rock)       { ch = '#'; color = COLOR_BLACK; }
			else if (c == Cell::tunnel)     { ch = '.'; color = COLOR_WHITE; }
			else if (c == Cell::river)      { ch = '~'; color = (FRAND() > 0.5) ? COLOR_BLUE : COLOR_WHITE; }
			else if (c == Cell::stair_up)   { ch = '<'; color = COLOR_WHITE; }
			else if (c == Cell::stair_down) { ch = '>'; color = COLOR_WHITE; }
			else if (c == Cell::mountain)   { ch = '^'; color = COLOR_WHITE; }
			else                            { ch = '?'; color = COLOR_MAGENTA; }

			int dist_x = w->pc->x - x;
			int dist_y = w->pc->y - y;
			int view_radius = PC_VIEW_RADIUS(w);
			bool directly_visible = dist_x * dist_x + dist_y * dist_y <
				view_radius * view_radius;
			if (directly_visible) {
				attron(A_BOLD);
			} else {
				color = COLOR_WHITE;
			}

			draw_with_color(ch, draw_x, draw_y, color);

			if (directly_visible) attroff(A_BOLD);
		}
	}

	int line = 0;
	int console_start_line = RENDER_HEIGHT - w->messages.size();
	for (Message &m : w->messages) {
		if (m.severity == MessageSeverity::Good) {
			attron(COLOR_PAIR(COLOR_GREEN));
		} else if (m.severity == MessageSeverity::Warning) {
			attron(COLOR_PAIR(COLOR_YELLOW));
		} else if (m.severity == MessageSeverity::OhGodTheresBloodEverywhere) {
			attron(COLOR_PAIR(COLOR_RED));
		}

		mvprintw(console_start_line + line, RENDER_WIDTH + 1, "%s", m.text.c_str());
		line++;

		if (m.severity == MessageSeverity::Good) {
			attroff(COLOR_PAIR(COLOR_GREEN));
		} else if (m.severity == MessageSeverity::Warning) {
			attroff(COLOR_PAIR(COLOR_YELLOW));
		} else if (m.severity == MessageSeverity::OhGodTheresBloodEverywhere) {
			attroff(COLOR_PAIR(COLOR_RED));
		}
	}

	refresh();
}

