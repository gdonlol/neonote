#include "NcursesSetup.h"

NcursesSetup::NcursesSetup() {}

NcursesSetup::~NcursesSetup() {
    cleanup();
}

bool NcursesSetup::initialize() {
    // ncurses setup
    initscr();
    cbreak();
    noecho();
    raw();
    use_default_colors();
    keypad(stdscr, TRUE);
    intrflush(stdscr, FALSE);

    start_color();
    init_color(COLOR_BLACK, 150, 150, 150);
    init_pair(1, COLOR_BLACK, COLOR_WHITE); // Inverted highlight
    init_pair(2, COLOR_RED, -1);    // H1
    init_pair(3, COLOR_GREEN, -1);  // H2
    init_pair(4, COLOR_YELLOW, -1); // H3
    init_pair(5, COLOR_BLUE, -1);   // H4
    init_pair(6, COLOR_MAGENTA, -1);// H5
    init_pair(7, COLOR_CYAN, -1);   // H6
    init_pair(9, COLOR_WHITE, COLOR_BLACK); // Codeblock

    if (!has_colors()) {
        printw("Terminal does not support color");
        getch();
        endwin();
        return false;
    }

    return true;
}

void NcursesSetup::cleanup() {
    endwin();
}
