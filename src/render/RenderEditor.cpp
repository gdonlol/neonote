#include "RenderEditor.h"
#include <ncurses.h>

void renderEditor(WINDOW *win)
{
    werase(win);
    wrefresh(win);

    int sidebar_width = COLS * 0.3;
    int content_width = COLS - sidebar_width;
    WINDOW *sidebar = derwin(win, LINES - 2, sidebar_width - 2, 1, 1);
    WINDOW *content = derwin(win, LINES - 2, content_width - 2, 1, sidebar_width);
    refresh();

    box(sidebar, 0, 0);
    mvwprintw(sidebar, 1, 1, "Menu");

    box(content, 0, 0);
    mvwprintw(content, 1, 1, "Content");

    wrefresh(win);
    wrefresh(sidebar);
    wrefresh(content);
}