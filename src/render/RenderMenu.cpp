#include "RenderMenu.h"
#include <ncurses.h>

void renderMenu(WINDOW *win, int menu_option)
{
    clear();

    // box(win, 0, 0); //this for border if we want
    mvwprintw(win, (LINES - 11) / 2, (COLS - 8) / 2, "neonote.");

    if (menu_option == 0)
    {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, (LINES - 2) / 2, (COLS - 8) / 2, "my notes");
        wattroff(win, COLOR_PAIR(1));
        mvwprintw(win, (LINES - 1) / 2, (COLS - 4) / 2, "exit");
    }
    else
    {
        mvwprintw(win, (LINES - 2) / 2, (COLS - 8) / 2, "my notes");
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, (LINES - 1) / 2, (COLS - 4) / 2, "exit");
        wattroff(win, COLOR_PAIR(1));
    }

    refresh();
}