## Keybindings Guide
This guide provides an overview of the keybindings available in the terminal editor, covering both content editing and sidebar navigation.

# Content Editing Mode
- Arrow Keys (↑, ↓, ←, →) - Move the cursor up, down, left, or right.
- Backspace (⌫) - Deletes the character to the left of the cursor.
- Enter (⏎) - Inserts a new line at the current cursor position.
- `Ctrl + S` - Saves the current file.
- `Ctrl + Q` - Saves and exits the editor.
- `Ctrl + B` - Inserts ** (bold formatting marker).
- `Ctrl + I` - Inserts * (italic formatting marker).
- `Ctrl + O` / `Ctrl + D` - Switches focus to the sidebar.
- `Ctrl + R` - Renders the entire user interface (calls renderUI()).
- `Ctrl + E` - Renders the content (calls renderContent()).
- `Ctrl + W` - Displays content in the content window (calls displayContent()).
- `Ctrl + X` - Cleans up and exits the ncurses session (calls cleanup()).
- Printable Characters (`A-Z`, `0-9`, `Symbols`) - Inserts the typed character at the cursor position.

# Sidebar Navigation Mode
- Arrow Up (↑) - Moves up the file list.
- Arrow Down (↓) - Moves down the file list.
- Enter (⏎) - Loads the selected file into the editor.
- `Ctrl + O` / `Ctrl + D` - Switches focus back to content editing.

# Main Menu Navigation
- Arrow Up (↑) or `k` - Moves up the menu options.
- Arrow Down (↓) or `j` - Moves down the menu options.
- Enter (⏎) or `\n` - Selects the currently highlighted menu option (either "my notes" or "exit").

# Cursor and Scroll Adjustments
- Cursor movement is restricted within the text bounds.
- Scrolling occurs automatically when the cursor moves beyond the visible area.
