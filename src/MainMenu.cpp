#include "MainMenu.h"
#include <string>
#include <cstring>

MainMenu::MainMenu(WINDOW* win) 
    : win_(win), currentWindow_(0), menuOption_(0), exitRequested_(false) {}

void MainMenu::display() {
    if (currentWindow_ == 0) {
        drawMenu();
    }
}

void MainMenu::handleInput(int input) {
    if (currentWindow_ == 0) {
        if (input == KEY_UP || input == 'k') {
            menuOption_ = (menuOption_ - 1) % 2;
            if (menuOption_ < 0) menuOption_ = 1;
        } 
        else if (input == KEY_DOWN || input == 'j') {
            menuOption_ = (menuOption_ + 1) % 2;
        } 
        else if (input == KEY_ENTER || input == '\n') {
            if (menuOption_ == 0) {
                currentWindow_ = 1;
            } else {
                exitRequested_ = true;
            }
        }
    }
}

bool MainMenu::shouldExit() const {
    return exitRequested_;
}

int MainMenu::getCurrentWindow() const {
    return currentWindow_;
}

void MainMenu::returnToMenu() {
    currentWindow_ = 0;
}

void MainMenu::drawMenu() const {
    curs_set(0);
    char text[] = {67, 114, 101, 100, 105, 116, 115, 58, 32, 71, 111, 114, 100, 111, 110, 32, 88, 117, 44, 32, 65, 108, 108, 101, 110, 32, 90, 104, 117, 0};
    mvwprintw(win_, (LINES - 1), (COLS - strlen(text)) / 2, "%s", text);
    mvwprintw(win_, (LINES - 11) / 2 - 5, (COLS - 40) / 2, "    _   __           _   __      __     ");
    mvwprintw(win_, (LINES - 11) / 2 - 4, (COLS - 40) / 2, "   / | / /__  ____  / | / /___  / /____ ");
    mvwprintw(win_, (LINES - 11) / 2 - 3, (COLS - 40) / 2, "  /  |/ / _ \\/ __ \\/  |/ / __ \\/ __/ _ \\");
    mvwprintw(win_, (LINES - 11) / 2 - 2, (COLS - 40) / 2, " / /|  /  __/ /_/ / /|  / /_/ / /_/  __/");
    mvwprintw(win_, (LINES - 11) / 2 - 1, (COLS - 40) / 2, "/_/ |_/\\___/\\____/_/ |_/\\____/\\__/\\___/ ");    

    int rowPrint = (LINES - 2) / 2;

    if (menuOption_ == 0) {
        wattron(win_, COLOR_PAIR(1));
        mvwprintw(win_, rowPrint, (COLS - 8) / 2, "my notes");
        wattroff(win_, COLOR_PAIR(1));
        mvwprintw(win_, rowPrint + 1, (COLS - 4) / 2, "exit");
    } else {
        mvwprintw(win_, rowPrint, (COLS - 8) / 2, "my notes");
        wattron(win_, COLOR_PAIR(1));
        mvwprintw(win_, rowPrint + 1, (COLS - 4) / 2, "exit");
        wattroff(win_, COLOR_PAIR(1));
    }
    wrefresh(win_);
}
