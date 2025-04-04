#include "TerminalEditor.h"
#include "Settings.h"
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
    else if (focused_div == 2) { //**< 2 = handleinputkanban */
        handleInputKanban(ch);
    }
    else if (focused_div == 3) { //**< 3 = handleinputcalendar */
        handleInputCalendar(ch);
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
        case CURSOR_UP: 
            if (row > 0) row--;  /**< Move the cursor up one line. */
            break;
        case CURSOR_DOWN: 
            if (row < lines.size() - 1) row++;  /**< Move the cursor down one line. */
            break;
        case CURSOR_LEFT: 
            if (col > 0) {
                col--;  /**< Move the cursor left. */
            } else if (row > 0) {
                // At beginning of line, move to end of previous line
                row--;
                col = lines[row].length();
            }
            break;
        case CURSOR_RIGHT: 
            if (col < lines[row].length()) {
                col++;  /**< Move the cursor right. */
            } else if (row < lines.size() - 1) {
                // At end of line, move to beginning of next line
                row++;
                col = 0;
            }
            break;
        case DELETE_RIGHT:
	    if (col < lines[row].length()) {  /**< Delete the character at the cursor position. */
                lines[row].erase(col, 1);
            } else if (row < lines.size() - 1) {  /**< Merge with next line if at end of current line. */
                lines[row] += lines[row + 1];
                lines.erase(lines.begin() + row + 1);
            }
            break;
        case DELETE_LEFT:
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
        case SKIP_LEFT:
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

        case SKIP_RIGHT: // Ctrl + ]
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
                    lines[row].replace(numStart, col - numStart, INDENTATION + numberPart + " ");
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
	case INDENT:
        case INDENT_ALT:
            lines[row].insert(col, INDENTATION);
            col += 4;
            break;
        case NEW_LINE: 
            lines.insert(lines.begin() + row + 1, lines[row].substr(col));  /**< Insert a new line at the current cursor position. */
            lines[row].resize(col);  /**< Resize the current line to the cursor position. */
            row++;  /**< Move cursor to the next line. */
            col = 0;  /**< Reset the column to the beginning of the new line. */
            break;
        case GOTO_FILE_START: // Ctrl+F - Go to front of file
            row = 0;
            col = 0;
            break;
        case GOTO_FILE_END: // Ctrl+T - Go to tail of file
            row = lines.size() - 1;
            col = lines[row].length();
            break;
        case SAVE_FILE: // Ctrl+S
            fileManager.saveFile(current_file, lines);  /**< Save the current file. */
            break;
        case BOLD: // Ctrl+B - Handle bold markdown
            if (col + 2 <= lines[row].length() && lines[row].substr(col, 2) == "**") {
                // If we're at existing **, just move cursor right past them
                col += 2;
            } else {
                // First press: insert **** and move left 2 positions
                lines[row].insert(col, "****");
                col += 2; // Position cursor in the middle
            }
            break;
        case ITALIC: // Ctrl+I - Handle italic markdown
            if (col < lines[row].length() && lines[row][col] == '*') {
                // If we're at existing *, just move cursor right past it
                col++;
            } else {
                // Insert * and stay at position (user types between the asterisks)
                lines[row].insert(col, "**");
		col += 1;
            }
            break;
        case DELETE_WORD:  // Ctrl+Backspace - delete work
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
	case NEW_FILE: // Ctrl+N - New file
            fileManager.newFile(); /**< Push new file to files vector. */
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case SWITCH_PANEL:
        case SWITCH_PANEL_ALT:
        // Ctrl+O or Ctrl+D - Swap to sidebar control:
            last_focused_div = focused_div;
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
        case SWITCH_PANEL:
        case SWITCH_PANEL_ALT:
            if (sidebar_index < fileManager.getFiles().size()){
                last_focused_div = focused_div;
                focused_div = 0; /**< Flip focused_div to content.  */
                curs_set(1);
                fileManager.saveFile(current_file, lines);
                fileManager.loadFile(fileManager.getFiles()[sidebar_index], lines, current_file);
                adjustCursorPosition();
                ui.displayContent(lines, row, col, scroll_row, scroll_col, fileManager.getFiles()[sidebar_index]);
            }
            else if (sidebar_index == fileManager.getFiles().size()){
                //kanban swap
                taskManager.swapIn();
                taskManager.renderTasks();
                last_focused_div = focused_div;
                focused_div = 2; /**< Flip focused_div to kanban. */
            }
            else{
                //calendar swap
                calendar.setSelectedEvent(0);
                calendar.renderCalendar();
                last_focused_div = focused_div;
                focused_div = 3; /**< Flip focused_div to calendar. */
            }
            break;
        case CURSOR_UP: 
            sidebar_index = (sidebar_index - 1 + len_files) % len_files;
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case CURSOR_DOWN: 
            sidebar_index = std::min(sidebar_index + 1, len_files) % len_files; 
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case NEW_FILE: // Ctrl+N - New file
            fileManager.newFile(); /**< Push new file to files vector. */
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case RENAME_FILE:
            if(sidebar_index < fileManager.getFiles().size()){
                fileManager.saveFile(current_file, lines);
                fileManager.loadFile(fileManager.getFiles()[sidebar_index], lines, current_file);
                input = ui.displayPrompt("Rename note");
                while (input.empty()) {
                    input = ui.displayPrompt("Rename note (Field cannot be empty)");
                }
                fileManager.renameFile(fileManager.getFiles()[sidebar_index], input, current_file);
                ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
                ui.displayContent(lines, row, col, scroll_row, scroll_col, fileManager.getFiles()[sidebar_index]);
            }
            break;
        case DELETE_FILE:
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
        case CONFIRM_OPTION:
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
                calendar.setSelectedEvent(-1);
                calendar.renderCalendar();
            }
            break;
    }
}

/**
 * @brief Handles keyboard input for the Kanban task manager interface.
 * 
 * This function processes various key inputs to perform actions such as:
 * - Swapping the focus between divisions.
 * - Adding, moving, or deleting tasks.
 * - Navigating through the task list.
 * - Rendering the task manager and sidebar display.
 * 
 * @param ch The key code representing the user input.
 */
void TerminalEditor::handleInputKanban(int ch){
    string input;
    switch(ch){
        case SWITCH_PANEL:
        case SWITCH_PANEL_ALT:
            last_focused_div = focused_div;
            taskManager.swapOut();
            taskManager.renderTasks();
            focused_div = 1; /**< Flip focused_div. */
            break;
        case NEW_FILE: // Ctrl+N - Add new task
            input = ui.displayPrompt("Enter new task:");
            while (input.empty()){
                input = ui.displayPrompt("Enter new task: (Field cannot be empty)");
            }
            taskManager.addTask(input, 0); /**< Add new task to task manager. */
            taskManager.renderTasks();  /**< Refresh task display. */
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case CURSOR_UP:
            taskManager.moveSelection(0);
            taskManager.renderTasks();  /**< Refresh task display. */
            break;
        case CURSOR_DOWN:
            taskManager.moveSelection(1);
            taskManager.renderTasks();  /**< Refresh task display. */
            break;
        case MOVE_TASK_LEFT:
            taskManager.moveSelection(2);
            taskManager.renderTasks();  /**< Refresh task display. */
            break;
        case MOVE_TASK_RIGHT:
            taskManager.moveSelection(3);
            taskManager.renderTasks();  /**< Refresh task display. */
            break;
        case CONFIRM_OPTION: // Enter to move task
            taskManager.moveTaskPopup(taskManager.getSelectedTaskId());
            taskManager.renderTasks();  /**< Refresh task display. */
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case DELETE_FILE:
            if(taskManager.getSelectedTaskId() == -1){break;}
            input = ui.displayPrompt("Delete permanently? (Y/N)");
            if(input == "Y" || input == "y"){
                taskManager.removeTask(taskManager.getSelectedTaskId());
            }
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            taskManager.renderTasks();  /**< Refresh task display. */
            refresh();
            break;
    }
}

/**
 * @brief Handles keyboard input for the calendar interface.
 * 
 * This function processes various key inputs to:
 * - Switch the focus between divisions.
 * - Add new events with a title, description, and date.
 * - Navigate through the list of events.
 * - Delete events permanently.
 * - Render the calendar and sidebar display.
 * 
 * @param ch The key code representing the user input.
 */
void TerminalEditor::handleInputCalendar(int ch){
    string event_title, event_desc, event_date, input;
    switch(ch){
        case SWITCH_PANEL:
        case SWITCH_PANEL_ALT:
            last_focused_div = focused_div;
            focused_div = 1; /**< Flip focused_div. */
            calendar.setSelectedEvent(-1);
            calendar.renderCalendar();
            break;
        case NEW_FILE:
            event_title = ui.displayPrompt("Event Name");
            while (event_title.empty()) {
                event_title = ui.displayPrompt("Event Name (Field cannot be empty)");
            }
            
            event_desc = ui.displayPrompt("Event Description");
            while (event_desc.empty()) {
                event_desc = ui.displayPrompt("Event Description (Field cannot be empty)");
            }
            
            event_date = ui.displayPrompt("Event Date");
            while (event_date.empty()) {
                event_date = ui.displayPrompt("Event Date (Field cannot be empty)");
            }
            calendar.addEvent(Event(calendar.nextFree(), event_title, event_desc, event_date));
            calendar.renderCalendar();
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            break;
        case CURSOR_UP:
            calendar.setSelectedEvent(std::max(0, calendar.getSelectedEvent() - 1));
            calendar.renderCalendar();
            break;
        case CURSOR_DOWN:
            calendar.setSelectedEvent(std::min(calendar.getEvents().size() - 1, static_cast<size_t>(calendar.getSelectedEvent()) + 1));
            calendar.renderCalendar();
            break;
        case DELETE_FILE:
            input = ui.displayPrompt("Delete permanently? (Y/N)");
            if(input == "Y" || input == "y"){
                calendar.removeEvent(calendar.getSelectedEvent());
            }
            ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);
            calendar.renderCalendar();
            refresh();
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
 * @brief Redraws the terminal editor interface.
 * 
 * This function adjusts the window sizes based on the current terminal dimensions,
 * repositions the sidebar and content windows, and renders the appropriate section
 * based on the current focus:
 * - **Content editor**: Displays the current file with cursor visibility.
 * - **Task manager**: Renders the task list.
 * - **Calendar**: Displays the calendar view.
 * 
 * It also ensures the sidebar and main content are properly refreshed.
 */
void TerminalEditor::redraw(int sidebar_width) {

    ui.renderUI(sidebar_width, fileManager.getFiles());

    if (focused_div == 0 || last_focused_div == 0){
        ui.displayContent(lines, row, col, scroll_row, scroll_col, current_file);
    }else if (focused_div == 2 || last_focused_div == 2){
        taskManager.renderTasks();
    }
    else if (focused_div == 3 || last_focused_div == 3){
        calendar.renderCalendar();
    } 

    ui.renderSidebar(sidebar_width, fileManager.getFiles(), sidebar_index);

    curs_set(focused_div == 0 ? 1 : 0); // Show cursor only in content editor
    if (focused_div == 0) {
        wmove(ui.getContent(), row - scroll_row, col - scroll_col);
    }

    wrefresh(ui.getMainWindow());
    wrefresh(ui.getSidebar());
    wrefresh(ui.getContent());
    refresh();
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
