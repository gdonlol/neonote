## How to run

Clone the repository.

```
git clone https://github.com/gdonlol/neonote.git
```

Navigate to project directory in your terminal and use `make` to compile the program or `make run` to compile and run.

-   The compiled binary will be located in the `bin/` folder.
-   If the `bin/` directory doesn’t exist, `make` will automatically create it.

## Dependencies

Neonote relies on the following dependencies for proper compilation and execution.

-   `g++` (GCC)
-   **ncurses Library**
    -   Used for terminal-based UI rendering.
    -   Install commands for common Linux distributions:
        -   **Debian/Ubuntu**:
        ```
        sudo apt install libncurses5-dev libncursesw5-dev
        ```
        -   **Arch Linux**:
        ```
        sudo pacman -S ncurses
        ```
        -   **Fedora/RHEL**:
        ```
        sudo dnf install ncurses-devel
        ```
-   **GNU Make**
    -   Used to build the project using the provided `Makefile`.
    -   Install commands:
        -   **Debian/Ubuntu**:
        ```
        sudo apt install make
        ```
        -   **Arch Linux**:
        ```
        sudo pacman -S make
        ```
        -   **Fedora/RHEL**:
        ```
        sudo dnf install make
        ```

## Description

CS3307 Group 14

Beautiful, fast, and fully featured text editor in the terminal for users who wish to keep their workflow in one place. This project will incorporate planning features similar to Notion to streamline personal productivity but be optimized for terminal use with a graphical TUI (text-based user interface). The text editor will integrate tools for task management, calendar scheduling, and more, while being optimized for a keyboard-driven workflow in a terminal environment.

This software will be developed in C++ and will run virtually on a Raspberry Pi. We do not require a physical Raspberry Pi kit for this project.

## Features

-   Markdown editing
-   Keyboard user-centric, efficient keybinds for everything
-   Kanban board todo list for productivity
-   Calendars for event planning
-   Interface for list of pages on sidebar
