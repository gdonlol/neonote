#include <ncurses.h>
#include <string>
#include <signal.h>
#include <vector>
#include "TerminalEditor.h"

using namespace std;

WINDOW *win;
int curr_window = 0; // current active window, 0=menu 1=text editor
int menu_option = 0; // for main menu selection

void draw_screen() {
    if (curr_window == 0) {
        // draw menu
        curs_set(0);
        mvwprintw(win, (LINES - 11) / 2, (COLS - 8) / 2, "neonote.");
        int rowPrint = (LINES - 2) / 2;
        
        if (menu_option == 0) {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, rowPrint, (COLS - 8) / 2, "my notes");
            wattroff(win, COLOR_PAIR(1));
            mvwprintw(win, rowPrint + 1, (COLS - 4) / 2, "exit");
        } else {
            mvwprintw(win, rowPrint, (COLS - 8) / 2, "my notes");
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, rowPrint + 1, (COLS - 4) / 2, "exit");
            wattroff(win, COLOR_PAIR(1));
        }
        wrefresh(win);
    }
}

int main() {
    // ncurses setup
    initscr();
    cbreak();
    noecho();
    raw();
    use_default_colors();
    keypad(stdscr, TRUE);
    intrflush(stdscr, FALSE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    if (!has_colors()) {
        printw("Terminal does not support color");
        getch();
        endwin();
        return 0;
    }

    // create windows
    win = newwin(LINES, COLS, 0, 0);
    int sidebar_width = COLS * 0.25;
    int content_width = COLS - sidebar_width;
    WINDOW *sidebar = derwin(win, LINES, sidebar_width, 0, 0);
    WINDOW *content = derwin(win, LINES, content_width, 0, sidebar_width);
    refresh();

    // ----------------------
    // main menu loop
    // ----------------------
    while (true) {
        int input;

        if (curr_window == 0) {
            while (true) {
                draw_screen();
                input = getch();
                
                if (input == KEY_UP || input == 'k') {
                    menu_option = (menu_option - 1) % 2;
                    if (menu_option < 0) menu_option = 1;
                } 
                else if (input == KEY_DOWN || input == 'j') {
                    menu_option = (menu_option + 1) % 2;
                } 
                else if (input == KEY_ENTER || input == '\n') {
                    if (menu_option == 0) {
                        curr_window = 1;
                        break;
                    } else {
                        endwin();
                        return 0;
                    }
                }
            }
        }

        if (curr_window == 1) {
             TerminalEditor terminal_editor(win, sidebar, content, vector<string>());
            
             while (true) {
                input = getch();
                
                 if (input == 17 || input == KEY_F(1)) {
                    curr_window = 0;
                    break;
                }
                
                terminal_editor.handleInput(input);
            }
        }
    }

    endwin();
    return 0;
}
