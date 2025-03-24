#include "SidebarUI.h"

SidebarUI::SidebarUI() : selectedFile(0) {}

void SidebarUI::render(WINDOW* sidebar, const std::vector<std::string>& files, int focused_div) {
    werase(sidebar);
    box(sidebar, 0, 0);
    
    mvwprintw(sidebar, 2, 2, "My Tasks");
    mvwprintw(sidebar, 3, 2, "Calendar");
    
    for (int i = 0; i < files.size(); i++) {
        if (focused_div == 1 && i == selectedFile) {
            wattron(sidebar, A_REVERSE);
        }
        mvwprintw(sidebar, 7 + i, 2, files[i].c_str());
        if (focused_div == 1 && i == selectedFile) {
            wattroff(sidebar, A_REVERSE);
        }
    }
    
    wrefresh(sidebar);
}

int SidebarUI::handleInput(int ch, const std::vector<std::string>& files) {
    switch (ch) {
        case KEY_UP:
            if (selectedFile > 0) selectedFile--;
            break;
        case KEY_DOWN:
            if (selectedFile < files.size() - 1) selectedFile++;
            break;
        case '\n':
            return 0; // Switch focus to content
        case KEY_LEFT:
            return 0; // Switch focus to content
        case KEY_RIGHT:
            return 1; // Keep focus on sidebar
    }
    return -1; // No focus change
}
