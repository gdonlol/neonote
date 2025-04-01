/**
 * @file EditorUI.h
 * @brief Defines the EditorUI class for managing and rendering a text editor user interface using ncurses.
 * 
 * @author Gordon Xu
 * @author Runcong Zhu
 */

#ifndef EDITOR_UI_H
#define EDITOR_UI_H

#include <ncurses.h>
#include <vector>
#include <string>

/**
 * @class EditorUI
 * @brief Handles the rendering and interaction of a text editor's user interface.
 * 
 * The EditorUI class is responsible for managing the main window, sidebar, and content area.
 * It provides functions for rendering the UI, displaying content, managing scrolling, 
 * and handling user prompts.
 * @Author Gordon Xu
 * @Author Runcong Zhu
 */
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

    std::string formatWithEllipsis(const std::string& text, int maxWidth);
};

#endif
