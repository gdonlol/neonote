#ifndef EDITOR_INPUTS_H
#define EDITOR_INPUTS_H

#include "TerminalEditor.h"

class EditorInputs {
public:
    EditorInputs(TerminalEditor& editor);
    void handleInput(int input);

private:
    TerminalEditor& editor;
};

#endif // EDITOR_INPUTS_H
