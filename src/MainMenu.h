/**
 * @file MainMenu.h
 * @brief Header file for the MainMenu class, which manages the main menu interface of the calendar application.
 * 
 * This file defines the MainMenu class, which handles rendering the main menu, processing user input, 
 * and managing window navigation. It is part of a calendar application utilizing ncurses for terminal-based UI.
 * 
 * @author 
 * - Gordon Xu
 * - Runcong Zhu
 */
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
