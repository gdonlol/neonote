#ifndef CONTENTUI_H
#define CONTENTUI_H

#include <ncurses.h>
#include "DocumentModel.h"
#include "Viewport.h"

class ContentUI {
public:
    ContentUI();
    
    void render(WINDOW* content, const DocumentModel& document, Viewport& viewport);
    void handleInput(int ch, DocumentModel& document, Viewport& viewport);
    
private:
    std::string processMarkdown(const std::string& line, bool showFormatting) const;
};

#endif // CONTENTUI_H

