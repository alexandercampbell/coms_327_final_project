
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
	{' ', Key::trade_item},
	{'r', Key::use_ring},
	{KEY_ENTER, Key::trade_item},
	{'\n', Key::trade_item},
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

static string dice_to_string(Dice *d) {
	return to_string(d->base) + "+" + to_string(d->num) +
		"d" + to_string(d->sides);
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

			int dist_x = w->pc->x - x;
			int dist_y = w->pc->y - y;
			int view_radius = PC_VIEW_RADIUS(w);
			bool directly_visible = dist_x * dist_x + dist_y * dist_y <
				view_radius * view_radius;

			if (directly_visible) {
				attron(A_BOLD);

				Mob *m = w->cur_level->mobs[y][x];
				if (m) {
					draw_with_color(m->symb, draw_x, draw_y, m->is_friendly ?
							COLOR_BLUE : COLOR_RED);
					continue;
				}

				Item *item = w->cur_level->items[y][x];
				if (item) {
					draw_with_color(item->symb, draw_x, draw_y, COLOR_YELLOW);
					continue;
				}
			} else {
				attroff(A_BOLD);
			}

			Cell c = w->cur_level->pc_memory[y][x];
			char ch;
			int color;

			if (c == Cell::none) continue;
			else if (c == Cell::grass)           { ch = ','; color = COLOR_GREEN; }
			else if (c == Cell::tree)            { ch = 'T'; color = COLOR_GREEN; }
			else if (c == Cell::rock)            { ch = '#'; color = COLOR_BLACK; }
			else if (c == Cell::tunnel)          { ch = '.'; color = COLOR_WHITE; }
			else if (c == Cell::river)           { ch = '~'; color = (FRAND() > 0.5) ? COLOR_BLUE : COLOR_WHITE; }
			else if (c == Cell::lava)            { ch = '~'; color = (FRAND() > 0.5) ? COLOR_RED : COLOR_YELLOW; }
			else if (c == Cell::stair_up)        { ch = '<'; color = COLOR_WHITE; }
			else if (c == Cell::stair_down)      { ch = '>'; color = COLOR_WHITE; }
			else if (c == Cell::mountain)        { ch = '^'; color = COLOR_WHITE; }
			else if (c == Cell::health_fountain) { ch = 'H'; color = COLOR_GREEN; }
			else                                 { ch = '?'; color = COLOR_MAGENTA; }

			if (!directly_visible) {
				color = COLOR_WHITE;
			}

			draw_with_color(ch, draw_x, draw_y, color);
		}
	}

	attroff(A_BOLD);

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

	string location = "Floor " + to_string(w->pc->level) + "/" + to_string(BOSS_LEVEL);
	if (w->pc->level == TOWN_LEVEL) {
		location = "Town";
	} else if (w->pc->level == BOSS_LEVEL) {
		location = "BOSS";
	}

	mvprintw(1, RENDER_WIDTH + 1, "%s", location.c_str());

	attron(COLOR_PAIR(COLOR_YELLOW));
	mvprintw(2, RENDER_WIDTH + 1, "HP:       %2d/%d", w->pc->hp, w->pc->max_hp);
	mvprintw(3, RENDER_WIDTH + 1, "Ring:     %s", (w->pc->ring) ?
			w->pc->ring->name.c_str() : "none");

	string weapon_str = "fists " + dice_to_string(&w->pc->unarmed_attack);
	if (w->pc->weapon) {
		weapon_str = w->pc->weapon->name + string(" ") +
			dice_to_string(&w->pc->weapon->damage);
	}
	mvprintw(4, RENDER_WIDTH + 1, "Wielding: %s", weapon_str.c_str());
	attroff(COLOR_PAIR(COLOR_YELLOW));


	attron(COLOR_PAIR(COLOR_WHITE));
	mvprintw(RENDER_HEIGHT, 0,
	"                   MOVE            USE RING           TAKE");
	mvprintw(RENDER_HEIGHT+1, 0,
	"               arrows / hjkl          r               space");
	attroff(COLOR_PAIR(COLOR_WHITE));

	move(RENDER_HEIGHT / 2, RENDER_WIDTH / 2);

	refresh();
}

bool io_main_menu(World *w) {
	string right_justify = string("%") + to_string(TERMINAL_WIDTH) + "s";

	int menu_index = 0;
	const vector<string> menu_items = {
		string("Play as Human             (hp up)"),
		string("Play as Dwarf      (find 2x loot)"),
		string("Play as Elf         (see farther)"),
		string("                           (quit)"),
	};

	while (true) {
		clear();

		attron(COLOR_PAIR(COLOR_GREEN));
		attron(A_BOLD);
		mvprintw(8, 3, "Rogue Hunter");
		attroff(A_BOLD);
		mvprintw(9, 3, "v1.0.0");
		attroff(COLOR_PAIR(COLOR_GREEN));

		attron(COLOR_PAIR(COLOR_WHITE));
		for (int i = 0; i < menu_items.size(); i++) {
			string fmt_str;
			if (menu_index == i) {
				attron(A_BOLD);
				fmt_str = "       [ %s ] ";
			} else {
				fmt_str = "         %s   ";
			}
			mvprintw(8 + i, TERMINAL_WIDTH - 45, fmt_str.c_str(), menu_items[i].c_str());
			if (menu_index == i) attroff(A_BOLD);
		}
		attroff(COLOR_PAIR(COLOR_MAGENTA));

		attron(COLOR_PAIR(COLOR_CYAN));
		mvprintw(TERMINAL_HEIGHT - 2, 0, right_justify.c_str(),
				"COM S 327 SPRING 2017 (Prof. Jeremy Sheaffer)");
		mvprintw(TERMINAL_HEIGHT - 1, 0, right_justify.c_str(),
				"Final Project :: Alexander Campbell");
		attroff(COLOR_PAIR(COLOR_CYAN));

		refresh();

		Key k = io_wait_for_key();
		if (k == Key::quit) return false;
		if (k == Key::trade_item) break;

		if (k == Key::down) {
			menu_index++;
			menu_index %= menu_items.size();
		}

		if (k == Key::up) {
			menu_index--;
			menu_index %= menu_items.size();
		}
	}

	if (menu_index == 0) {
		w->pc->max_hp = w->pc->hp = w->pc->hp + 35;
	} else if (menu_index == 1) {
		world_increase_loot(w);
	} else if (menu_index == 2) {
		w->pc->view_radius += 4;
	} else if (menu_index == 3) {
		return false;
	}

	return true;
}

