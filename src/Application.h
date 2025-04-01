/**
 * @file Application.h
 * @brief Main application class managing terminal editor and main menu interfaces for NeoNote.
 * 
 * @author Runcong Zhu
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <ncurses.h>
#include <memory>
#include <vector>
#include "TerminalEditor.h"
#include "MainMenu.h"
#include "NcursesSetup.h"
#include "Settings.h"

/**
 * @class Application
 * @brief Main application class managing terminal editor and main menu interfaces
 * @Author Runcong Zhu
 *
 * Handles initialization, window management, and the main event loop for NeoNote.
 * Manages transitions between main menu and editor and handles resizes.
 */
class Application {
public:
    Application();
    ~Application();
    int run();

private:
    struct WindowDeleter {
        void operator()(WINDOW* win) const;
    };

    enum class WindowState {
        MainMenu,
        Editor
    };

    struct Dimensions {
        int lines{0};
        int cols{0};
    };

    bool initialize();
    void create_windows();
    void main_loop();
    void handle_resize();
    void handle_main_menu();
    void handle_editor();
    void cleanup();

    std::pair<int, int> calculate_layout(int total_cols) const;
    void setup_window_layout(int lines, int cols);
    int calculate_sidebar_width(int current_cols = COLS) const;

    static constexpr double SIDEBAR_WIDTH_RATIO = 0.25; 

    NcursesSetup ncurses_setup_;
    MainMenu main_menu_;
    TerminalEditor terminal_editor_;
    WindowState current_window_{WindowState::MainMenu};
    bool running_{true};
    Dimensions previous_dimensions_;
    
    // Smart pointers managing windows
    std::unique_ptr<WINDOW, WindowDeleter> main_window_{nullptr};
    std::unique_ptr<WINDOW, WindowDeleter> sidebar_{nullptr};
    std::unique_ptr<WINDOW, WindowDeleter> content_{nullptr};
};

#endif // APPLICATION_H
