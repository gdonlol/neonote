#ifndef TERMINALEDITOR_H
#define TERMINALEDITOR_H

#include <string>
#include <vector>
#include <ncurses.h>

class TerminalEditor {
public:
    TerminalEditor();
    void run(const std::string &filename);
    void handleInput(int ch);
    void loadFile(const std::string &filename);
    void displayContent();

private:

    std::vector<std::string> lines; // Stores the lines of the file
    int row, col;                   // Current cursor position
    int scroll_row, scroll_col;     // Scroll position for the content window
    WINDOW *sidebar;                // Sidebar window
    WINDOW *content;                // Content window

    void saveFile(const std::string &filename);
    void initScreen();
    void renderUI();

    void cleanup();
};

#endif // TERMINALEDITOR_H
