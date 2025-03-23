#include "RenderEditor.h"
#include <ncurses.h>
#include <vector>
#include <string>
using namespace std;

void renderEditor(WINDOW *win, vector<string> &files)
{
    werase(win);
    wrefresh(win);

    int sidebar_width = COLS * 0.25;
    int content_width = COLS - sidebar_width;
    WINDOW *sidebar = derwin(win, LINES, sidebar_width, 0, 0);
    WINDOW *content = derwin(win, LINES, content_width, 0, sidebar_width);
    refresh();

    // CALENDAR DIV

    box(sidebar, 0, 0);
    mvwprintw(sidebar, 2, 2, "My Tasks");
    mvwprintw(sidebar, 3, 2, "Calendar");
    mvwhline(win, 5, 1, ACS_HLINE, sidebar_width - 2);

    for (int i = 0; i < files.size(); i++)
    {
        mvwprintw(sidebar, 7 + i, 2, files[i].c_str());
    }

    // CONTENT DIV

    box(content, 0, 0);
    mvwprintw(content, 1, 2, "Content");

    wrefresh(win);
    wrefresh(sidebar);
    wrefresh(content);
}