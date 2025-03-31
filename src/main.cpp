#include <ncurses.h>
#include <string>
#include <signal.h>
#include <vector>
#include "TerminalEditor.h"
#include "MainMenu.h"
#include "NcursesSetup.h"

using namespace std;

/**
 * @brief Main function that initializes the application and runs the main loop.
 * 
 * This function sets up the ncurses environment, creates the main window and sidebar, and handles 
 * the resizing of the terminal. It also manages the flow between the main menu and the editor window 
 * based on user input. The program runs continuously until the user exits.
 * 
 * @return 0 if the program exits successfully. Returns 1 if initialization fails.
 */
int main() {
    // Initialize ncurses
    NcursesSetup ncurses;
    if (!ncurses.initialize()) {
        return 1;
    }

    // Create windows
    WINDOW* win = newwin(LINES, COLS, 0, 0);
    int sidebar_width = COLS * 0.25;
    int content_width = COLS - sidebar_width;
    WINDOW* sidebar = derwin(win, LINES, sidebar_width, 0, 0);
    WINDOW* content = derwin(win, LINES, content_width, 0, sidebar_width);
    refresh();

    // Initialize main menu
    MainMenu mainMenu(win);

    int prev_cols = COLS;
    int prev_lines = LINES;

    // Main application loop
    while (true) {
        int current_cols, current_lines;
        getmaxyx(stdscr, current_lines, current_cols);

        if (current_cols != prev_cols || current_lines != prev_lines) {
            clear(); // Clear the screen
	    wclear(win);
            refresh();

            sidebar_width = current_cols * 0.25;
            content_width = current_cols - sidebar_width;

            wresize(sidebar, current_lines, sidebar_width);
            wresize(content, current_lines, content_width);
            mvwin(sidebar, 0, 0);
            mvwin(content, 0, sidebar_width);

            // Update size
            prev_cols = current_cols;
            prev_lines = current_lines;

            // Redraw
            mainMenu.display();
        }

        // Get the current window
        int currentWindow = mainMenu.getCurrentWindow();

        if (currentWindow == 0) { // Menu window
            mainMenu.display();
            int input = getch();
            mainMenu.handleInput(input);

            if (mainMenu.shouldExit()) {
                break;
            }
        }
        else { // Editor window
            TerminalEditor terminal_editor(win, sidebar, content, vector<string>());

            while (true) {
                int input = getch();

                if (input == 17 || input == KEY_F(1)) { // Ctrl+Q or F1 to exit
                    terminal_editor.cleanup();
                    exit(0);
                    break;
                }
                else {
                    terminal_editor.handleInput(input);
                }
            }
        }
    }

    // Clean up
    delwin(sidebar);
    delwin(content);
    delwin(win);
    return 0;
}

