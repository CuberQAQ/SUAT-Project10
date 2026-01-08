#include <ncurses.h>
#include "Gui.h"
#include <cstring>
#include <clocale>
using namespace std;

void Gui::init() {
    setlocale(LC_ALL, "");      /* Support Chinese characters */
    initscr();                  /* Start curses mode              */
    refresh();                  /* Print it on to the real screen */
    win = stdscr;
    refresh();
    curs_set(0);
    wrefresh(win);
    noecho();
    cbreak();
    keypad(win, true);
    nodelay(win, true);
    flushinp();
    
    if(has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);    // Enemy (Default/Gunboat)
        init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Player
        init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Items (Gold/Health)
        init_pair(4, COLOR_BLUE, COLOR_BLACK);   // Obstacles
        init_pair(5, COLOR_CYAN, COLOR_BLACK);   // Gunboat (Alternative)
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);// Cruiser / Missiles
        init_pair(7, COLOR_WHITE, COLOR_BLACK);  // Default
        init_pair(8, COLOR_RED, COLOR_BLACK);    // Destroyer (Use Red to distinguish from Items)
    }
}

int Gui::get() {
    int c = wgetch(win);
    flushinp();
    return c;
}

void Gui::paintat(size_t rw, size_t cl, char c) {
    mvwaddch(win, rw, cl, c);
    return;
}

void Gui::paintat(size_t rw, size_t cl, char c, int color) {
    wattron(win, COLOR_PAIR(color));
    mvwaddch(win, rw, cl, c);
    wattroff(win, COLOR_PAIR(color));
    return;
}

void Gui::printMsg(int row, int col, const char* prompt) {
    mvwprintw(win, row, col, prompt);
}

void Gui::clear() {
    werase(win);
}

void Gui::redraw() {
    wrefresh(win);
}
