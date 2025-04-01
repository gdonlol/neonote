#include "Application.h"
#include <ncurses.h>

/**
 * @brief Default constructor for Application
 *
 * Initializes member bariables with default values and null pointers.
 * Actual windows created in initalize().
 */
Application::Application() 
    : main_menu_(nullptr),
      terminal_editor_(nullptr, nullptr, nullptr, {}) {
}

/**
 * @brief Destructor for Application
 *
 * Ensures cleanup for ncurses resources and windows.
 */
Application::~Application() {
    cleanup();
}

/**
 * @brief Initizes the application in ncurses
 * @return If initialzation succeeded
 *
 * Sets up ncurses enviroment and creates windows.
 * Needs to be called before running main app loop.
 */
bool Application::initialize() {
    if (!ncurses_setup_.initialize()) {
        return false;
    }

    create_windows();
    return true;
}

/**
 * @brief Deletes Window object
 * @param win Window pointer to deleted window
 *
 * Uses delwin(win) to deallocate the window.
 */
void Application::WindowDeleter::operator()(WINDOW* win) const {
    if (win) {
        delwin(win);
    }
}


/**
 * @brief Creates and initializes parent container for all UI elements
 *
 * Creates the main window container and divides it into sidebar and content area.
 * Initializes the main menue
 */
void Application::create_windows() {
    main_window_ = std::unique_ptr<WINDOW, WindowDeleter>(
        newwin(LINES, COLS, 0, 0), WindowDeleter());

    auto [sidebar_width, content_width] = calculate_layout(COLS);

    // Create sidebar and content windows in container
    sidebar_ = std::unique_ptr<WINDOW, WindowDeleter>(
        derwin(main_window_.get(), LINES, sidebar_width, 0, 0), WindowDeleter());
    content_ = std::unique_ptr<WINDOW, WindowDeleter>(
        derwin(main_window_.get(), LINES, content_width, 0, sidebar_width), WindowDeleter());
    
    // Initialize main menu in the window
    setup_window_layout(LINES, COLS);
    main_menu_ = MainMenu(main_window_.get());
    refresh();
}

/**
 * @brief Calculates window layout dimentions
 * @param total_cols Widths of terminal
 * @return Pair containing sidebar width and content width
 *
 * Uses a ratio (25% by default) of the width of the terminal for the sidebar.
 * Remaining space is for the content.
 */
std::pair<int, int> Application::calculate_layout(int total_cols) const {
    const int sidebar = static_cast<int>(total_cols * SIDEBAR_WIDTH_RATIO);
    return {sidebar, total_cols - sidebar};
}

/**
 * @brief Sets up or updates the window layout
 * @param lines Current number of rows
 * @param cols Current number of columns
 *
 * Handles resizing and repositioning of all windows when terminal dimentiosn change.
 * Redraws borders and refreshes all windows to maintain consistant appearance.
 */
void Application::setup_window_layout(int lines, int cols) {
    // Calculate new dimentions
    auto [sidebar_width, content_width] = calculate_layout(cols);
    
    // Resize main window first
    wresize(main_window_.get(), lines, cols); 
    werase(main_window_.get());
    wrefresh(main_window_.get());
    
    // Resize and reposition subwindows
    wresize(sidebar_.get(), lines, sidebar_width);
    wresize(content_.get(), lines, content_width);
    mvwin(sidebar_.get(), 0, 0);
    mvwin(content_.get(), 0, sidebar_width);
    
    // Redraw borders
    box(sidebar_.get(), 0, 0);
    box(content_.get(), 0, 0);
    
    // Refresh all windows
    wrefresh(sidebar_.get());
    wrefresh(content_.get());
    wrefresh(main_window_.get());
}

/**
 * @brief Runs the main app loop
 * @return Application exit code (0 for sucess, 1 for initialization failure)
 *
 * Initializes the application and enters the main app loop
 * Returns when the user exits NeoNote
 */
int Application::run() {
    if (!initialize()) {
        return 1;
    }

    main_loop();
    return 0;
}

/**
 * @brief Main app loop
 *
 * Continuously processes user input and window transitions until exit.
 * Manages state transitions between the main menu and editor UIs.
 */
void Application::main_loop() {
    while (running_) {
        handle_resize();
        
        if (current_window_ == WindowState::MainMenu) {
            wclear(main_window_.get());
            refresh();
	    handle_main_menu();
        } else {
            handle_editor();
        }
    }
}

/**
 * @brief Handles terminal resize events
 *
 * Checks for terminal dimension changes and updates the window accordingly.
 * Maintains UI consistency across resizes.
 */

void Application::handle_resize() {
    // Retrieve terminal size
    int current_lines, current_cols;
    getmaxyx(stdscr, current_lines, current_cols);

    // Redraw if size changed
    if (current_cols != previous_dimensions_.cols || current_lines != previous_dimensions_.lines) {
        
        // Blanks entire terminal and refreshes
        werase(main_window_.get());
        refresh();

        setup_window_layout(current_lines, current_cols);
        previous_dimensions_ = {current_lines, current_cols};

        if (current_window_ == WindowState::MainMenu) {
            main_menu_.display();
        }
	else if (current_window_ == WindowState::Editor) {
            const int sidebar_width = static_cast<int>(current_cols * SIDEBAR_WIDTH_RATIO);        
	    terminal_editor_.redraw(sidebar_width);
        }
    }
}
/**
 * @brief Handles main menu interaction
 *
 * Displays the main menu and processes user input.
 * Manages transitions between menu state to editor state/exit
 */
void Application::handle_main_menu() {
    main_menu_.display();
    const int input = getch();
    main_menu_.handleInput(input);

    if (main_menu_.shouldExit()) {
        running_ = false;
    } else if (main_menu_.getCurrentWindow() != 0) { ///< "new note"
	// Creates new editor instance
        current_window_ = WindowState::Editor;
        terminal_editor_ = TerminalEditor(main_window_.get(), 
                                        sidebar_.get(), 
                                        content_.get(), 
                                        {});
    }
}

/**
 * @brief Handles editor interaction
 *
 * Process user input in the editor mode. Sends signals to terminal editor
 * and transitions back to main menu when exited
 */
void Application::handle_editor() {
    while (current_window_ == WindowState::Editor) {
        handle_resize();
     
        const int input = getch();
        if (input == MENU_SHORTCUT) {
            terminal_editor_.cleanup();
            main_menu_.returnToMenu();
	    current_window_ = WindowState::MainMenu;
            break;
        }
        
        terminal_editor_.handleInput(input);
    }
}

/**
 * @brief Cleans up application resources
 *
 * Properly deallocates windows and resets terminal state before exit.
 * Ensures clean termination of ncurses.
 */
void Application::cleanup() {
    if (sidebar_) {
        wclear(sidebar_.get());
    }
    if (content_) {
        wclear(content_.get());
    }
    if (main_window_) {
        wclear(main_window_.get());
    }
    refresh();
}
