#ifndef TRMINALCONTROLLER_H
#define TERMINALCONTROLLER_H

#include <ncurses.h>
#include <vector>
#include <string>
#include "DocumentModel.h"
#include "Viewport.h"
#include "SidebarUI.h"
#include "ContentUI.h"
#include "FileHandler.h"
#include "MenuUI.h"

class TerminalController {
public:
    TerminalController();
    ~TerminalController();
    void run();
    
private:
    WINDOW* win;
    WINDOW* sidebar;
    WINDOW* content;
    
    int focused_div;
    int sidebar_width;
    int content_width;
    
    MenuUI menuUI;
    int currentWindow; // 0 = menu, 1 = editor

    DocumentModel document;
    Viewport viewport;
    SidebarUI sidebarUI;
    ContentUI contentUI;
    FileHandler fileHandler;
    
    std::vector<std::string> files;
    
    void initializeUI();
    void handleInput(int ch);
    void cleanup();
};

#endif // TERMINALCONTROLLER _H

