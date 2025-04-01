// settings.h
#ifndef SETTINGS_H
#define SETTINGS_H

#include <ncurses.h>

// Sizing
static constexpr double SIDEBAR_WIDTH_RATIO = 0.25;

// Keybinds
static constexpr int MENU_SHORTCUT = 17;

// Basic Movement    
constexpr int CURSOR_UP = KEY_UP;
constexpr int CURSOR_DOWN = KEY_DOWN;
constexpr int CURSOR_LEFT = KEY_LEFT;
constexpr int CURSOR_RIGHT = KEY_RIGHT;

// Navigation
constexpr int SKIP_LEFT = 29;    // Ctrl+]
constexpr int SKIP_RIGHT = 28;   // Ctrl+ "\"
constexpr int GOTO_FILE_START = KEY_PPAGE;   // Ctrl+F
constexpr int GOTO_FILE_END = KEY_NPAGE;    // Ctrl+T
    
// Editing
constexpr int DELETE_LEFT = KEY_BACKSPACE;     // Backspace
constexpr int DELETE_RIGHT = KEY_DC; // Delete
constexpr int DELETE_WORD = 8;       // Ctrl+Backspace
constexpr int NEW_LINE = '\n';       // Enter
constexpr int INDENT = KEY_BTAB;    // Tab
constexpr int INDENT_ALT = KEY_CTAB; // Ctrl+Tab

// Formatting
constexpr int ITALIC = 9;            // Ctrl+I
constexpr int BOLD = 2;              // Ctrl+B
    
// File Operations
constexpr int NEW_FILE = 14;         // Ctrl+N
constexpr int SAVE_FILE = 19;        // Ctrl+S
constexpr int RENAME_FILE = 18;      // Ctrl+R
constexpr int DELETE_FILE = KEY_DC;  // Delete (in sidebar)

// UI Navigation
constexpr int SWITCH_PANEL = 15;     // Ctrl+O
constexpr int SWITCH_PANEL_ALT = 4;  // Ctrl+D
constexpr int TOGGLE_SIDEBAR = KEY_F(1);
    
// Kanban Specific
constexpr int MOVE_TASK_LEFT = KEY_LEFT;
constexpr int MOVE_TASK_RIGHT = KEY_RIGHT;

// Basic
constexpr int CONFIRM_OPTION = '\n';

// Tab Size
constexpr const char* INDENTATION = "    ";

// Colors

// Codeblock Grey
constexpr short CODEBLOCK_R = 150;
constexpr short CODEBLOCK_G = 150;
constexpr short CODEBLOCK_B = 150;

// Color Gradient
constexpr short COLOR1_R = 555;
constexpr short COLOR1_G = 110;
constexpr short COLOR1_B = 1000;

constexpr short COLOR2_R = 580;
constexpr short COLOR2_G = 200;
constexpr short COLOR2_B = 965;

constexpr short COLOR3_R = 615;
constexpr short COLOR3_G = 285;
constexpr short COLOR3_B = 925;

constexpr short COLOR4_R = 635;
constexpr short COLOR4_G = 375;
constexpr short COLOR4_B = 880;

constexpr short COLOR5_R = 670;
constexpr short COLOR5_G = 470;
constexpr short COLOR5_B = 845;

constexpr short COLOR6_R = 700;
constexpr short COLOR6_G = 565;
constexpr short COLOR6_B = 800;




#endif // SETTINGS_H
