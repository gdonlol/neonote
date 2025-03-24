#ifndef DOCUMENTMODEL_H
#define DOCUMENTMODEL_H

#include <vector>
#include <string>

class DocumentModel {
public:
    DocumentModel();
    
    void setLines(const std::vector<std::string>& newLines);
    const std::vector<std::string>& getLines() const;
    
    void insertChar(int row, int col, char ch);
    void deleteChar(int row, int col);
    void insertNewline(int row, int col);
    
    int getRow() const { return cursorRow; }
    int getCol() const { return cursorCol; }
    void setCursor(int row, int col);
    
private:
    std::vector<std::string> lines;
    int cursorRow;
    int cursorCol;
    
    void ensureValidCursorPosition();
};

#endif // DOCUMENTMODEL_H

