#include <ncurses.h>
#include <string>
#include <signal.h>

WINDOW *win;

// Function to handle window resize
void handle_resize(int sig)
{
  if (sig == SIGWINCH)
  {
    endwin();  // End ncurses mode
    refresh(); // Refresh the terminal

    int height = LINES;
    int width = COLS;

    delwin(win);                       // clean up the old window
    win = newwin(height, width, 0, 0); // create new full-screen window
    box(win, 0, 0);                    // redraw border

    // redraw content
    mvwprintw(win, height / 2, (width - 8) / 2, "neonote.");
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

  signal(SIGWINCH, handle_resize);

  int height, width;
  getmaxyx(stdscr, height, width);

  // create window at 0 0 with max height and width
  win = newwin(height, width, 0, 0);
  refresh();

  box(win, 0, 0);
  mvwprintw(win, height / 2, (width - 8) / 2, "neonote.");
  // note: subtract length of string from width to have centered

  wrefresh(win);
  getch();

  endwin();

  return 0;
}
