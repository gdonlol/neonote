#include "TerminalController.h"
#include <filesystem>

TerminalController::TerminalController() : currentWindow(0) {
    initscr();
    cbreak();
    noecho();
    use_default_colors();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    
    win = newwin(LINES, COLS, 0, 0);
    sidebar_width = COLS * 0.25;
    content_width = COLS - sidebar_width;
    sidebar = derwin(win, LINES, sidebar_width, 0, 0);
    content = derwin(win, LINES, content_width, 0, sidebar_width);
    
    focused_div = 0;
    
    // Initialize file handler and load files
    const char* home = getenv("HOME");
    if (home) {
        std::string homePath = std::string(home) + "/.local/share/neonote";
        fileHandler.setBasePath(homePath);
        
        for (const auto& entry : std::filesystem::directory_iterator(homePath)) {
            files.push_back(entry.path().stem().string());
        }
        
        if (files.empty()) {
            std::string defaultFile = "Untitled1.md";
            fileHandler.createFile(defaultFile);
            files.push_back("Untitled1");
        }
    }
    
    // Load first file
    if (!files.empty()) {
        document.setLines(fileHandler.loadFile(files[0]));
    }
}

TerminalController::~TerminalController() {
    cleanup();
}

void TerminalController::run() {
    while (true) {
        int ch = getch();
        
        if (currentWindow == 0) {
            int selection = menuUI.handleInput(ch);
            if (selection == 0) {
                currentWindow = 1;
                curs_set(1);
                continue;
            } else if (selection == 1) {
                break; // Exit
            }
            menuUI.render(win);
        } 
        else {
            if (ch == KEY_F(1)) { // Example: F1 to return to menu
                currentWindow = 0;
                curs_set(0);
                menuUI.render(win);
                continue;
            }
            
            if (focused_div == 0) {
                contentUI.handleInput(ch, document, viewport);
                contentUI.render(content, document, viewport);
            } else {
                int new_focus = sidebarUI.handleInput(ch, files);
                if (new_focus != -1) {
                    focused_div = new_focus;
                    if (new_focus == 0) {
                        document.setLines(fileHandler.loadFile(files[sidebarUI.getSelectedFile()]));
                    }
                }
                sidebarUI.render(sidebar, files, focused_div);
            }
        }
        
        wrefresh(win);
    }
}

void TerminalController::handleInput(int ch) {
    if (focused_div == 0) {
        contentUI.handleInput(ch, document, viewport);
    } else {
        int new_focus = sidebarUI.handleInput(ch, files);
        if (new_focus != -1) {
            focused_div = new_focus;
            if (new_focus == 0) {
                document.setLines(fileHandler.loadFile(files[sidebarUI.getSelectedFile()]));
            }
        }
    }
}

void TerminalController::cleanup() {
    endwin();
}
