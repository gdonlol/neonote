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
