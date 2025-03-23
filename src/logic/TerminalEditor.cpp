#include "TerminalEditor.h"
#include <ncurses.h>
#include <fstream>

TerminalEditor::TerminalEditor() : row(0), col(0), scroll_row(0), scroll_col(0) {}

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
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
}

void TerminalEditor::saveFile(const std::string &filename)
{
    std::ofstream file(filename);
    for (const auto &line : lines)
    {
        file << line << '\n';
    }
}

void TerminalEditor::initScreen()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    refresh();
}

void TerminalEditor::renderUI() {
    int sidebar_width = COLS * 0.25;
    int content_width = COLS - sidebar_width;

    sidebar = derwin(stdscr, LINES, sidebar_width, 0, 0);
    content = derwin(stdscr, LINES, content_width, 0, sidebar_width);

    box(sidebar, 0, 0);
    mvwprintw(sidebar, 1, 2, "Menu");

    box(content, 0, 0);
    mvwprintw(content, 1, 2, "Content");

    wrefresh(sidebar);
    wrefresh(content);
}

void TerminalEditor::displayContent() {
    werase(content);
    int max_lines = LINES - 4; // Subtract 2 for borders and 2 for padding
    int max_cols = COLS * 0.75 - 4; // Subtract 2 for borders and 2 for padding

    // Adjust scroll_row to ensure the cursor is visible
    if (row < scroll_row) {
        scroll_row = row;
    } else if (row >= scroll_row + max_lines) {
        scroll_row = row - max_lines + 1;
    }

    // Adjust scroll_col to ensure the cursor is visible
    if (col < scroll_col) {
        scroll_col = col;
    } else if (col >= scroll_col + max_cols) {
        scroll_col = col - max_cols + 1;
    }

    for (int i = 0; i < max_lines; ++i) {
        int line_index = scroll_row + i;
        if (line_index < lines.size()) {
            std::string line = lines[line_index].substr(scroll_col, max_cols);
            mvwprintw(content, i + 2, 2, "%s", line.c_str());
        }
    }

    // Move cursor to the correct position relative to the scroll
    wmove(content, row - scroll_row + 2, col - scroll_col + 2);
    wrefresh(content);
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
