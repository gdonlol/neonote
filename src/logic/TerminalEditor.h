#ifndef TERMINALEDITOR_H
#define TERMINALEDITOR_H

#include <string>

class TerminalEditor {
public:
    TerminalEditor();
    void insertText(const std::string& text);
    std::string getContent() const;
    std::string applyBold(const std::string& text) const;
    std::string applyItalic(const std::string& text) const;
    void saveFile(const std::string& filePath) const;
    void openFile(const std::string& filePath);
    
private:
    std::string content;
};

#endif // TERMINALEDITOR_H
