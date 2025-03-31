#include "MainMenu.h"
#include <string>
#include <cstring>

/**
 * @brief Constructs a MainMenu object.
 * 
 * This constructor initializes the MainMenu object with a given window pointer. It also sets default values
 * for the current window, selected menu option, and exit request status.
 * 
 * @param win Pointer to the window object to display the menu on.
 */
MainMenu::MainMenu(WINDOW* win) 
    : win_(win), currentWindow_(0), menuOption_(0), exitRequested_(false) {}


/**
 * @brief Displays the current window content.
 * 
 * This method checks the `currentWindow_` and displays the menu if it's the main menu (i.e., `currentWindow_ == 0`).
 */
void MainMenu::display() {
    if (currentWindow_ == 0) {
        drawMenu();
    }
}

/**
 * @brief Handles user input to navigate the menu.
 * 
 * This method processes the user input to navigate through the menu options. It allows the user to move up or down
 * through the options and select an option to either continue to another window or exit the application.
 * 
 * @param input The input character or key pressed by the user.
 */
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

/**
 * @brief Checks if the user has requested to exit.
 * 
 * This method returns whether the user has selected the "exit" option from the main menu.
 * 
 * @return `true` if the user requested to exit, otherwise `false`.
 */
bool MainMenu::shouldExit() const {
    return exitRequested_;
}

/**
 * @brief Gets the current window index.
 * 
 * This method returns the index of the current window. A value of 0 indicates the main menu.
 * 
 * @return The current window index.
 */
int MainMenu::getCurrentWindow() const {
    return currentWindow_;
}

/**
 * @brief Returns to the main menu.
 * 
 * This method sets the `currentWindow_` to 0, indicating the user is back at the main menu.
 */
void MainMenu::returnToMenu() {
    currentWindow_ = 0;
}


/**
 * @brief Draws the main menu.
 * 
 * This method draws the visual layout of the main menu, including a title and the menu options ("my notes" and "exit").
 * The currently selected option is highlighted.
 */
void MainMenu::drawMenu() const {
    curs_set(0);
    char text[] = {67, 114, 101, 100, 105, 116, 115, 58, 32, 71, 111, 114, 100, 111, 110, 32, 88, 117, 44, 32, 65, 108, 108, 101, 110, 32, 90, 104, 117, 44, 32, 69, 109, 105, 108, 121, 32, 65, 116, 121, 101, 111, 0};
    attron(COLOR_PAIR(2));
    mvprintw((LINES - 11) / 2 - 5, (COLS - 40) / 2, "    _   __           _   __      __     ");
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));
    mvprintw((LINES - 11) / 2 - 4, (COLS - 40) / 2, "   / | / /__  ____  / | / /___  / /____ ");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(4));
    mvprintw((LINES - 11) / 2 - 3, (COLS - 40) / 2, "  /  |/ / _ \\/ __ \\/  |/ / __ \\/ __/ _ \\");
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(5));
    mvprintw((LINES - 11) / 2 - 2, (COLS - 40) / 2, " / /|  /  __/ /_/ / /|  / /_/ / /_/  __/");
    attroff(COLOR_PAIR(5));
    attron(COLOR_PAIR(6));
    mvprintw((LINES - 11) / 2 - 1, (COLS - 40) / 2, "/_/ |_/\\___/\\____/_/ |_/\\____/\\__/\\___/ ");
    attroff(COLOR_PAIR(6));
    mvwprintw(win_, (LINES - 1), (COLS - strlen(text)) / 2, "%s", text);

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
