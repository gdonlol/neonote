#ifndef TERMINAL_EDITOR_H
#define TERMINAL_EDITOR_H

#include <ncurses.h>
#include <vector>
#include <string>
#include "FileManager.h"
#include "EditorUI.h"
#include "Calendar.h"
#include "TaskManager.h"

class TerminalEditor {
public:
    TerminalEditor(WINDOW *win, WINDOW *sidebar, WINDOW *content,
                 const std::vector<std::string> &files);
    
    void handleInput(int ch);
    void redraw();
    void cleanup();
    
private:
    FileManager fileManager;
    EditorUI ui;
    Calendar calendar;
    TaskManager taskManager; 
    
    int row;
    int col;
    int scroll_row;
    int scroll_col;
    int focused_div;
    int last_focused_div;
    int sidebar_index;
    int sidebar_width;
    std::vector<std::string> lines;
    std::string current_file;

    void handleInputContent(int ch);
    void handleInputSidebar(int ch);
    void adjustCursorPosition();
};

#endif
