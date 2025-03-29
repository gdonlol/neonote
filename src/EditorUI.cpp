#include "EditorUI.h"
#include <ncurses.h>
#include <string>
#include <vector>
#include "TextPrompt.h"

/**
 * @brief Constructor for the EditorUI class.
 * 
 * @param win_in The main window to be used for rendering.
 * @param sidebar_in The sidebar window to be used for rendering.
 * @param content_in The content window to be used for rendering.
 */
EditorUI::EditorUI(WINDOW *win_in, WINDOW *sidebar_in, WINDOW *content_in) 
    : win(win_in), sidebar(sidebar_in), content(content_in) {}

/**
 * @brief Renders the entire user interface.
 * 
 * Clears the main window, renders the sidebar, and the content window. 
 * Refreshes all windows after rendering.
 * 
 * @param sidebar_width The width of the sidebar.
 * @param files A vector of strings representing the files to display in the sidebar.
 */
void EditorUI::renderUI(int sidebar_width, const std::vector<std::string> &files) {
    werase(win);
    wrefresh(win);
    refresh();

    renderSidebar(sidebar_width, files, 0);
    box(content, 0, 0);

    wrefresh(win);
    wrefresh(sidebar);
    wrefresh(content);
}

/**
 * @brief Renders the sidebar with the list of files and additional content.
 * 
 * The sidebar includes labels like "My Tasks" and "Calendar", 
 * along with a horizontal line and a list of file names.
 * 
 * @param sidebar_width The width of the sidebar.
 * @param files A vector of strings representing the file names to be displayed.
 */
void EditorUI::renderSidebar(int sidebar_width, const std::vector<std::string> &files, int sidebar_index) {
    wclear(sidebar); 
    box(sidebar, 0, 0);
    mvwhline(sidebar, 5, 1, ACS_HLINE, sidebar_width - 2);
    if(sidebar_index == files.size()) wattron(sidebar, COLOR_PAIR(1));
    mvwprintw(sidebar, 2, 2, "%s", std::string("My Tasks").substr(0, std::max(0, sidebar_width - 4)).c_str());
    wattroff(sidebar, COLOR_PAIR(1));

    if(sidebar_index == files.size() + 1) wattron(sidebar, COLOR_PAIR(1));
    mvwprintw(sidebar, 3, 2, "%s", std::string("Calendar").substr(0, std::max(0, sidebar_width - 4)).c_str());
    wattroff(sidebar, COLOR_PAIR(1));
    
    for (size_t i = 0; i < files.size(); i++) {
        if(i == sidebar_index) wattron(sidebar, COLOR_PAIR(1));
        mvwprintw(sidebar, 7 + i, 2, "%s", files[i].substr(0, std::max(0, sidebar_width - 4)).c_str());
        wattroff(sidebar, COLOR_PAIR(1));
    }
    wrefresh(sidebar);
}

/**
 * @brief Displays the content in the content window.
 * 
 * Clears the content window, draws a box around it, and then renders the 
 * actual content lines in the window. The content is rendered based on 
 * the current row, column, and scrolling parameters.
 * 
 * @param lines A vector of strings representing the lines to be displayed.
 * @param row The current row position of the cursor.
 * @param col The current column position of the cursor.
 * @param scroll_row The row index for the scrolling.
 * @param scroll_col The column index for the scrolling.
 */
void EditorUI::displayContent(const std::vector<std::string> &lines, 
                            int row, int col, int scroll_row,
                            int scroll_col, std::string title) {
    werase(content);
    box(content, 0, 0);
    wattron(content, A_BOLD);
    mvwprintw(content, 1, 2, "%s", title.substr(0, std::max(0, static_cast<int>((COLS * 0.75) - 4))).c_str());
    wattroff(content, A_BOLD);
    renderContent(lines, row, col, scroll_row, scroll_col);
    wrefresh(content);
}

/**
 * @brief Renders the actual content inside the content window.
 * 
 * This method handles the rendering of lines with special formatting such as 
 * bold and italics based on the presence of asterisks (*). It also manages 
 * cursor position and scrolling logic.
 * 
 * @param lines A vector of strings representing the lines to be displayed.
 * @param row The current row position of the cursor.
 * @param col The current column position of the cursor.
 * @param scroll_row The row index for the scrolling.
 * @param scroll_col The column index for the scrolling.
 */
void EditorUI::renderContent(const std::vector<std::string> &lines,
                           int row, int col,
                           int scroll_row, int scroll_col) {
    int max_lines = LINES - 4;
    int max_cols = COLS * 0.75 - 4;
    static bool bold_on = false;
    static bool italics_on = false;
    int total_asterisk_offset = 0; // Track total asterisk offset up to cursor position
    int line_asterisk_offset = 0;  // Track asterisk offset for current line

    // Calculate the total asterisk offset up to the cursor position
    if (row >= scroll_row && row < scroll_row + max_lines) {
        std::string cursor_line = lines[row];
        for (size_t pos = 0; pos < col && pos < cursor_line.length(); ++pos) {
            if (pos + 1 < cursor_line.length() && cursor_line[pos] == '*' && cursor_line[pos + 1] == '*') {
                total_asterisk_offset += 2;
                pos++;
            }
            else if (cursor_line[pos] == '*') {
                total_asterisk_offset += 1;
            }
            else if (cursor_line[pos] == '\\' && pos + 1 < cursor_line.length() && 
                    (cursor_line[pos + 1] == '*' || cursor_line[pos + 1] == '\\')) {
                total_asterisk_offset += 1;
                pos++;
            }
        }
    }

    // Render all lines
    for (int i = 0; i < max_lines; ++i) {
        int line_index = scroll_row + i;
        if (line_index < lines.size()) {
            std::string line = lines[line_index];
            int x = 2;
            line_asterisk_offset = 0;

            if (bold_on) wattroff(content, A_BOLD);
            if (italics_on) wattroff(content, A_ITALIC);
            bold_on = italics_on = false;

            for (size_t pos = scroll_col; pos < line.length() && x < max_cols + 2; ++pos) {
                // Handle formatting marks
                if (pos + 1 < line.length() && line[pos] == '*' && line[pos + 1] == '*') {
                    bold_on = !bold_on;
                    line_asterisk_offset += 2;
                    pos++;
                    if (bold_on) wattron(content, A_BOLD);
                    else wattroff(content, A_BOLD);
                    continue;
                }
                else if (line[pos] == '*') {
                    italics_on = !italics_on;
                    line_asterisk_offset += 1;
                    if (italics_on) wattron(content, A_ITALIC);
                    else wattroff(content, A_ITALIC);
                    continue;
                }
                else if (line[pos] == '\\' && pos + 1 < line.length() && 
                        (line[pos + 1] == '*' || line[pos + 1] == '\\')) {
                    pos++;
                    line_asterisk_offset += 1;
                }

                mvwaddch(content, i + 2, x, line[pos]);
                ++x;
            }
        }
    }

    if (bold_on) wattroff(content, A_BOLD);
    if (italics_on) wattroff(content, A_ITALIC);
    
    // Apply the total asterisk offset to cursor position
    wmove(content, row - scroll_row + 2, col - scroll_col + 2 - total_asterisk_offset);
}

std::string EditorUI::displayPrompt(std::string title){
    TextPrompt prompt(win, title);
    return prompt.prompt();
}

/**
 * @brief Cleans up and terminates the ncurses session.
 * 
 * This function is called to clean up the ncurses environment after 
 * the program has finished running.
 */
void EditorUI::cleanup() {
    clear();
    refresh();
    wrefresh(win);
    endwin();
}
