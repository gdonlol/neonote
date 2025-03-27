#ifndef EDITOR_UI_H
#define EDITOR_UI_H

#include <ncurses.h>
#include <vector>
#include <string>

class EditorUI {
public:
    EditorUI(WINDOW *win, WINDOW *sidebar, WINDOW *content);
    
    void renderUI(int sidebar_width, const std::vector<std::string> &files);
    void displayContent(const std::vector<std::string> &lines, 
                       int row, int col, 
                       int scroll_row, int scroll_col);
    void renderSidebar(int sidebar_width, const std::vector<std::string> &files, int sidebar_index);
    void cleanup();
        
private:
    WINDOW *win;
    WINDOW *sidebar;
    WINDOW *content;
    
    void renderContent(const std::vector<std::string> &lines, 
                      int row, int col, 
                      int scroll_row, int scroll_col);
};

#endif
