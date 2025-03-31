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
                       int row, int col, int scroll_row,
                       int scroll_col, std::string title);
    void renderSidebar(int sidebar_width, const std::vector<std::string> &files, int sidebar_index);
    void cleanup();
    std::string displayPrompt(std::string title);
    
    WINDOW* getMainWindow() const { return win; }
    WINDOW* getSidebar() const { return sidebar; }
    WINDOW* getContent() const { return content; }
        
private:
    WINDOW *win;
    WINDOW *sidebar;
    WINDOW *content;

    int sidebarScrollOffset;
    
    void renderContent(const std::vector<std::string> &lines, 
                      int row, int col, 
                      int scroll_row, int scroll_col);
};

#endif
