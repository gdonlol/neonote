#include "EditorUI.h"
#include <ncurses.h>
#include <string>
#include <vector>

EditorUI::EditorUI(WINDOW *win_in, WINDOW *sidebar_in, WINDOW *content_in) 
    : win(win_in), sidebar(sidebar_in), content(content_in) {}

void EditorUI::renderUI(int sidebar_width, const std::vector<std::string> &files) {
    werase(win);
    wrefresh(win);
    refresh();

    renderSidebar(sidebar_width, files);
    box(content, 0, 0);

    wrefresh(win);
    wrefresh(sidebar);
    wrefresh(content);
}

void EditorUI::renderSidebar(int sidebar_width, const std::vector<std::string> &files) {
    box(sidebar, 0, 0);
    mvwprintw(sidebar, 2, 2, "My Tasks");
    mvwprintw(sidebar, 3, 2, "Calendar");
    mvwhline(sidebar, 5, 1, ACS_HLINE, sidebar_width - 2);

    for (size_t i = 0; i < files.size(); i++) {
        mvwprintw(sidebar, 7 + i, 2, files[i].c_str());
    }
}

void EditorUI::displayContent(const std::vector<std::string> &lines, 
                            int row, int col, 
                            int scroll_row, int scroll_col) {
    werase(content);
    box(content, 0, 0);
    renderContent(lines, row, col, scroll_row, scroll_col);
    wrefresh(content);
}

void EditorUI::renderContent(const std::vector<std::string> &lines,
                           int row, int col,
                           int scroll_row, int scroll_col) {
    int max_lines = LINES - 4;
    int max_cols = COLS * 0.75 - 4;
    static bool bold_on = false;
    static bool italics_on = false;
    int asterisk_offset = 0; // Track asterisks that affect cursor position

    for (int i = 0; i < max_lines; ++i) {
        int line_index = scroll_row + i;
        if (line_index < lines.size()) {
            std::string line = lines[line_index];
            int x = 2;
            asterisk_offset = 0; // Reset for each line

            if (bold_on) wattroff(content, A_BOLD);
            if (italics_on) wattroff(content, A_ITALIC);
            bold_on = italics_on = false;

            for (size_t pos = scroll_col; pos < line.length() && x < max_cols + 2; ++pos) {
                // Handle formatting marks
                if (pos + 1 < line.length() && line[pos] == '*' && line[pos + 1] == '*') {
                    bold_on = !bold_on;
                    asterisk_offset += 2;
                    pos++;
                    if (bold_on) wattron(content, A_BOLD);
                    else wattroff(content, A_BOLD);
                    continue;
                }
                else if (line[pos] == '*') {
                    italics_on = !italics_on;
                    asterisk_offset++;
                    if (italics_on) wattron(content, A_ITALIC);
                    else wattroff(content, A_ITALIC);
                    continue;
                }
                else if (line[pos] == '\\' && pos + 1 < line.length() && 
                        (line[pos + 1] == '*' || line[pos + 1] == '\\')) {
                    pos++;
                    asterisk_offset++;
                }

                mvwaddch(content, i + 2, x, line[pos]);
                ++x;
            }
        }
    }

    if (bold_on) wattroff(content, A_BOLD);
    if (italics_on) wattroff(content, A_ITALIC);
    
    // Apply asterisk offset to cursor position
    wmove(content, row - scroll_row + 2, col - scroll_col + 2 -  asterisk_offset);
}


void EditorUI::cleanup() {
    endwin();
}
