#include "TerminalHeader.h"
#include <ncurses.h>
#include <fstream>

TerminalEditor::TerminalEditor() : row(0), col(0), sidebar(nullptr), content(nullptr) {}

void TerminalEditor::run(const std::string& filename) {
    initScreen();
    loadFile(filename);
    mainLoop();
    saveFile(filename);
    cleanup();
}

void TerminalEditor::loadFile(const std::string& filename) {
    // Implementation to load file content into 'lines' vector
    std::ifstream file(filename); // Loads file for reading
    std::string line; // Temp variable for current line read
    while (std::getline(file, line)) {
        lines.push_back(line); // Load to lines vector
    }
}

void TerminalEditor::saveFile(const std::string& filename) {
    // Implementation to save 'lines' vector content to file
    std::ofstream file(filename); // Opens file for writing
    for (const auto& line : lines) { // Loop over lines to write
        file << line << '\n';
    }
}

void TerminalEditor::initScreen() {
    // Implementation to initialize ncurses screen and windows
}

void TerminalEditor::mainLoop() {
    // Implementation of the main loop to handle input and render UI
}

void TerminalEditor::renderUI() {
    // Implementation to render sidebar and content windows
}

void TerminalEditor::displayContent() {
    // Implementation to display the 'lines' vector content in the content window
}

void TerminalEditor::handleInput(int ch) {
    // Implementation to handle user input (keyboard events)
}

void TerminalEditor::cleanup() {
    // Implementation to clean up ncurses resources
}
