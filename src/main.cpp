#include <ncurses.h>
#include <string>
#include <signal.h>

WINDOW *win;

int curr_window = 0; // current active window, 0=menu 1=text editor

// for main menu:
int menu_option = 0;

void render_menu()
{
  // box(win, 0, 0); //this for border if we want
  mvwprintw(win, (LINES - 10) / 2, (COLS - 8) / 2, "neonote.");

  if (menu_option == 0)
  {
    mvwprintw(win, (LINES - 2) / 2, (COLS - 16) / 2, ">> my notes");
    mvwprintw(win, (LINES - 1) / 2, (COLS - 16) / 2, "2. exit");
  }
  else
  {
    mvwprintw(win, (LINES - 2) / 2, (COLS - 16) / 2, "1. my notes");
    mvwprintw(win, (LINES - 1) / 2, (COLS - 16) / 2, ">> exit");
  }

  refresh();
}

void draw_screen()
{
  clear();
  // render menu
  if (curr_window == 0)
  {
    render_menu();
  }
  if (curr_window == 1)
  {
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
    // box(win, 0, 0);                    // redraw border

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
    draw_screen();
    getch();
    menu_option = (menu_option + 1) % 2;
  }
  endwin();

  return 0;
}
