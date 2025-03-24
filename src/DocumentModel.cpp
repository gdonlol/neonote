#include "DocumentModel.h"

DocumentModel::DocumentModel() : cursorRow(0), cursorCol(0) {
    lines.push_back("");
}

void DocumentModel::setLines(const std::vector<std::string>& newLines) {
    lines = newLines;
    if (lines.empty()) {
        lines.push_back("");
    }
    ensureValidCursorPosition();
}

const std::vector<std::string>& DocumentModel::getLines() const {
    return lines;
}

void DocumentModel::insertChar(int row, int col, char ch) {
    if (row >= 0 && row < lines.size()) {
        lines[row].insert(col, 1, ch);
        cursorCol = col + 1;
    }
}

void DocumentModel::deleteChar(int row, int col) {
    if (row >= 0 && row < lines.size()) {
        if (col > 0) {
            lines[row].erase(col - 1, 1);
            cursorCol = col - 1;
        } else if (row > 0) {
            cursorCol = lines[row - 1].length();
            lines[row - 1] += lines[row];
            lines.erase(lines.begin() + row);
            cursorRow = row - 1;
        }
    }
}

void DocumentModel::insertNewline(int row, int col) {
    if (row >= 0 && row < lines.size()) {
        std::string remainder = lines[row].substr(col);
        lines[row].resize(col);
        lines.insert(lines.begin() + row + 1, remainder);
        cursorRow = row + 1;
        cursorCol = 0;
    }
}

void DocumentModel::setCursor(int row, int col) {
    cursorRow = row;
    cursorCol = col;
    ensureValidCursorPosition();
}

void DocumentModel::ensureValidCursorPosition() {
    if (cursorRow < 0) cursorRow = 0;
    if (cursorRow >= lines.size()) cursorRow = lines.size() - 1;
    if (cursorCol < 0) cursorCol = 0;
    if (cursorRow >= 0 && cursorRow < lines.size()) {
        if (cursorCol > lines[cursorRow].length()) {
            cursorCol = lines[cursorRow].length();
        }
    }
}
