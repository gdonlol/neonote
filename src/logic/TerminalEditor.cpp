#include "TerminalEditor.h"
#include <iostream>

// ANSI escape codes for text formatting in the terminal
const std::string ANSI_BOLD_ON = "\033[1m";
const std::string ANSI_ITALIC_ON = "\033[3m";
const std::string ANSI_RESET = "\033[0m";

TerminalEditor::TerminalEditor() : content("") {}

void TerminalEditor::insertText(const std::string& text) {
    content += text;
}

std::string TerminalEditor::getContent() const {
    return content;
}

std::string TerminalEditor::applyBold(const std::string& text) const {
    return ANSI_BOLD_ON + text + ANSI_RESET;
}

std::string TerminalEditor::applyItalic(const std::string& text) const {
    return ANSI_ITALIC_ON + text + ANSI_RESET;
}

void TerminalEditor::saveFile(const std::string& filePath) const {
    std::cout << "Saving file to " << filePath << "..." << std::endl;
    // File I/O code would be added here.
}

void TerminalEditor::openFile(const std::string& filePath) {
    std::cout << "Opening file from " << filePath << "..." << std::endl;
    // File I/O code would be added here.
    content = "Loaded file content.";
}
