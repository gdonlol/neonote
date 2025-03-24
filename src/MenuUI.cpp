#include "MenuUI.h"

MenuUI::MenuUI() : menuOption(0) {}

void MenuUI::render(WINDOW* win) {
    werase(win);
    mvwprintw(win, (LINES - 11) / 2, (COLS - 8) / 2, "neonote.");
    
    int rowPrint = (LINES - 2) / 2;
    
    if (menuOption == 0) {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, rowPrint, (COLS - 8) / 2, "my notes");
        wattroff(win, COLOR_PAIR(1));
        mvwprintw(win, rowPrint + 1, (COLS - 4) / 2, "exit");
    } else {
        mvwprintw(win, rowPrint, (COLS - 8) / 2, "my notes");
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, rowPrint + 1, (COLS - 4) / 2, "exit");
        wattroff(win, COLOR_PAIR(1));
    }
    
    wrefresh(win);
}

int MenuUI::handleInput(int ch) {
    if (ch == KEY_UP || ch == 'k') {
        menuOption = (menuOption - 1) % 2;
        if (menuOption < 0) menuOption = 1;
    } else if (ch == KEY_DOWN || ch == 'j') {
        menuOption = (menuOption + 1) % 2;
    } else if (ch == KEY_ENTER || ch == '\n') {
        return menuOption; // 0 = my notes, 1 = exit
    }
    return -1; // No selection made
}
