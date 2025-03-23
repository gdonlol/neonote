#ifndef RENDEREDITOR_H
#define RENDEREDITOR_H

#include <ncurses.h>
#include <vector>
#include <string>
using namespace std;

void renderEditor(WINDOW *win, vector<string> &files);

#endif