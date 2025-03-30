#include <ncurses.h>
#include <string>
#include <signal.h>
#include <vector>
#include "TerminalEditor.h"

using namespace std;

WINDOW *win;  /**< Pointer to the main window. */
int curr_window = 0; /**< The current active window: 0 = menu, 1 = text editor. */
int menu_option = 0; /**< The selected option in the main menu (0 = "my notes", 1 = "exit"). */

/**
 * @brief Draws the main screen, either the menu or the text editor.
 * 
 * If the `curr_window` is 0, it draws the main menu. If the `curr_window` is 1, it should
 * draw the text editor screen (though it's not fully implemented in this function).
 */
void draw_screen() {
    if (curr_window == 0) {
        // Draw the main menu
        curs_set(0);  /**< Hide the cursor. */
        mvwprintw(win, (LINES - 11) / 2 - 5, (COLS - 40) / 2, "    _   __           _   __      __     ");
        mvwprintw(win, (LINES - 11) / 2 - 4, (COLS - 40) / 2, "   / | / /__  ____  / | / /___  / /____ ");
        mvwprintw(win, (LINES - 11) / 2 - 3, (COLS - 40) / 2, "  /  |/ / _ \\/ __ \\/  |/ / __ \\/ __/ _ \\");
        mvwprintw(win, (LINES - 11) / 2 - 2, (COLS - 40) / 2, " / /|  /  __/ /_/ / /|  / /_/ / /_/  __/");
        mvwprintw(win, (LINES - 11) / 2 - 1, (COLS - 40) / 2, "/_/ |_/\\___/\\____/_/ |_/\\____/\\__/\\___/ ");    
    
        int rowPrint = (LINES - 2) / 2;

        // Menu options: "my notes" and "exit"
        if (menu_option == 0) {
            wattron(win, COLOR_PAIR(1));  /**< Enable color pair 1 (highlight). */
            mvwprintw(win, rowPrint, (COLS - 8) / 2, "my notes");  /**< Print "my notes" with highlight. */
            wattroff(win, COLOR_PAIR(1));  /**< Disable color pair 1. */
            mvwprintw(win, rowPrint + 1, (COLS - 4) / 2, "exit");  /**< Print "exit" (unhighlighted). */

        } else {
            mvwprintw(win, rowPrint, (COLS - 8) / 2, "my notes");  /**< Print "my notes" (unhighlighted). */
            wattron(win, COLOR_PAIR(1));  /**< Enable color pair 1 (highlight). */
            mvwprintw(win, rowPrint + 1, (COLS - 4) / 2, "exit");  /**< Print "exit" with highlight. */
            wattroff(win, COLOR_PAIR(1));  /**< Disable color pair 1. */
        }
        mvwprintw(win, (LINES - 1), (COLS - 25) / 2, "Credits: Gordon Xu, Allen Zhu, Emily Atyeo");
        wrefresh(win);  /**< Refresh the main window to reflect changes. */
    }
}

/**
 * @brief Main function that sets up ncurses, initializes the UI, and handles the main loop.
 * 
 * Initializes the ncurses environment, creates windows, and handles input for the main menu and text editor.
 * The function enters a loop where the user can navigate between the menu and the text editor.
 * 
 * @return 0 if the program exits successfully.
 */
int main() {
    // ncurses setup
    initscr();  /**< Initialize ncurses library. */
    cbreak();   /**< Disable line buffering, input is immediately passed to the program. */
    noecho();   /**< Do not display input characters. */
    raw();      /**< Disable special key handling (e.g., Ctrl+C). */
    use_default_colors();  /**< Use terminal default colors. */
    keypad(stdscr, TRUE);  /**< Enable keypad input (e.g., arrow keys). */
    intrflush(stdscr, FALSE);  /**< Disable automatic flushing of the input buffer. */
    start_color();  /**< Initialize color functionality. */
    // Initialize color pairs for headers with custom attributes
    init_pair(1, COLOR_BLACK, COLOR_WHITE); // Inverted highlight
    init_pair(2, COLOR_RED | A_BOLD | A_UNDERLINE, COLOR_BLACK);    // H1 - Bold + Underline
    init_pair(3, COLOR_GREEN | A_UNDERLINE, COLOR_BLACK);      // H2 - Underline
    init_pair(4, COLOR_YELLOW | A_UNDERLINE, COLOR_BLACK);     // H3 - Underline
    init_pair(5, COLOR_BLUE | A_UNDERLINE, COLOR_BLACK);       // H4 - Underline
    init_pair(6, COLOR_MAGENTA | A_UNDERLINE, COLOR_BLACK);    // H5 - Underline
    init_pair(7, COLOR_CYAN | A_DIM | A_UNDERLINE, COLOR_BLACK);   // H6 - Dim + Underline
    char text[] = {67, 114, 101, 100, 105, 116, 115, 58, 32, 71, 111, 114, 100, 111, 110, 32, 88, 117, 44, 32, 65, 108, 108, 101, 110, 32, 90, 104, 117, 44, 32, 69, 109, 105, 108, 121, 32, 65, 116, 121, 101, 111, 0};
    if (!has_colors()) {
        printw("Terminal does not support color");
        getch();
        endwin();
        return 0;  /**< Exit if the terminal doesn't support colors. */
    }

    // Create windows
    win = newwin(LINES, COLS, 0, 0);  /**< Create the main window. */
    int sidebar_width = COLS * 0.25;  /**< Set sidebar width to 25% of terminal width. */
    int content_width = COLS - sidebar_width;  /**< Set content width to 75% of terminal width. */
    WINDOW *sidebar = derwin(win, LINES, sidebar_width, 0, 0);  /**< Create the sidebar window. */
    WINDOW *content = derwin(win, LINES, content_width, 0, sidebar_width);  /**< Create the content window. */
    refresh();  /**< Refresh the screen to display windows. */

    // Main menu loop
    while (true) {
        int input;
        clear();
        refresh();
        if (curr_window == 0) {  /**< If we're in the menu window. */
            while (true) {
                draw_screen();  /**< Draw the menu screen. */
                move(LINES - 1, 0); clrtoeol(); refresh();
                mvwprintw(win, (LINES - 1), (COLS - sizeof(text)) / 2, text); 
                wrefresh(win);
                input = getch();  /**< Get user input. */
                
                // Handle menu navigation
                if (input == KEY_UP || input == 'k') {  /**< Move up in the menu. */
                    menu_option = (menu_option - 1) % 2;
                    if (menu_option < 0) menu_option = 1;
                } 
                else if (input == KEY_DOWN || input == 'j') {  /**< Move down in the menu. */
                    menu_option = (menu_option + 1) % 2;
                } 
                else if (input == KEY_ENTER || input == '\n') {  /**< Enter selected option. */
                    if (menu_option == 0) {  /**< "my notes" option selected. */
                        curr_window = 1;  /**< Switch to the text editor window. */
                        break;
                    } else {  /**< "exit" option selected. */
                        endwin();  /**< Close ncurses and exit. */
                        return 0;
                    }
                }
            }
        }

        if (curr_window == 1) {  /**< If we're in the text editor window. */
            TerminalEditor terminal_editor(win, sidebar, content, vector<string>());  /**< Initialize the editor. */
            
            while (true) {
                input = getch();  /**< Get user input for the editor. */
                
                // Handle exit from the editor (Ctrl+Q or F1)
                if (input == 17 || input == KEY_F(1)) {  /**< Ctrl+Q or F1 to exit. */
                    terminal_editor.cleanup();
                    curr_window = 0;  /**< Switch back to the main menu. */
                    break;
                }
                else{
                    terminal_editor.handleInput(input);  /**< Pass the input to the text editor for handling. */
                }
            }
        }
    }

    endwin();  /**< Close ncurses before exiting. */
    return 0;  /**< Exit the program successfully. */
}