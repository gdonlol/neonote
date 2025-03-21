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
    use_default_colors();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
  }

  printf("\033[0m"); // Reset any previous ANSI formatting
  refresh();

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  WINDOW *win = newwin(max_y, max_x, 0, 0);
  wbkgd(win, COLOR_PAIR(1));

  std::string message = "\033[1mBold Text\033[0m\n";
  int msg_x = (max_x - message.length()) / 2;
  int msg_y = max_y / 2;

  mvprintw(msg_y, msg_x, "%s", message.c_str());

  wrefresh(win);

  getch();

  delwin(win);
  endwin();

  return 0;
}
