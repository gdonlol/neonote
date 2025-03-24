#ifndef MENUUI_H
#define MENUUI_H

#include <ncurses.h>

class MenuUI {
public:
    MenuUI();
    
    void render(WINDOW* win);
    int handleInput(int ch);
    
private:
    int menuOption;
};

#endif // MENUUI_H

