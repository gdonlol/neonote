
## Description

CS3307 Group 14
Beautiful, fast, and fully featured text editor in the terminal for users who wish to keep their workflow in one place. This project will incorporate planning features similar to Notion to streamline personal productivity but be optimized for terminal use with a graphical TUI (text-based user interface). The text editor will integrate tools for task management, calendar scheduling, and more, while being optimized for a keyboard-driven workflow in a terminal environment.

This software will be developed in C++ and will run virtually on a Raspberry Pi. We do not require a physical Raspberry Pi kit for this project.

## Features

### Required Features

-   Rich text editing (bold, italic, color, without markdown syntax) – if this is too hard, we go just plain text editor
-   Custom file format
-   Keyboard user-centric, efficient keybinds for everything
-   Easy creation of tables
-   Todo lists for productivity
-   Kanban board
-   Calendars for event planning
-   Interface for list of pages on sidebar

### Optional Features

-   Widgets like clock, weather, ASCII art, PC hardware stats, animated matrix, etc.
-   Startup options in arguments, for users to automatically open a widget, for example, on startup
-   Password-locked pages
-   File system where users can make nested structures, like pages within pages
-   Linking between pages, like a wiki
-   Man page/help docs for keybinds

### Wish-list Features

-   Custom keybinds in config file
-   Custom color themes
-   UML diagram creation (unfeasible)
-   Custom plugin API for other devs/modders (unfeasible)
-   Live collaborative editing (unfeasible)

## Risks

-   **Potential complexity in managing multiple productivity tools.**

    -   _Mitigation:_ Modularize development and focus on core functionalities first.

-   **Potential performance issues with large tables or lists.**

    -   _Mitigation:_ Load and display only one section at a time.

-   **Integrating the graphical TUI with multiple productivity tools may lead to UI challenges.**

    -   _Mitigation:_ Frequent testing to ensure all elements integrate properly, and make sure that all features of the app are accessible with solely keyboard shortcuts.

-   **Keyboard shortcuts for navigation within graphical TUI may not be intuitive to design.**

    -   _Mitigation:_ Lots of live testing and add support for many popular muscle memory shortcuts, that provide the same result (e.g., `Alt + [number]` and `Ctrl + [number]` doing the same thing, that is, switching to tab `[number]`).

-   **Rich text editing without special formatting may be tricky to execute.**

    -   _Mitigation:_ Use vim-like controls, allowing users to enter different modes, like insert mode or normal mode, where normal mode allows for more keyboard shortcuts for rich text editing.

-   **Displaying rich text as plain text in insert mode.**

    -   _Mitigation:_ Code to handle a custom file format.

-   **Learning curve for advanced TUI programming and C++ libraries.**
    -   _Mitigation:_ Allocate extra time for training and experimentation, and pair less experienced members with those who have prior exposure to similar technologies.

## Other Notes

### File Format Ideas

-   **Custom binary** (best speed but might be hard)
    -   Store metadata in the first couple of bytes
    -   Store text as string blocks, where we have content and styles
    -   For parsing, we can either:
        -   Store the length of the block at the start of the block to prevent reading bytes outside of the range
        -   Use a special character as a null terminator
-   **Text edits should be made to memory**; the user writes to disk with a command/keyboard shortcut like Vim
-   **Example:**

    -   `"Hello"` = `0x05 0x48 0x65 0x6C 0x6C 0x6F 0x01`
        -   First byte: length to loop to parse the block
        -   Last byte: example style flag for bold (we can have a bunch of `0`s to represent flags that can be toggled on or off, or maybe represent styles in a different way)

-   **Json**
-   **Markdown**

## List of Libraries to Potentially Use

-   `ncurses` – for TUI implementation and keyboard shortcuts
-   `SQLite` – for storing and managing data locally in the text editor (for calendar events, task lists, etc.)
-   `chrono` – for keeping track of time for clock and timer features (if implemented)
-   System calls – for displaying PC stats/monitoring (RAM, CPU, disk space, uptime, etc.)

## References

-   **Feature inspiration (Notion):** [Notion](https://www.notion.com/product)
-   **Raspberry Pi Documentation:** [Raspberry Pi](https://www.raspberrypi.com/documentation/)
-   **UI/ASCII art inspiration:** [Neofetch](https://en.wikipedia.org/wiki/Neofetch)

## User Stories

### Required Features

#### User Story 1:

**As a user, I can bold text in the text editor so that I can emphasize important points in my writing.**

-   **Acceptance Tests:**
    -   Verify that selecting text and pressing a specific shortcut (e.g., `Ctrl+B`) bolds the text.
    -   Ensure that the bold text is displayed correctly in the editor.
-   **Story Points:** 2

#### User Story 2:

**As a user, I can italicize text so that I can highlight important ideas.**

-   **Acceptance Tests:**
    -   Verify that pressing `Ctrl+I` on selected text italicizes it.
    -   Ensure the text appears italicized after applying the formatting.
-   **Story Points:** 2

#### User Story 3:

**As a user, I can create a task in the Kanban board so that I can manage my tasks effectively.**

-   **Acceptance Tests:**
    -   Verify that clicking on the “Add Task” button allows the user to create a new task.
    -   Ensure that the task is visible on the board after creation.
-   **Story Points:** 3

#### User Story 4:

**As a user, I can move tasks between columns (e.g., from “To Do” to “In Progress”) so that I can track my progress.**

-   **Acceptance Tests:**
    -   Verify that tasks can be dragged and dropped to different columns.
    -   Ensure that the task’s status changes when moved.
-   **Story Points:** 3

#### User Story 5:

**As a user, I can add calendar events so that I can manage my schedule.**

-   **Acceptance Tests:**
    -   Verify that users can create events with a title, date, and description.
    -   Check that the event appears in the calendar view after being created.
-   **Story Points:** 3

#### User Story 6:

**As a developer, I can implement a custom binary file format for the text editor so that the user’s data can be saved efficiently and securely.**

-   **Acceptance Tests:**
    -   Verify that the editor can save and load files using the custom binary format.
    -   Ensure that the data is correctly parsed and displayed when reopened.
-   **Story Points:** 5

#### User Story 7:

**As a user, I want efficient keybindings for all actions so that I can navigate and edit everything efficiently.**

-   **Acceptance Tests:**
    -   Verify keybindings exist for all core actions (e.g., creating tables, switching between pages, etc.).
    -   Confirm that the keybindings are documented in a help menu.
-   **Story Points:** 4

#### User Story 8:

**As a user, I can easily create tables in the text editor so that I can organize information effectively.**

-   **Acceptance Tests:**
    -   Verify a table can be inserted with a keyboard shortcut (e.g., `CTRL+T`).
    -   Ensure users can specify the number of rows and columns of the table.
    -   Confirm tables can be edited after their creation.
-   **Story Points:** 3

#### User Story 9:

**As a user, I can view a list of my pages in a sidebar so that I can easily navigate between them.**

-   **Acceptance Tests:**
    -   Verify that the sidebar displays all existing pages in an organized list.
    -   Ensure that clicking on a page in the sidebar opens it in the main editor.
-   **Story Points:** 3

---

### Optional Features

#### User Story 10:

**As a user, I can lock a page with a password so that I can protect sensitive information from unauthorized access.**

-   **Acceptance Tests:**
    -   Verify that the user can set a password on a specific page.
    -   Ensure that attempting to open a locked page requires the correct password before displaying its content.
-   **Story Points:** 4

#### User Story 11:

**As a user, I can link one page to another so that I can quickly navigate between related notes or documents.**

-   **Acceptance Tests:**
    -   Verify that the user can create a link to another existing page by specifying its title or identifier.
    -   Ensure that clicking or selecting the link in the editor jumps to the linked page.
-   **Story Points:** 3

#### User Story 12:

**As a user, I can create nested subpages within a main page (or folder) so that I can keep my notes organized hierarchically.**

-   **Acceptance Tests:**
    -   Verify that a user can create a new subpage under an existing page (folder).
    -   Ensure the nested structure is visible and navigable in the sidebar or file explorer.
-   **Story Points:** 4

#### User Story 13:

**As a user, I can enable optional widgets (e.g., clock, weather, ASCII) in my terminal interface so that I can have quick, at-a-glance information.**

-   **Acceptance Tests:**
    -   Verify that a user can toggle widgets on or off through a settings menu or command-line argument.
    -   Ensure that widgets update in real time (e.g., clock changes every minute, weather updates periodically).
-   **Story Points:** 5

#### User Story 14:

**As a user, I can specify startup options via command-line arguments so that I can automatically open specific widgets or pages on launch.**

-   **Acceptance Tests:**
    -   Verify that users can pass arguments that enable widgets on startup.
    -   Confirm that invalid arguments return an appropriate error message.
-   **Story Points:** 4

#### User Story 15:

**As a user, I can access a man page or built-in help documentation for keybindings so that I can quickly learn or reference available shortcuts.**

-   **Acceptance Tests:**
    -   Verify run command (e.g., `--help`) displays documentation about keybindings and commands.
    -   Ensure that keybindings are listed in a clear, categorized format.
-   **Story Points:** 3

---

### Wish-list Features

#### User Story 16:

**As a user, I can define my own keyboard shortcuts so that I can use my preferred muscle-memory key combinations in the editor.**

-   **Acceptance Tests:**
    -   Verify that a user can open a configuration file or settings panel to set custom keybinds.
    -   Ensure the new keybinds override defaults once saved and loaded.
-   **Story Points:** 3

#### User Story 17:

**As a user, I can choose from multiple color schemes or create my own so that the TUI matches my preferences.**

-   **Acceptance Tests:**
    -   Verify that switching themes updates the entire interface.
    -   Ensure that custom themes can be saved and loaded on startup.
-   **Story Points:** 2

#### User Story 18:

**As a user, I can collaborate with others in real time on the same page so that we can co-edit notes simultaneously.**

-   **Acceptance Tests:**
    -   Verify that multiple users can connect to the same session and see each other’s changes in real time.
    -   Ensure that conflicts are handled gracefully (e.g., last write wins, or conflict resolution).
-   **Story Points:** 8

#### User Story 19:

**As a developer, I can create a plugin architecture so that third-party developers can extend the editor’s functionality with custom modules.**

-   **Acceptance Tests:**
    -   Verify that the application can dynamically load external plugin files.
    -   Ensure plugins can add, modify, or remove editor features without requiring a core code change.
-   **Story Points:** 10
