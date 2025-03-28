#include "TextPrompt.h"
#include <string>
#include <ncurses.h>
#include <algorithm>

TextPrompt::TextPrompt(WINDOW *win_in, std::string title_in){
    win = win_in;
    title = title_in;
}

/**
 * @brief Displays a text prompt in a window and returns the user input.
 * 
 * This function creates a temporary window for displaying a text prompt to the user, who can input text. 
 * The function supports cursor movement, backspace functionality, and scrolling when the input exceeds the visible space. 
 * The user can finalize their input by pressing the Enter key, which will return the input as a string.
 * 
 * @return A string containing the user input.
 */
std::string TextPrompt::prompt() {
    int height = 3;  ///< Height of the prompt window.
    int width = COLS / 2;  ///< Width of the prompt window, half the screen width.
    int start_y = (LINES - height) / 2;  ///< Vertical position of the prompt window, centered on the screen.
    int start_x = (COLS - width) / 2;  ///< Horizontal position of the prompt window, centered on the screen.

    // Create the prompt window
    WINDOW *prompt_win = derwin(win, height, width, start_y, start_x);
    if (!prompt_win) {
        return "";  ///< Return empty string if window creation fails.
    }

    // Draw the border and title
    box(prompt_win, 0, 0);
    mvwprintw(prompt_win, 1, 2, "%s", title.substr(0, std::max(0, width - 4)).c_str());
    wrefresh(win);
    wrefresh(prompt_win);

    // Cursor settings
    curs_set(1);  ///< Make the cursor visible.
    char input[256] = {0};  ///< Input buffer for storing user input.
    int input_len = 0;  ///< Length of the input text.
    int cursor_pos = 0;  ///< Position of the cursor.
    int view_start = 0;  ///< Scrolling position (text view start).
    const int max_visible = width - 5;  ///< Maximum number of characters visible in the prompt window.

    while (1) {
        werase(prompt_win);  ///< Clear the window for redrawing.
        box(prompt_win, 0, 0);  ///< Redraw the border.
        mvwprintw(prompt_win, 0, 2, "%s", title.substr(0, std::max(0, width - 4)).c_str());

        // Display the visible part of the input text
        // If the cursor is near the right edge of the visible area, scroll to the right.
        if (cursor_pos - view_start >= max_visible) {
            view_start = cursor_pos - max_visible + 1;  // Adjust view_start so the cursor is visible.
        }

        // If the cursor is near the left edge of the visible area, scroll to the left.
        if (cursor_pos - view_start < 0) {
            view_start = cursor_pos;  // Adjust view_start so the cursor is at the start of the visible area.
        }

        // Now, print the visible portion of the input text based on the adjusted view_start.
        if (input_len - view_start > max_visible) {
            mvwprintw(prompt_win, 1, 2, "%.*s", max_visible, &input[view_start]);
        } else {
            mvwprintw(prompt_win, 1, 2, "%s", &input[view_start]);
        }

        // Calculate the cursor's x position
        int cursor_x = 2 + cursor_pos - view_start;
        if (cursor_x >= max_visible) cursor_x = max_visible + 1;
        wmove(prompt_win, 1, cursor_x);  ///< Move the cursor.
        wrefresh(prompt_win);  ///< Refresh the window to show updates.

        int ch = getch();  ///< Capture the user input.

        switch (ch) {
            case '\n':  ///< Enter key pressed, return the input text.
                delwin(prompt_win);
                curs_set(0);  ///< Hide the cursor.
                return std::string(input);
            
            case KEY_BACKSPACE:
            case 127:  ///< Handle backspace.
                if (cursor_pos > 0) {
                    // Shift characters left if there's text to delete.
                    for (int i = cursor_pos - 1; i < input_len - 1; i++) {
                        input[i] = input[i + 1];
                    }
                    input[--input_len] = '\0';  ///< Null-terminate the string.
                    cursor_pos--;  ///< Move the cursor left.
                }
                break;

            case KEY_RIGHT:  ///< Right arrow key.
                if (cursor_pos < input_len) {
                    cursor_pos++;  ///< Move cursor right.
                }
                break;

            case KEY_LEFT:  ///< Left arrow key.
                if (cursor_pos > 0) {
                    cursor_pos--;  ///< Move cursor left.
                } else if (view_start > 0) {
                    view_start--;  ///< Scroll input text to the left.
                }
                break;

            default:  ///< Handle regular characters.
                if (ch >= 32 && ch <= 126 && input_len < sizeof(input) - 1) {
                    // Insert the character at the cursor position.
                    for (int i = input_len; i > cursor_pos; i--) {
                        input[i] = input[i - 1];
                    }
                    input[cursor_pos++] = (char)ch;
                    input_len++;
                    input[input_len] = '\0';  ///< Null-terminate the string.

                    // Scroll input text if necessary when the cursor goes out of view.
                    if (cursor_pos - view_start >= max_visible && view_start < input_len - max_visible) {
                        view_start++;  ///< Scroll text to the right.
                    }
                }
                break;
        }
    }

    delwin(prompt_win);  ///< Delete the prompt window after use.
    curs_set(0);  ///< Hide the cursor.
    return std::string(input);  ///< Return the input string.
}