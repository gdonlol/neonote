#ifndef SIDEBARUI_H
#define SIDEBARUI_H

#include <ncurses.h>
#include <vector>
#include <string>

class SidebarUI {
public:
    SidebarUI();
    
    void render(WINDOW* sidebar, const std::vector<std::string>& files, int focused_div);
    int handleInput(int ch, const std::vector<std::string>& files);
    
    int getSelectedFile() const { return selectedFile; }
    
private:
    int selectedFile;
};

#endif // SIDEBARUI_H

