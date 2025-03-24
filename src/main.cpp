#include <ncurses.h>
#include <signal.h>
#include "TerminalController.h"

int main() {
    TerminalController controller;
    controller.run();
    return 0;
}
