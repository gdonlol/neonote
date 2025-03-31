#ifndef NCURSES_SETUP_H
#define NCURSES_SETUP_H

#include <ncurses.h>

class NcursesSetup {
public:
    NcursesSetup();
    ~NcursesSetup();
    
    bool initialize();
    void cleanup();
};

#endif
