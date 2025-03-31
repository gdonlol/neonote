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
 * @return 0 if the program exits successfully.
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

    // Main application loop
    while (true) {
        int currentWindow = mainMenu.getCurrentWindow();

        if (currentWindow == 0) { // Menu window
            mainMenu.display();
            int input = getch();
            mainMenu.handleInput(input);
            
            if (mainMenu.shouldExit()) {
                break;
            }
        }
        else if (currentWindow == 1) { // Editor window
            TerminalEditor terminal_editor(win, sidebar, content, vector<string>());
            
            while (true) {
                int input = getch();
                
                if (input == 17 || input == KEY_F(1)) { // Ctrl+Q or F1 to exit
                    terminal_editor.cleanup();
                    mainMenu.returnToMenu();
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
