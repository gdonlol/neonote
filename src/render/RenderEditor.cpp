#include "RenderEditor.h"
#include <ncurses.h>

void renderEditor(WINDOW *win)
{
    mvwprintw(win, 0, 0, "editor screen");

    refresh();
}