/**
 * @file NcursesSetup.h
 * @brief Header file for the NcursesSetup class, which manages the initialization and cleanup of the ncurses library.
 * 
 * This file defines the NcursesSetup class, responsible for setting up and properly cleaning up 
 * the ncurses environment used for the terminal-based calendar application.
 * 
 * @author 
 * - Gordon Xu
 * - Runcong Zhu
 */

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
