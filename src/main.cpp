#include <ncurses.h>
#include <string>

int main() {
  // initialize ncurses
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);

  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
  }

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  WINDOW *win = newwin(max_y, max_x, 0, 0);
  wbkgd(win, COLOR_PAIR(1));

  std::string message = "neonote.";
  int msg_x = (max_x - message.length()) / 2;
  int msg_y = max_y / 2;

  mvprintw(msg_y, msg_x, "%s", message.c_str());

  wrefresh(win);

  getch();

  delwin(win);
  endwin();

  return 0;
}
