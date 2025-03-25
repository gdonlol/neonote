#include "TerminalEditor.h"
#include <string>

using std::string;

TerminalEditor::TerminalEditor(WINDOW *win_in, WINDOW *sidebar_in, 
                             WINDOW *content_in, const std::vector<std::string> &files_in)
    : fileManager(), ui(win_in, sidebar_in, content_in),
      row(0), col(0), scroll_row(0), scroll_col(0), focused_div(0) {
    
    // Load initial file
    std::vector<std::string> initialFiles = fileManager.getFiles();
    if (!initialFiles.empty()) {
        fileManager.loadFile(initialFiles[0], lines);
    } else {
        lines.push_back("");
    }
    
    curs_set(1);

    // Initial render
    ui.renderUI(COLS * 0.25, initialFiles);
    ui.displayContent(lines, row, col, scroll_row, scroll_col);
}

void TerminalEditor::handleInput(int ch) {
    if (focused_div == 0) {
        handleInputContent(ch);
    } else {
        handleInputSidebar(ch);
    }
    adjustCursorPosition();
    ui.displayContent(lines, row, col, scroll_row, scroll_col);
}

void TerminalEditor::handleInputContent(int ch) {
    if (lines.empty()) {
        lines.push_back("");
    }

    switch (ch) {
        case KEY_UP: if (row > 0) row--; break;
        case KEY_DOWN: if (row < lines.size() - 1) row++; break;
        case KEY_LEFT: if (col > 0) col--; break;
        case KEY_RIGHT: if (col < lines[row].length()) col++; break;
        case KEY_BACKSPACE:
            if (col > 0) {
                lines[row].erase(col - 1, 1);
                col--;
            } else if (row > 0) {
                col = lines[row - 1].length();
                lines[row - 1] += lines[row];
                lines.erase(lines.begin() + row);
                row--;
            }
            break;
        case '\n':
            lines.insert(lines.begin() + row + 1, lines[row].substr(col));
            lines[row].resize(col);
            row++;
            col = 0;
            break;
        case 19: // Ctrl+S
            fileManager.saveFile(fileManager.getFiles()[0], lines);
            break;
        case KEY_F(1):
        case 17: // Ctrl+Q
            cleanup();
            break;
        default:
            if (ch >= 32 && ch <= 126) {
                lines[row].insert(col, string(1, ch));
                col++;
            }
            break;
    }
}

void TerminalEditor::handleInputSidebar(int ch) {
    // Sidebar navigation logic would go here
}

void TerminalEditor::adjustCursorPosition() {
    if (row >= lines.size()) row = lines.size() - 1;
    if (col > lines[row].length()) col = lines[row].length();
    
    // Adjust scroll positions
    int max_lines = LINES - 4;
    int max_cols = COLS * 0.75 - 4;
    
    if (row < scroll_row) scroll_row = row;
    else if (row >= scroll_row + max_lines) scroll_row = row - max_lines + 1;
    
    if (col < scroll_col) scroll_col = col;
    else if (col >= scroll_col + max_cols) scroll_col = col - max_cols + 1;
}

void TerminalEditor::cleanup() {
    fileManager.saveFile(fileManager.getFiles()[0], lines);
    ui.cleanup();
    exit(0);
}
