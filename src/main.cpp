#include <ncurses.h>
#include <string>
#include <signal.h>
#include <vector>
using namespace std;

#include "./render/RenderEditor.h"
#include "./render/RenderMenu.h"
#include "./logic/TerminalEditor.h"

WINDOW *win;

int curr_window = 0; // current active window, 0=menu 1=text editor

// for main menu:
int menu_option = 0;

// for editor screen:
int focused_div = 0;

// placeholders for now:
vector<string> files = {"Untitled1", "Untitled2"};

void draw_screen()
{
  // render menu
  if (curr_window == 1)
  {
    curs_set(1);
    renderEditor(win, files);
  }
  else if (curr_window == 0)
  {
    curs_set(0);
    renderMenu(win, menu_option);
  }

  wrefresh(win);
}

// function to handle window resize
void handle_resize(int sig)
{
  if (sig == SIGWINCH)
  {
    endwin();  // end ncurses mode
    refresh(); // refresh the terminal

    delwin(win);                     // clean up the old window
    win = newwin(LINES, COLS, 0, 0); // create new full-screen window

    // redraw content
    draw_screen();
    wrefresh(win);
  }
}

int main()
{
  // ncurses setup
  initscr();            // initialize screen
  cbreak();             // allows ctrl c terminate
  noecho();             // stop inputs from printing
  use_default_colors(); // use default terminal colors for users with custom terminals
  keypad(stdscr, TRUE); // enable arrow keys
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  // check color support
  if (!has_colors())
  {
    printw("Terminal does not support color");
    getch();
    return -1;
  }
  signal(SIGWINCH, handle_resize); // listen for terminal resize

  // create window at 0 0 with max height and width
  win = newwin(LINES, COLS, 0, 0);
  refresh();
  while (1)
  {
    int input;

    if (curr_window == 0)
    {
      while (1)
      {
        draw_screen();
        input = getch();
        if (input == KEY_UP || input == 'k')
        {
          menu_option = (menu_option - 1) % 2;
        }
        else if (input == KEY_DOWN || input == 'j')
        {
          menu_option = (menu_option + 1) % 2;
        }
        else if (input == KEY_ENTER || input == '\n')
        {
          if (menu_option == 0)
          {
            curr_window = 1;
            break;
          }
          else
          {
            endwin();
            return 0;
          }
        }
      }
    }

    if (curr_window == 1)
    {
      TerminalEditor terminal_editor;
      while (1)
      {
        draw_screen();
        input = getch();
        if (focused_div == 0)
        {
          terminal_editor.loadFile(files[0]);
          terminal_editor.handleInput(input);
        }
        else
        {
          // side bar inputs
        }
      }
    }
  }
  endwin();

  return 0;
}
