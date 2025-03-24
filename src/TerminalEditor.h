#ifndef TERMINALEDITOR_H
#define TERMINALEDITOR_H

#include <string>
#include <vector>
#include <ncurses.h>
using namespace std;

class TerminalEditor
{
public:
    TerminalEditor(WINDOW *win, WINDOW *sidebar, WINDOW *content, vector<string> &files);
    void handleInputContent(int ch);
    void handleInputSidebar(int ch);
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
    vector<string> &files;      // Files vector

    void loadFile(const string &filename);
    void saveFile(const string &filename);

    bool isCursorInFormattedRegion(int row, int col);
    void applyMarkdownFormatting(const std::string &line, int y, int x);

    void cleanup();
};

#endif // TERMINALEDITOR_H
