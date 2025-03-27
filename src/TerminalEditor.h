#ifndef TERMINAL_EDITOR_H
#define TERMINAL_EDITOR_H

#include <ncurses.h>
#include <vector>
#include <string>
#include "FileManager.h"
#include "EditorUI.h"

class TerminalEditor {
public:
    TerminalEditor(WINDOW *win, WINDOW *sidebar, WINDOW *content,
                 const std::vector<std::string> &files);
    
    void handleInput(int ch);
    void cleanup();
    
private:
    FileManager fileManager;
    EditorUI ui;
    
    int row;
    int col;
    int scroll_row;
    int scroll_col;
    int focused_div;
    int sidebar_index;
    std::vector<std::string> lines;
    std::string current_file;

    void handleInputContent(int ch);
    void handleInputSidebar(int ch);
    void adjustCursorPosition();
};

#endif
