#include "ContentUI.h"
#include <algorithm>

ContentUI::ContentUI() {}

void ContentUI::render(WINDOW* content, const DocumentModel& document, Viewport& viewport) {
    werase(content);
    box(content, 0, 0);
    
    int max_lines = LINES - 4;
    int max_cols = (COLS * 0.75) - 4;
    
    const auto& lines = document.getLines();
    int cursorRow = document.getRow();
    int cursorCol = document.getCol();
    
    viewport.update(cursorRow, cursorCol, lines.size(), 
                  lines.empty() ? 0 : lines[cursorRow].length(), 
                  max_lines, max_cols);
    
    int scrollRow = viewport.getScrollRow();
    int scrollCol = viewport.getScrollCol();
    
    for (int i = 0; i < max_lines; ++i) {
        int line_index = scrollRow + i;
        if (line_index < lines.size()) {
            bool isCurrentLine = (line_index == cursorRow);
            std::string processed = processMarkdown(lines[line_index], isCurrentLine);
            
            if (scrollCol < processed.size()) {
                processed = processed.substr(scrollCol, max_cols);
            } else {
                processed = "";
            }
            
            mvwprintw(content, i + 1, 1, "%s", processed.c_str());
        }
    }
    
    // Position cursor
    int cursor_x = cursorCol - scrollCol + 1;
    int cursor_y = cursorRow - scrollRow + 1;
    wmove(content, cursor_y, cursor_x);
    
    wrefresh(content);
}

void ContentUI::handleInput(int ch, DocumentModel& document, Viewport& viewport) {
    int row = document.getRow();
    int col = document.getCol();
    
    switch (ch) {
        case KEY_UP:
            document.setCursor(row - 1, col);
            break;
        case KEY_DOWN:
            document.setCursor(row + 1, col);
            break;
        case KEY_LEFT:
            document.setCursor(row, col - 1);
            break;
        case KEY_RIGHT:
            document.setCursor(row, col + 1);
            break;
        case KEY_BACKSPACE:
            document.deleteChar(row, col);
            break;
        case '\n':
            document.insertNewline(row, col);
            break;
        default:
            if (ch >= 32 && ch <= 126) { // Printable characters
                document.insertChar(row, col, ch);
            }
            break;
    }
}

std::string ContentUI::processMarkdown(const std::string& line, bool showFormatting) const {
    if (!showFormatting) {
        std::string result;
        bool inBold = false;
        bool inItalic = false;
        
        for (size_t i = 0; i < line.size(); ) {
            if (i + 1 < line.size() && line[i] == '*' && line[i+1] == '*') {
                inBold = !inBold;
                i += 2;
            } else if (line[i] == '*' && !inBold) {
                inItalic = !inItalic;
                i += 1;
            } else {
                result += line[i++];
            }
        }
        
        return result;
    }
    return line;
}
