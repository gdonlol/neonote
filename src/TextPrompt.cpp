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

    // Create a new window for the prompt with specified dimensions and position.
    WINDOW *prompt_win = derwin(win, height, width, start_y, start_x);

    if (!prompt_win) {
        return "";  ///< Return empty string if window creation fails.
    }

    // Set up the border and title for the prompt window.
    box(prompt_win, 0, 0);
    mvwprintw(prompt_win, 1, 2, "%s", title.substr(0, std::max(0, width - 4)).c_str());
    mvwprintw(prompt_win, 2, 2, "Input: ");
    wrefresh(win);
    wrefresh(prompt_win);
    curs_set(1);  ///< Make the cursor visible.

    char input[256] = {0};  ///< Buffer to hold the user input.
    int input_len = 0;  ///< Current length of the input string.
    int cursor_pos = 0;  ///< Position of the cursor within the input string.
    int ch;  ///< Variable to store user input character.
    int view_start = 0;  ///< Starting position for scrolling input text.
    const int max_visible = width - 4;  ///< Maximum number of characters visible in the input area.

    while (1) {
        werase(prompt_win);  ///< Clear the prompt window for redrawing.
        box(prompt_win, 0, 0);  ///< Redraw the border around the window.
        mvwprintw(prompt_win, 0, 2, "%s", title.substr(0, std::max(0, width - 4)).c_str());

        // Print the input string, applying scrolling logic when necessary.
        if (input_len > max_visible) {
            mvwprintw(prompt_win, 1, 2, "%s", &input[view_start]);  ///< Display scrolled input.
        } else {
            mvwprintw(prompt_win, 1, 2, "%s", input);  ///< Display the full input if it fits.
        }

        // Move the cursor to the correct position within the input text.
        int cursor_x = 2 + cursor_pos - view_start;  ///< Calculate horizontal position of the cursor.
        wmove(prompt_win, 1, cursor_x);  ///< Move the cursor within the window.
        wrefresh(prompt_win);  ///< Refresh the window to reflect the changes.

        ch = getch();  ///< Wait for user input and capture the pressed key.

        switch (ch) {
            case '\n':  ///< Enter key pressed, finalize input and exit the loop.
                delwin(prompt_win);  ///< Delete the prompt window.
                curs_set(0);  ///< Hide the cursor.
                return std::string(input);  ///< Return the input string.
                break;

            case KEY_BACKSPACE:
            case 127:  ///< Handle backspace key for text deletion.
                if (input_len > 0 && cursor_pos > 0) {
                    // Shift the characters left from the cursor position.
                    for (int i = cursor_pos - 1; i < input_len - 1; i++) {
                        input[i] = input[i + 1];  ///< Shift characters left.
                    }
                    input[--input_len] = '\0';  ///< Null-terminate the string.
                    cursor_pos--;  ///< Move the cursor left.
                }
                break;

            case KEY_RIGHT:  ///< Handle right arrow key for cursor movement.
                if (cursor_pos < input_len) {
                    cursor_pos++;  ///< Move cursor right within the input string.
                } else if (cursor_pos < input_len && input_len > max_visible) {
                    // Scroll input if cursor reaches the end of visible text.
                    if (view_start < input_len - max_visible) {
                        view_start++;  ///< Scroll input to the right.
                    }
                }
                break;

            case KEY_LEFT:  ///< Handle left arrow key for cursor movement.
                if (cursor_pos > 0) {
                    cursor_pos--;  ///< Move cursor left within the input string.
                } else if (view_start > 0) {
                    // Scroll input if cursor reaches the start of visible text.
                    view_start--;  ///< Scroll input to the left.
                }
                break;

            default:  ///< Handle printable characters.
                if (ch >= 32 && ch <= 126) {
                    if (input_len < sizeof(input) - 1) {
                        // Insert character at the current cursor position.
                        for (int i = input_len; i > cursor_pos; i--) {
                            input[i] = input[i - 1];  ///< Shift characters to the right.
                        }
                        input[cursor_pos++] = (char)ch;  ///< Insert the new character.
                        input_len++;  ///< Increment the length of the input.
                        input[input_len] = '\0';  ///< Null-terminate the string.

                        // Handle scrolling when the input exceeds the visible space.
                        if (input_len > max_visible) {
                            if (view_start < input_len - max_visible) {
                                view_start++;  ///< Scroll input to the right if needed.
                            }
                        }
                    }
                }
                break;
        }
    }

    std::string result = input;  ///< Finalize the input as a string.
    curs_set(0);  ///< Hide the cursor.
    delwin(prompt_win);  ///< Delete the prompt window after usage.
    return result;  ///< Return the user input as a string.
}