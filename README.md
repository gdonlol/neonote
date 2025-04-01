# Neonote
A terminal-based, markdown text editor with task management, calendar scheduling, and more.

## How to run
Clone the repository.

```
git clone https://gitlab.sci.uwo.ca/courses/2025/01/COMPSCI3307/group14.git
```

Navigate to project directory in your terminal and use `make` to compile the program or `make run` to compile and run.

- The compiled binary will be located in the `bin/` folder.
- If the `bin/` directory doesn’t exist, `make` will automatically create it.

## Dependencies
Neonote relies on the following dependencies for proper compilation and execution.

1. `g++` (GCC)
2. **ncurses Library**
- Used for terminal-based UI rendering.
- Install commands for common Linux distributions:
- 2.1. Debian/Ubuntu:
```
sudo apt install libncurses5-dev libncursesw5-dev
```
- 2.2. Arch Linux:
```
sudo pacman -S ncurses
```
- 2.3. Fedora/RHEL:
```
sudo dnf install ncurses-devel
```
3. **GNU Make**
- Used to build the project using the provided `Makefile`.
- Install commands:
- 3.1. Debian/Ubuntu:
```
sudo apt install make
```
- 3.2. Arch Linux:
```
sudo pacman -S make
```
- 3.3. Fedora/RHEL:
```
sudo dnf install make
```

## Description - CS3307 Group 14
Beautiful, fast, and fully featured text editor in the terminal for users who wish to keep their workflow in one place. This project will incorporate planning features similar to Notion to streamline personal productivity but be optimized for terminal use with a graphical TUI (text-based user interface). The text editor will integrate tools for task management, calendar scheduling, and more, while being optimized for a keyboard-driven workflow in a terminal environment.

This software will be developed in C++ and will run virtually on a Raspberry Pi. We do not require a physical Raspberry Pi kit for this project.

## Features
- Markdown editing
- Keyboard user-centric, efficient keybinds for everything
- Kanban board todo list for productivity
- Calendars for event planning
- Interface for list of pages on sidebar

## Keybinds Guide
This guide provides an overview of the keybindings available in the terminal editor, covering both content editing and sidebar navigation.

1. **Main Menu Navigation**
- Arrow Up (↑) or `k` - Moves up the menu options.
- Arrow Down (↓) or `j` - Moves down the menu options.
- Enter (⏎) or `\n` - Selects the currently highlighted menu option (either "my notes" or "exit").

2. **Content Editing Mode**
- Arrow Keys (↑, ↓, ←, →) - Move the cursor up, down, left, or right.
- Backspace (⌫) - Deletes the character to the left of the cursor.
- Enter (⏎) - Inserts a new line at the current cursor position.
- `Ctrl + S` - Saves the current file.
- `Ctrl + Q` - Saves and exits the editor.
- `Ctrl + B` - Inserts ** (bold formatting marker).
- `Ctrl + I` - Inserts * (italic formatting marker).
- `Ctrl + O` / `Ctrl + D` - Switches focus to the sidebar.
- `Ctrl + N` - Makes a new file.
- `Ctrl + ]` - Move the cursor to the start of the previous word.
- `Ctrl + \` - Skip to the next word.
- Printable Characters (`A-Z`, `0-9`, `Symbols`) - Inserts the typed character at the cursor position.

3. **Sidebar Navigation Mode**
- Arrow Up (↑) - Moves up the file list.
- Arrow Down (↓) - Moves down the file list.
- Enter (⏎) - Loads the selected file or page into the editor.
- `Ctrl + O` / `Ctrl + D` - Switches focus back to content editing.
- `Ctrl + R` - Rename a file (click enter to confirm after typing in new name).
- `Ctrl + T` - Create a new task for the kanban board (click enter to confirm after typing in new name).
- `Ctrl + P` - Move an existing task into a new category (to do, in progress or done).

4. **Cursor and Scroll Adjustments**
- Cursor movement is restricted within the text bounds.
- Scrolling occurs automatically when the cursor moves beyond the visible area.