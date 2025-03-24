#ifndef TERMINALEDITOR_H
#define TERMINALEDITOR_H

#include <string>
#include <vector>
#include <ncurses.h>

class TerminalEditor
{
public:
    TerminalEditor(WINDOW *win, WINDOW *sidebar, WINDOW *content);
    void handleInput(int ch);
    void loadFile(const std::string &filename);
    void displayContent();
    void RenderUI(int sidebar_width, std::vector<std::string> &files);

private:
    std::vector<std::string> lines; // Stores the lines of the file
    int row, col;                   // Current cursor position
    int scroll_row, scroll_col;     // Scroll position for the content window
    WINDOW *win;                    // Main window
    WINDOW *sidebar;                // Sidebar window
    WINDOW *content;                // Content window

    void saveFile(const std::string &filename);

    void cleanup();
};

#endif // TERMINALEDITOR_H
