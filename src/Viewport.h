#ifndef VIEWPORT_H
#define VIEWPORT_H

class Viewport {
public:
    Viewport();
    
    void update(int cursorRow, int cursorCol, int numLines, int lineLength, int maxLines, int maxCols);
    
    int getScrollRow() const { return scrollRow; }
    int getScrollCol() const { return scrollCol; }
    
private:
    int scrollRow;
    int scrollCol;
};

#endif // VIEWPORT_H

