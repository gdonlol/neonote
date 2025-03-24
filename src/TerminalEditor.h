#ifndef TERMINALEDITOR_H
#define TERMINALEDITOR_H

#include <string>
#include <vector>
#include <ncurses.h>
using namespace std;

class TerminalEditor
{
public:
    TerminalEditor(WINDOW *win, WINDOW *sidebar, WINDOW *content);
    void handleInputContent(int ch);
    void handleInputSidebar(int ch);
    void loadFile(const string &filename);
    void displayContent();
    void RenderUI(int sidebar_width, vector<string> &files);
    void handleInput(int ch);

private:
    vector<string> lines;       // Stores the lines of the file
    int row, col;               // Current cursor position
    int scroll_row, scroll_col; // Scroll position for the content window
    WINDOW *win;                // Main window
    WINDOW *sidebar;            // Sidebar window
    WINDOW *content;            // Content window
    int focused_div;            // Current focused panel

    void saveFile(const string &filename);

    void cleanup();
};

#endif // TERMINALEDITOR_H
