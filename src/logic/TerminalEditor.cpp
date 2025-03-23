#include "TerminalEditor.h"
#include <ncurses.h>
#include <fstream>

TerminalEditor::TerminalEditor() : row(0), col(0) {}

void TerminalEditor::run(const std::string &filename)
{
    initScreen();
    loadFile(filename);
    mainLoop();
    saveFile(filename);
    cleanup();
}

void TerminalEditor::loadFile(const std::string &filename)
{
    // Implementation to load file content into 'lines' vector
    std::ifstream file(filename); // Loads file for reading
    std::string line;             // Temp variable for current line read
    while (std::getline(file, line))
    {
        lines.push_back(line); // Load to lines vector
    }
}

void TerminalEditor::saveFile(const std::string &filename)
{
    // Implementation to save 'lines' vector content to file
    std::ofstream file(filename); // Opens file for writing
    for (const auto &line : lines)
    { // Loop over lines to write
        file << line << '\n';
    }
}

void TerminalEditor::initScreen()
{
    // Implementation to initialize ncurses screen and windows
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    refresh();
}

// RENDERUI SENT TO RENDER, MAINLOOP SENT TO EDITORINPUTS

void TerminalEditor::displayContent()
{
    // Implementation to display the 'lines' vector content in the content window
}

void TerminalEditor::handleInput(int ch)
{
    switch (ch)
    {
    case KEY_UP:
        if (row > 0)
            --row;
        break;
    case KEY_DOWN:
        if (row < lines.size() - 1)
            ++row;
        break;
    case KEY_LEFT:
        if (col > 0)
            --col;
        break;
    case KEY_RIGHT:
        if (col < lines[row].length())
            ++col;
        break;
    case KEY_BACKSPACE:
        if (col > 0)
        {
            lines[row].erase(col - 1, 1);
            --col;
        }
        else if (row > 0)
        {
            col = lines[row - 1].length();
            lines[row - 1] += lines[row];
            lines.erase(lines.begin() + row);
            --row;
        }
        break;
    case '\n':
        lines.insert(lines.begin() + row + 1, lines[row].substr(col));
        lines[row].resize(col);
        ++row;
        col = 0;
        break;
    default:
        lines[row].insert(col, 1, ch);
        ++col;
        break;
    }
}

void TerminalEditor::cleanup()
{
    endwin();
}
