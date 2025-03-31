#include "TerminalEditor.h"
#include <string>
#include <algorithm>

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
    : fileManager(), ui(win_in, sidebar_in, content_in), calendar(content_in), taskManager(content_in),
      row(0), col(0), scroll_row(0), scroll_col(0), focused_div(0), sidebar_index(0), sidebar_width(COLS * 0.25){

    // Load initial file from file manager
    std::vector<std::string> initialFiles = fileManager.getFiles();
    if (!initialFiles.empty()) {
        current_file = initialFiles[0];
        fileManager.loadFile(initialFiles[0], lines, current_file);  /**< Load the first file into lines. */
    } else {
        lines.push_back("");  /**< If no files exist, start with an empty line. */
    }

    curs_set(1);  /**< Show the cursor in the terminal editor. */

    // Render the initial UI and display the loaded content
    ui.renderUI(sidebar_width, initialFiles);  /**< Render the user interface with the list of files. */
    ui.displayContent(lines, row, col, scroll_row, scroll_col, fileManager.getFiles()[sidebar_index]);  /**< Display the loaded content in the editor. */
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
    if (focused_div == 0) { //**< 0 = content */
        handleInputContent(ch);  /**< Handle input in the content area of the editor. */
        adjustCursorPosition();  /**< Adjust cursor position based on current content. */
        ui.displayContent(lines, row, col, scroll_row, scroll_col, fileManager.getFiles()[sidebar_index]);  /**< Redraw the content after input. */
    } else if (focused_div == 1) { //**< 1 = sidebar */
        handleInputSidebar(ch);  /**< Handle input in the sidebar area. */
    }
    else if (focused_div == 2) { //**< 2 = kanban */
        string task_title, task_desc, task_due_date;
        switch(ch) {
            case 15:
            case 4: // Switch focus
                focused_div = 1; 
                break;
            case 14: // Add a new task
                task_title = ui.displayPrompt("Task Name");
                task_desc = ui.displayPrompt("Task Description");
                task_due_date = ui.displayPrompt("Task Due Date");
                Task new_task(taskManager.nextFree(), task_title, "To Do", task_desc, task_due_date); 
                taskManager.addTask(new_task);
                taskManager.renderTasks();
                ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
                break;
        }
    }
    else if (focused_div == 3) { //**< 3 = calendar */
        string event_title, event_desc, event_date;
        switch(ch){
            case 15:
            case 4:
                focused_div = 1; /**< Flip focused_div. */
                break;
            case 14:
                event_title = ui.displayPrompt("Event Name");
                event_desc = ui.displayPrompt("Event Description");
                event_date = ui.displayPrompt("Event Date");
                Event event_in(calendar.nextFree(), event_title, event_desc, event_date);
                calendar.addEvent(event_in);
                calendar.renderCalendar();
                ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            }
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
        case 127:
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
        case 29: // Ctrl + ]
            if (col > 0) {
                // Skip any spaces immediately left
                while (col > 0 && lines[row][col-1] == ' ') {
                    col--;
                }
                // Jump to start of word
                while (col > 0 && lines[row][col-1] != ' ') {
                    col--;
                }
            }
            break;

        case 28: // Ctrl + "\"
            if (col < lines[row].length()) {
                // Skip current word
                while (col < lines[row].length() && lines[row][col] != ' ') {
                    col++;
                }
                // Skip spaces to next word
                while (col < lines[row].length() && lines[row][col] == ' ') {
                    col++;
                }
            }
            break;

        case ' ': {
            bool shouldExpand = false;
            int numStart = col - 1;
            
            while (numStart > 0 && isdigit(lines[row][numStart - 1])) {
                numStart--;
            }
            
            if (numStart < col - 1 && lines[row][col - 1] == '.') {
                shouldExpand = true;
                for (int i = 0; i < numStart; ++i) {
                    if (lines[row][i] != ' ') {
                        shouldExpand = false;
                        break;
                    }
                }
                
                if (shouldExpand) {
                    string numberPart = lines[row].substr(numStart, col - numStart);
                    lines[row].replace(numStart, col - numStart, "    " + numberPart + " ");
                    col = numStart + 5 + numberPart.length();
                }
            }
            else if (col >= 1 && lines[row][col - 1] == '-') {
                shouldExpand = true;
                for (int i = 0; i < col - 1; ++i) {
                    if (lines[row][i] != ' ') {
                        shouldExpand = false;
                        break;
                    }
                }
                if (shouldExpand) {
                    lines[row].replace(col - 1, 1, "    - ");
                    col += 5;
                }
            }

            if (!shouldExpand) {
                lines[row].insert(col, " ");
                col++;
            }
            break;
        }
	case KEY_CTAB:
        case KEY_BTAB:
            lines[row].insert(col, "    ");
            col += 4;
            break;
        case '\n': 
            lines.insert(lines.begin() + row + 1, lines[row].substr(col));  /**< Insert a new line at the current cursor position. */
            lines[row].resize(col);  /**< Resize the current line to the cursor position. */
            row++;  /**< Move cursor to the next line. */
            col = 0;  /**< Reset the column to the beginning of the new line. */
            break;
        case 19: // Ctrl+S
            fileManager.saveFile(current_file, lines);  /**< Save the current file. */
            break;
        case 2: // Ctrl+B - Handle bold markdown
            if (col + 2 <= lines[row].length() && lines[row].substr(col, 2) == "**") {
                // If we're at existing **, just move cursor right past them
                col += 2;
            } else {
                // First press: insert **** and move left 2 positions
                lines[row].insert(col, "****");
                col += 2; // Position cursor in the middle
            }
            break;
        case 9: // Ctrl+I - Handle italic markdown
            if (col < lines[row].length() && lines[row][col] == '*') {
                // If we're at existing *, just move cursor right past it
                col++;
            } else {
                // Insert * and stay at position (user types between the asterisks)
                lines[row].insert(col, "**");
		col += 1;
            }
            break;
        case 8:  // Ctrl+Backspace - delete work
        case KEY_DC:
            if (col > 0) {
                int word_start = col - 1;
                // Skip trailing spaces
                while (word_start >= 0 && lines[row][word_start] == ' ') {
                    word_start--;
                }
                // Skip the word itself
                while (word_start >= 0 && lines[row][word_start] != ' ') {
                    word_start--;
                }
                lines[row].erase(word_start + 1, col - (word_start + 1));
                col = word_start + 1;
            }
            break;
	case 14: // Ctrl+N - New file
            fileManager.newFile(); /**< Push new file to files vector. */
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case 15:
        case 4:
        // Ctrl+O or Ctrl+D - Swap to sidebar control:
            focused_div = 1; /**< Flip focused_div. */
            curs_set(0);
            break;
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
    int len_files = fileManager.getFiles().size() + 2;
    std::string input; 
    switch (ch) {
        case 15:
        case 4:
            if (sidebar_index < fileManager.getFiles().size()){
                focused_div = 0; /**< Flip focused_div to content.  */
                curs_set(1);
                ui.displayContent(lines, row, col, scroll_row, scroll_col, fileManager.getFiles()[sidebar_index]);
            }
            else if (sidebar_index == fileManager.getFiles().size()){
                //kanban swap
                focused_div = 2; /**< Flip focused_div to kanban. */
            }
            else{
                //calendar swap
                focused_div = 3; /**< Flip focused_div to calendar. */
            }
            break;
        case KEY_UP: 
            sidebar_index = (sidebar_index - 1 + len_files) % len_files;
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case KEY_DOWN: 
            sidebar_index = std::min(sidebar_index + 1, len_files) % len_files; 
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case 14: // Ctrl+N - New file
            fileManager.newFile(); /**< Push new file to files vector. */
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case KEY_F(2):
        case 18:
            fileManager.saveFile(current_file, lines);
            input = ui.displayPrompt("Rename note");
            fileManager.renameFile(fileManager.getFiles()[sidebar_index], input, current_file);
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            ui.displayContent(lines, row, col, scroll_row, scroll_col, fileManager.getFiles()[sidebar_index]);
            break;
        case 20: // Ctrl+T - Add new task
            input = ui.displayPrompt("Enter new task:");
            if (!input.empty()) {
                taskManager.addTask(input); /**< Add new task to task manager. */
                taskManager.renderTasks();  /**< Refresh task display. */
            }
            break;
        case KEY_DC:
            if(fileManager.getFiles().size() > 1){
                input = ui.displayPrompt("Delete permanently? (Y/N)");
                if(input == "Y" || input == "y"){
                    fileManager.deleteFile(fileManager.getFiles()[sidebar_index]);
                    sidebar_index = std::max(sidebar_index - 1, 0);
                    fileManager.loadFile(fileManager.getFiles()[sidebar_index], lines, current_file);
                    adjustCursorPosition();  /**< Adjust cursor position based on current content. */
                }
            }
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            ui.displayContent(lines, row, col, scroll_row, scroll_col, fileManager.getFiles()[sidebar_index]);
            refresh();
            break;
        case '\n':
            if (sidebar_index < fileManager.getFiles().size()){
                fileManager.saveFile(current_file, lines);
                fileManager.loadFile(fileManager.getFiles()[sidebar_index], lines, current_file);
                adjustCursorPosition();  /**< Adjust cursor position based on current content. */
                ui.displayContent(lines, row, col, scroll_row, scroll_col, fileManager.getFiles()[sidebar_index]);  /**< Redraw the content after input. */
            }
            else if (sidebar_index == fileManager.getFiles().size()){
                //render kanban here
                taskManager.renderTasks();
            }
            else {
                //render calendar here
                calendar.renderCalendar();
            }
            break;
    }
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
    int max_cols = COLS - sidebar_width - 4;
    
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
    fileManager.saveFile(current_file, lines);  /**< Save the current file. */
    ui.cleanup();  /**< Clean up the UI (e.g., end ncurses session). */
}
