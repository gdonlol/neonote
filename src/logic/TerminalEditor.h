#ifndef TERMINALEDITOR_H
#define TERMINALEDITOR_H

#include <string>
#include <vector>
#include <ncurses.h>

class TerminalEditor
{
public:
    TerminalEditor();
    void run(const std::string &filename);

private:
    std::vector<std::string> lines;
    int row, col;

    void loadFile(const std::string &filename);
    void saveFile(const std::string &filename);
    void initScreen();
    void mainLoop();
    void renderUI();
    void displayContent();
    void handleInput(int ch);
    void cleanup();
};

#endif // TERMINALEDITOR_H
