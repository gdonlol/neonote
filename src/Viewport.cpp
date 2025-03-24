#include "Viewport.h"

Viewport::Viewport() : scrollRow(0), scrollCol(0) {}

void Viewport::update(int cursorRow, int cursorCol, int numLines, int lineLength, int maxLines, int maxCols) {
    // Adjust scrollRow to ensure cursor is visible
    if (cursorRow < scrollRow) {
        scrollRow = cursorRow;
    } else if (cursorRow >= scrollRow + maxLines) {
        scrollRow = cursorRow - maxLines + 1;
    }

    // Adjust scrollCol to ensure cursor is visible
    if (cursorCol < scrollCol) {
        scrollCol = cursorCol;
    } else if (cursorCol >= scrollCol + maxCols) {
        scrollCol = cursorCol - maxCols + 1;
    }
}
