#include "TerminalEditor.h"
#include <ncurses.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
using namespace std;

TerminalEditor::TerminalEditor(WINDOW *win_in, WINDOW *sidebar_in, WINDOW *content_in, vector<string> &files_in)
    : win(win_in), sidebar(sidebar_in), content(content_in), files(files_in)
{
    row = 0;
    col = 0;
    scroll_row = 0;
    scroll_col = 0;
    focused_div = 0;
    lines = {};

    // creating app directory
    const char *home = getenv("HOME");
    if (home == nullptr)
    {
        printw("no home directory");
        getch();
        endwin();
    }
    string homePath = string(home) + "/.local/share/neonote";
    struct stat info;
    // create the directory
    if (!(stat(homePath.c_str(), &info) == 0 && S_ISDIR(info.st_mode)) && mkdir(homePath.c_str(), 0775) != 0)
    {
        printw("mkdir failed");
        getch();
        endwin();
    }
    // read files
    for (const auto &entry : std::filesystem::directory_iterator(homePath))
    {
        files.push_back(entry.path().stem().string());
    }
    if (files.size() == 0)
    {
        string filePath = homePath + "/" + "Untitled1.md";
        ofstream file(filePath);
        if (file)
        {
            file.close();
            files.push_back("Untitled1");
        }
        else
        {
            printw("default file creation failed");
            getch();
            endwin();
        }
    }

    if (!files.empty())
    {
        loadFile(files[0]);
        displayContent();
    }
}

void TerminalEditor::loadFile(const string &filename)
{
    const char *home = getenv("HOME");
    if (home == nullptr)
        return;

    string path = string(home) + "/.local/share/neonote/" + filename + ".md";

    ifstream file(path);

    string line;
    while (getline(file, line))
    {
        // cout << line << endl; // for debug
        lines.push_back(line);
    }
    file.close();
}

void TerminalEditor::saveFile(const string &filename)
{
    const char *home = getenv("HOME");
    if (home == nullptr)
        return;

    string path = string(home) + "/.local/share/neonote/" + filename + ".md";

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
    int max_lines = LINES - 4;      // Subtract 2 for borders and 2 for padding
    int max_cols = COLS * 0.75 - 4; // Subtract 2 for borders and 2 for padding

    // Adjust scroll_row and scroll_col
    if (row < scroll_row)
    {
        scroll_row = row;
    }
    else if (row >= scroll_row + max_lines)
    {
        scroll_row = row - max_lines + 1;
    }

    if (col < scroll_col)
    {
        scroll_col = col;
    }
    else if (col >= scroll_col + max_cols)
    {
        scroll_col = col - max_cols + 1;
    }

    // Adjust for asterisks
    int asterisk_offset;

    static bool bold_on = false;
    static bool italics_on = false;

    for (int i = 0; i < max_lines; ++i)
    {

        int line_index = scroll_row + i; // Apply scrolling offset
        if (line_index < lines.size())
        {                                         // Display until last possible row
            std::string line = lines[line_index]; // Load to buffer
            int x = 2;                            // Compensate for padding
            asterisk_offset = 0;

            if (bold_on)
            {
                wattroff(content, A_BOLD);
                bold_on = false;
            }
            if (italics_on)
            {
                wattroff(content, A_ITALIC);
                italics_on = false;
            }

            for (size_t pos = scroll_col; pos < line.length() && x < max_cols + 2; ++pos)
            {

                if (pos + 1 < line.length() && line[pos] == '*' && line[pos + 1] == '*')
                {
                    bold_on = !bold_on;
                    ++asterisk_offset;
                    ++asterisk_offset;
                    if (bold_on)
                    {
                        wattron(content, A_BOLD);
                    }
                    else
                    {
                        wattroff(content, A_BOLD);
                    }

                    ++pos;    // Skip the second asterisk
                    continue; // Don't render the markers
                }

                else if (line[pos] == '*')
                {
                    italics_on = !italics_on;
                    ++asterisk_offset;
                    if (italics_on)
                    {
                        wattron(content, A_ITALIC);
                    }
                    else
                    {
                        wattroff(content, A_ITALIC);
                    }
                    continue;
                }

                // Handle escape for literal asterisks
                else if (line[pos] == '\\' && pos + 1 < line.length() &&
                         (line[pos + 1] == '*' || line[pos + 1] == '\\'))
                {
                    ++pos;
                    ++asterisk_offset;
                }

                mvwaddch(content, i + 2, x, line[pos]);
                ++x;
            }
        }
    }

    if (bold_on)
    {
        wattroff(content, A_BOLD);
        bold_on = false;
    }
    if (italics_on)
    {
        wattroff(content, A_ITALIC);
        italics_on = false;
    }

    wmove(content, row - scroll_row + 2, col - scroll_col + 2 - asterisk_offset);
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
    case 19:
        saveFile(files[0]);
        break;
    case KEY_F(1):
        cleanup();
        break;
    case 17:
        cleanup();
        break;
    default:
        if (ch >= 32 && ch <= 126)
        {
            lines[row].insert(col, std::string(1, ch));
            ++col;
        }
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
    saveFile(files[0]);
    endwin();
    exit(0);
}
