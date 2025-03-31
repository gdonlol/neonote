#include "Application.h"
#include <ncurses.h>

Application::Application() : main_menu_(nullptr) {
    // MainMenu initialized after main_window_ is created
}

Application::~Application() {
    cleanup();
}

bool Application::initialize() {
    if (!ncurses_setup_.initialize()) {
        return false;
    }

    create_windows();
    return true;
}

/**
 * @ brief Ensures that the `WINDOW` object is properly cleaned up when the `std::unique_ptr` goes out of scope or reset
 */
void Application::WindowDeleter::operator()(WINDOW* win) const {
    if (win) {
        delwin(win);
    }
}


/**
 * @brief Creates and initialises parent container for all UI elements
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
 * @brief Calculates sidebar width relative to container size
 * @return A pair of the sidebar size and the window size
 */
std::pair<int, int> Application::calculate_layout(int total_cols) const {
    static constexpr double SIDEBAR_WIDTH_RATIO = 0.25;
    const int sidebar = static_cast<int>(total_cols * SIDEBAR_WIDTH_RATIO);
    return {sidebar, total_cols - sidebar};
}

/**
 * @brief 
 */
void Application::setup_window_layout(int lines, int cols) {
    // Define variables
    auto [sidebar_width, content_width] = calculate_layout(cols);
    
    // Resize all windows
    wresize(main_window_.get(), lines, cols);
    wresize(sidebar_.get(), lines, sidebar_width);
    wresize(content_.get(), lines, content_width);

    // Reposition sidebar and content
    mvwin(sidebar_.get(), 0, 0);
    mvwin(content_.get(), 0, sidebar_width);
}

/**
 * @brief Runs the main loop
 */
int Application::run() {
    if (!initialize()) {
        return 1;
    }

    main_loop();
    return 0;
}

/**
 * @brief Main logic loop of program
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

void Application::handle_resize() {
    // Retrieve terminal size
    int current_lines, current_cols;
    getmaxyx(stdscr, current_lines, current_cols);

    // Redraw if size changed
    if (current_cols != previous_dimensions_.cols || current_lines != previous_dimensions_.lines) {
        
        // Blanks entire terminal and refreshes
        wclear(main_window_.get());
        refresh();

        setup_window_layout(current_lines, current_cols);
        previous_dimensions_ = {current_lines, current_cols};

        if (current_window_ == WindowState::MainMenu) {
            main_menu_.display();
        }
	else if (current_window_ == WindowState::Editor) {
            TerminalEditor editor(main_window_.get(), sidebar_.get(), content_.get(), {});
            wrefresh(main_window_.get());
	    wrefresh(sidebar_.get());
	    wrefresh(content_.get());
	}
    }
}


void Application::handle_main_menu() {
    main_menu_.display();
    const int input = getch();
    main_menu_.handleInput(input);

    if (main_menu_.shouldExit()) {
        running_ = false;
    } else if (main_menu_.getCurrentWindow() != 0) {
        current_window_ = WindowState::Editor;
    }
}


void Application::handle_editor() {
    TerminalEditor editor(main_window_.get(), sidebar_.get(), content_.get(), {});
    
    while (current_window_ == WindowState::Editor) {
        handle_resize();
        
        const int input = getch();
        if (input == 17 || input == KEY_F(1)) { // Ctrl+Q or F1
            main_menu_.returnToMenu();
	    current_window_ = WindowState::MainMenu;
            break;
        }
        
        editor.handleInput(input);
    }
}


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
