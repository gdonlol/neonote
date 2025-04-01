/**
 * @file TextPrompt.h
 * @brief Header file for the TextPrompt class, which handles text input prompts using ncurses.
 * 
 * This file defines the TextPrompt class responsible for displaying a text prompt 
 * and capturing user input in a terminal environment.
 * 
 * @author 
 * - Gordon Xu
 */

#ifndef TEXT_PROMPT_H
#define TEXT_PROMPT_H

#include <string>
#include <ncurses.h>

class TextPrompt {
public:
    TextPrompt(WINDOW *win, std::string title_in);
    std::string prompt();
    
private:
    std::string title;
    WINDOW *win;
};

#endif
