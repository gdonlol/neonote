#include "NcursesSetup.h"

/**
 * @brief Constructs an NcursesSetup object.
 * 
 * This constructor initializes the Ncurses setup object, but no specific actions are performed here. 
 * The object will be fully initialized by the `initialize()` method.
 */
NcursesSetup::NcursesSetup() {}

/**
 * @brief Destroys the NcursesSetup object and cleans up resources.
 * 
 * This destructor calls the `cleanup()` method to properly end the Ncurses session and release any resources
 * allocated during the setup.
 */
NcursesSetup::~NcursesSetup() {
    cleanup();
}

/**
 * @brief Initializes the Ncurses environment for terminal-based UI.
 * 
 * This method sets up the Ncurses library for terminal-based user interface functionality. It initializes
 * color support, sets terminal modes (like noecho, raw input, etc.), and checks for color support in the terminal.
 * If color support is unavailable, the function will display a message and return `false`.
 * 
 * @return `true` if the Ncurses setup is successful, otherwise `false`.
 */
bool NcursesSetup::initialize() {
    // ncurses setup
    initscr();
    cbreak();
    noecho();
    raw();
    use_default_colors();
    keypad(stdscr, TRUE);
    intrflush(stdscr, FALSE);

    start_color();
    init_color(COLOR_BLACK, 150, 150, 150); // Grey
    init_color(COLOR_RED, 555, 110, 1000); // Purple 1
    init_color(COLOR_GREEN, 580, 200, 965); // Purple 2
    init_color(COLOR_YELLOW, 615, 285, 925); // Purple 3
    init_color(COLOR_BLUE, 635, 375, 880); // Purple 4
    init_color(COLOR_MAGENTA, 670, 470, 845); // Purple 5
    init_color(COLOR_CYAN, 700, 565, 800); // Purple 6

    init_pair(1, COLOR_BLACK, COLOR_WHITE); // Inverted highlight
    init_pair(2, COLOR_RED, -1);    // H1
    init_pair(3, COLOR_GREEN, -1);  // H2
    init_pair(4, COLOR_YELLOW, -1); // H3
    init_pair(5, COLOR_BLUE, -1);   // H4
    init_pair(6, COLOR_MAGENTA, -1);// H5
    init_pair(7, COLOR_CYAN, -1);   // H6
    init_pair(9, COLOR_WHITE, COLOR_BLACK); // Codeblock

    if (!has_colors()) {
        printw("Terminal does not support color");
        getch();
        endwin();
        return false;
    }
    
    return true;
}

/**
 * @brief Cleans up the Ncurses session.
 * 
 * This method ends the Ncurses session and restores the terminal to its normal state.
 */
void NcursesSetup::cleanup() {
    endwin();
}
