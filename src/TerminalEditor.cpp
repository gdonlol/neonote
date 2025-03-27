#include "TerminalEditor.h"
#include <string>

using std::string;

/**
 * @brief Constructs a TerminalEditor instance.
 * 
 * Initializes the terminal editor by setting up the file manager and user interface. It also loads the 
 * initial file (if any) into the editor and renders the UI.
 * 
 * @param win_in Pointer to the main window.
 * @param sidebar_in Pointer to the sidebar window.
 * @param content_in Pointer to the content window.
 * @param files_in List of files to be loaded into the editor (not used directly here).
 */
TerminalEditor::TerminalEditor(WINDOW *win_in, WINDOW *sidebar_in, 
                               WINDOW *content_in, const std::vector<std::string> &files_in)
    : fileManager(), ui(win_in, sidebar_in, content_in),
      row(0), col(0), scroll_row(0), scroll_col(0), focused_div(0) {

    // Load initial file from file manager
    std::vector<std::string> initialFiles = fileManager.getFiles();
    if (!initialFiles.empty()) {
        fileManager.loadFile(initialFiles[0], lines);  /**< Load the first file into lines. */
    } else {
        lines.push_back("");  /**< If no files exist, start with an empty line. */
    }

    curs_set(1);  /**< Show the cursor in the terminal editor. */

    // Render the initial UI and display the loaded content
    ui.renderUI(COLS * 0.25, initialFiles);  /**< Render the user interface with the list of files. */
    ui.displayContent(lines, row, col, scroll_row, scroll_col);  /**< Display the loaded content in the editor. */
}

/**
 * @brief Handles input from the user for both content and sidebar sections.
 * 
 * Based on which section of the screen is focused (content or sidebar), it processes the user's input 
 * accordingly and updates the display.
 * 
 * @param ch The character code representing the user's input.
 */
void TerminalEditor::handleInput(int ch) {
    if (focused_div == 0) {
        handleInputContent(ch);  /**< Handle input in the content area of the editor. */
        adjustCursorPosition();  /**< Adjust cursor position based on current content. */
        ui.displayContent(lines, row, col, scroll_row, scroll_col);  /**< Redraw the content after input. */
    } else {
        handleInputSidebar(ch);  /**< Handle input in the sidebar area. */
    }   
}

/**
 * @brief Handles input within the content area of the editor.
 * 
 * This function processes user input in the content section, including cursor movement, 
 * text insertion, text deletion, and file-saving commands.
 * 
 * @param ch The character code representing the user's input.
 */
void TerminalEditor::handleInputContent(int ch) {
    if (lines.empty()) {
        lines.push_back("");  /**< Ensure at least one line exists. */
    }

    switch (ch) {
        case KEY_UP: 
            if (row > 0) row--;  /**< Move the cursor up one line. */
            break;
        case KEY_DOWN: 
            if (row < lines.size() - 1) row++;  /**< Move the cursor down one line. */
            break;
        case KEY_LEFT: 
            if (col > 0) col--;  /**< Move the cursor left. */
            break;
        case KEY_RIGHT: 
            if (col < lines[row].length()) col++;  /**< Move the cursor right. */
            break;
        case KEY_BACKSPACE:
            if (col > 0) {  /**< Delete the character to the left of the cursor. */
                lines[row].erase(col - 1, 1);
                col--;
            } else if (row > 0) {  /**< Merge current line with previous line if cursor is at the beginning. */
                col = lines[row - 1].length();
                lines[row - 1] += lines[row];
                lines.erase(lines.begin() + row);
                row--;
            }
            break;
        case '\n': 
            lines.insert(lines.begin() + row + 1, lines[row].substr(col));  /**< Insert a new line at the current cursor position. */
            lines[row].resize(col);  /**< Resize the current line to the cursor position. */
            row++;  /**< Move cursor to the next line. */
            col = 0;  /**< Reset the column to the beginning of the new line. */
            break;
        case 19: // Ctrl+S
            fileManager.saveFile(fileManager.getFiles()[0], lines);  /**< Save the current file. */
            break;
        case KEY_F(1):
        case 17: // Ctrl+Q
            cleanup();  /**< Exit and save the current file. */
            break;
        case 2: // Ctrl+B - Insert ** for bold
            lines[row].insert(col, "**");
            col += 2;
            break;
        case 9: // Ctrl+I - Insert * for italic
            lines[row].insert(col, "*");
            col++;
            break;
        case 15: //Ctrl+O - Swap to sidebar control:
            focused_div = (focused_div + 1) % 2;
        default:
            if (ch >= 32 && ch <= 126) {  /**< Insert printable characters. */
                lines[row].insert(col, string(1, ch));
                col++;
            }
            break;
    }
}

/**
 * @brief Handles input in the sidebar section (currently not implemented).
 * 
 * This function is a placeholder for processing input related to sidebar navigation, 
 * such as selecting a file or performing actions related to the sidebar.
 * 
 * @param ch The character code representing the user's input.
 */
void TerminalEditor::handleInputSidebar(int ch) {
    // Sidebar navigation logic would go here in the future.

}

/**
 * @brief Adjusts the cursor position based on the current line and column.
 * 
 * This function ensures the cursor stays within the boundaries of the text content 
 * and adjusts the scroll positions when necessary to keep the cursor in view.
 */
void TerminalEditor::adjustCursorPosition() {
    if (row >= lines.size()) row = lines.size() - 1;  /**< Ensure row does not exceed the number of lines. */
    if (col > lines[row].length()) col = lines[row].length();  /**< Ensure column does not exceed the line length. */
    
    // Adjust scroll positions to keep the cursor visible.
    int max_lines = LINES - 4;
    int max_cols = COLS * 0.75 - 4;
    
    if (row < scroll_row) scroll_row = row;  /**< Adjust scroll_row if the cursor is above the visible area. */
    else if (row >= scroll_row + max_lines) scroll_row = row - max_lines + 1;  /**< Scroll down if the cursor goes beyond visible lines. */
    
    if (col < scroll_col) scroll_col = col;  /**< Adjust scroll_col if the cursor is too far left. */
    else if (col >= scroll_col + max_cols) scroll_col = col - max_cols + 1;  /**< Scroll right if the cursor goes beyond visible columns. */
}

/**
 * @brief Cleans up resources before exiting the editor.
 * 
 * This function saves the current file and cleans up the user interface before 
 * exiting the application.
 */
void TerminalEditor::cleanup() {
    fileManager.saveFile(fileManager.getFiles()[0], lines);  /**< Save the current file. */
    ui.cleanup();  /**< Clean up the UI (e.g., end ncurses session). */
    exit(0);  /**< Exit the program. */
}
