#include "RenderEditor.h"
#include <ncurses.h>

void renderEditor(WINDOW *win)
{
    werase(win);
    wrefresh(win);

    int sidebar_width = COLS * 0.25;
    int content_width = COLS - sidebar_width;
    WINDOW *sidebar = derwin(win, LINES, sidebar_width, 0, 0);
    WINDOW *content = derwin(win, LINES, content_width, 0, sidebar_width);
    refresh();

    box(sidebar, 0, 0);
    mvwprintw(sidebar, 1, 2, "Menu");

    box(content, 0, 0);
    mvwprintw(content, 1, 2, "Content");

    wrefresh(win);
    wrefresh(sidebar);
    wrefresh(content);
}