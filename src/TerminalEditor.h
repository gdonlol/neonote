/**
 * @file TerminalEditor.h
 * @brief Header file for the TerminalEditor class, which serves as the main controller for the terminal-based editor.
 * 
 * This file defines the TerminalEditor class responsible for handling user input, rendering the UI,
 * managing files, and interacting with the calendar and task manager.
 * 
 * @author Gordon Xu
 * @author Runcong Zhu
 * @author Emily Atyeo
 */

#ifndef TERMINAL_EDITOR_H
#define TERMINAL_EDITOR_H

#include <ncurses.h>
#include <vector>
#include <string>
#include "FileManager.h"
#include "EditorUI.h"
#include "Calendar.h"
#include "TaskManager.h"

class TerminalEditor {
public:
    TerminalEditor(WINDOW *win, WINDOW *sidebar, WINDOW *content,
                 const std::vector<std::string> &files);
    
    void handleInput(int ch);
    void redraw(int sidebar_width);
    void cleanup();
    
private:
    FileManager fileManager;
    EditorUI ui;
    Calendar calendar;
    TaskManager taskManager; 
    
    int row;
    int col;
    int scroll_row;
    int scroll_col;
    int focused_div;
    int last_focused_div;
    int sidebar_index;
    int sidebar_width;
    std::vector<std::string> lines;
    std::string current_file;

    void handleInputContent(int ch);
    void handleInputSidebar(int ch);
    void handleInputKanban(int ch);
    void handleInputCalendar(int ch);
    void adjustCursorPosition();
};

#endif
