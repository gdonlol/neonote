#include "NcursesSetup.h"
#include "Settings.h"
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

    init_color(COLOR_BLACK, CODEBLOCK_R, CODEBLOCK_G, CODEBLOCK_B);
    init_color(COLOR_RED, COLOR1_R, COLOR1_G, COLOR1_B);
    init_color(COLOR_GREEN, COLOR2_R, COLOR2_G, COLOR2_B);
    init_color(COLOR_YELLOW, COLOR3_R, COLOR3_G, COLOR3_B);
    init_color(COLOR_BLUE, COLOR4_R, COLOR4_G, COLOR4_B);
    init_color(COLOR_MAGENTA, COLOR5_R, COLOR5_G, COLOR5_B);
    init_color(COLOR_CYAN, COLOR6_R, COLOR6_G, COLOR6_B);
    
    init_pair(1, COLOR_RED, -1);		///< Highlight
    init_pair(2, COLOR_RED, -1);		///< Color 1
    init_pair(3, COLOR_GREEN, -1);		///< Color 2
    init_pair(4, COLOR_YELLOW, -1);		///< Color 3
    init_pair(5, COLOR_BLUE, -1);		///< Color 4
    init_pair(6, COLOR_MAGENTA, -1);		///< Color 5
    init_pair(7, COLOR_CYAN, -1);		///< Color 6
    init_pair(9, COLOR_WHITE, COLOR_BLACK);	///< Codeblock
    
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
