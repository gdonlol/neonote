#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <ncurses.h>
#include <string>

class MainMenu {
public:
    MainMenu(WINDOW* win);
    
    void display();
    void handleInput(int input);
    bool shouldExit() const;
    int getCurrentWindow() const;
    void returnToMenu();

private:
    WINDOW* win_;
    int currentWindow_;
    int menuOption_;
    bool exitRequested_;
    
    void drawMenu() const;
};

#endif
