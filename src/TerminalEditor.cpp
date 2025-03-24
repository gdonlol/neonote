#include "TerminalEditor.h"
#include <ncurses.h>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

TerminalEditor::TerminalEditor(WINDOW *win_in, WINDOW *sidebar_in, WINDOW *content_in, vector<string> &files_in)
    : win(win_in), sidebar(sidebar_in), content(content_in), files(files_in) // Initialization list ✅
{
    row = 0;
    col = 0;
    scroll_row = 0;
    scroll_col = 0;
    focused_div = 0;
    lines = {""};

    if (!files.empty())
    {
        loadFile(files[0]);
    }
}

void TerminalEditor::loadFile(const string &filename)
{
    const char *home = getenv("HOME");
    if (home == nullptr)
        return;

    string path = string(home) + "/.local/share/" + filename + ".md";

    ifstream file(path);
    if (!file.is_open())
        return;

    string line;
    while (getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();
}

void TerminalEditor::saveFile(const string &filename)
{
    const char *home = getenv("HOME");
    if (home == nullptr)
        return;

    string path = string(home) + "/.local/share/" + filename + ".md";

    ofstream file(path);
    for (const auto &line : lines)
    {
        file << line << '\n';
    }
}

void TerminalEditor::RenderUI(int sidebar_width, vector<string> &files)
{
    werase(win);
    wrefresh(win);

    refresh();

    box(sidebar, 0, 0);
    mvwprintw(sidebar, 2, 2, "My Tasks");
    mvwprintw(sidebar, 3, 2, "Calendar");
    mvwhline(win, 5, 1, ACS_HLINE, sidebar_width - 2);

    for (int i = 0; i < files.size(); i++)
    {
        mvwprintw(sidebar, 7 + i, 2, files[i].c_str());
    }

    box(content, 0, 0);

    wrefresh(win);
    wrefresh(sidebar);
    wrefresh(content);
}

void TerminalEditor::displayContent()
{
    werase(content);
    box(content, 0, 0);
    int max_lines = LINES - 4;        // Subtract 2 for borders and 2 for padding
    int max_cols = (COLS * 0.75) - 4; // Subtract 2 for borders and 2 for padding

    // Adjust scroll_row to ensure the cursor is visible
    if (row < scroll_row)
    {
        scroll_row = row;
    }
    else if (row >= scroll_row + max_lines)
    {
        scroll_row = row - max_lines + 1;
    }

    // Adjust scroll_col to ensure the cursor is visible
    if (col < scroll_col)
    {
        scroll_col = col;
    }
    else if (col >= scroll_col + max_cols)
    {
        scroll_col = col - max_cols + 1;
    }

    for (int i = 0; i < max_lines; ++i)
    {
        int line_index = scroll_row + i;
        if (line_index < lines.size())
        {
            string line = lines[line_index];

            if (scroll_col < line.size())
            {
                line = line.substr(scroll_col, max_cols);
            }
            else
            {
                line = "";
            }

            mvwprintw(content, i + 2, 2, "%s", line.c_str());
        }
    }

    wmove(content, row - scroll_row + 2, col - scroll_col + 2);
    wrefresh(content);
}

void TerminalEditor::handleInput(int ch)
{
    if (focused_div == 0)
    {
        handleInputContent(ch);
    }
    else
    {
        handleInputSidebar(ch);
    }
}

void TerminalEditor::handleInputContent(int ch)
{
    // Segfault Insurance pt. 1
    if (lines.empty())
    {
        lines.push_back("");
    }

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
        lines[row].insert(col, string(1, ch));
        ++col;
        break;
    }

    // Segfault Insurance pt.2
    if (row >= lines.size())
    {
        row = lines.size() - 1;
    }
    if (col > lines[row].length())
    {
        col = lines[row].length();
    }
}

void TerminalEditor::handleInputSidebar(int ch)
{
}

void TerminalEditor::cleanup()
{
    endwin();
}
