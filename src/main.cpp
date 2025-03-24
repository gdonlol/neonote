#include <ncurses.h>
#include <string>
#include <signal.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "./TerminalEditor.h"

WINDOW *win;

int curr_window = 0; // current active window, 0=menu 1=text editor

// for main menu:
int menu_option = 0;

// for editor screen:
int focused_div = 0;

// placeholders for now:
vector<string> files = {"Untitled1", "Untitled2"};

int sidebar_width;
int content_width;
WINDOW *sidebar;
WINDOW *content;

TerminalEditor terminal_editor(win, sidebar, content); // helper functions in here

void draw_screen()
{
  // render menu
  if (curr_window == 1)
  {
    curs_set(1);
    terminal_editor.RenderUI(sidebar_width, files);
  }
  else if (curr_window == 0)
  {
    // draw menu
    curs_set(0);
    // box(win, 0, 0); //this for border if we want
    mvwprintw(win, (LINES - 11) / 2, (COLS - 8) / 2, "neonote.");
    int rowPrint = (LINES - 2) / 2;
    if (menu_option == 0)
    {
      wattron(win, COLOR_PAIR(1));
      mvwprintw(win, rowPrint, (COLS - 8) / 2, "my notes");
      wattroff(win, COLOR_PAIR(1));
      mvwprintw(win, rowPrint + 1, (COLS - 4) / 2, "exit");
    }
    else
    {
      mvwprintw(win, rowPrint, (COLS - 8) / 2, "my notes");
      wattron(win, COLOR_PAIR(1));
      mvwprintw(win, rowPrint + 1, (COLS - 4) / 2, "exit");
      wattroff(win, COLOR_PAIR(1));
    }

    refresh();
  }
  wrefresh(win);
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

  // create window at 0 0 with max height and width
  win = newwin(LINES, COLS, 0, 0);
  int sidebar_width = COLS * 0.25;
  int content_width = COLS - sidebar_width;
  WINDOW *sidebar = derwin(win, LINES, sidebar_width, 0, 0);
  WINDOW *content = derwin(win, LINES, content_width, 0, sidebar_width);

  TerminalEditor terminal_editor(win, sidebar, content); // helper functions in here

  refresh();

  // creating app directory
  const char *home = getenv("HOME");
  if (home == nullptr)
  {
    cerr << "Failed to get HOME directory" << endl;
    return false;
  }
  string path = string(home) + "/.local/share/neonote";
  struct stat info;
  // create the directory
  if (!(stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode)) && mkdir(path.c_str(), 0775) != 0)
  {
    printw("mkdir failed");
    getch();
    return -1;
  }

  // ----------------------
  // main menu
  // ----------------------
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
      // setup
      curs_set(1);
      terminal_editor.RenderUI(sidebar_width, files);
      terminal_editor.loadFile(files[0]); // wait is this supposed to be in the loop

      while (1)
      {
        input = getch();
        terminal_editor.handleInput(input);
        terminal_editor.displayContent();
      }
    }
  }
  endwin();

  return 0;
}
