# Introduction
Welcome to NeoNote!

NeoNote is a terminal-based note-taking app created for `CS3307` at the *University of Western Ontario* during the **2025 Winter Semester**.

### Goal
Markdown editors in the terminal typically display formatting characters or require users to toggle between editing and viewing modes. 
Neonote, however, aims to streamline this process by enabling seamless markdown editing without exposing the formatting characters.
In addition, we've integrated productivity tools like a Kanban board and a calendar to further enhance workflow management.

### Design Philosophy
We believe that the terminal is a space for focused, efficient work, where simplicity and control are paramount. 
By creating a Markdown editor integrated with a calendar and Kanban board, we think were empowering users to seamlessly manage their tasks, schedule, and documentation without distractions.
We believe that the text-based nature of the terminal allows for greater clarity and concentration, fostering a deeper engagement with the content.
We chose to not to wrap lines and simply allow to give users full control over their environment.
We believe this approach promotes a mindful, purposeful workflow, where users are encouraged to think critically about the structure of their work while minimizing unnecessary visual distractions.
We think that by embracing horizontal space and removing the automatic line breaks, were not only preserving the integrity of the content but also aligning with the minimalist ethos that defines the terminal experience.
Ultimately, we believe that this philosophy supports both deep focus and flexible productivity, providing a streamlined and powerful tool for those who value precision and simplicity in their workflow.

### Creators
Group 14 - Gordon Xu, Runcong Zhu, Emily Ateo

# Features

#### Basic
    1. `.md` files saved in `/.local/share/neonote`  can be displayed and made available for editing
    2. Users are able to input and delete text
        2a. Inputted text is displayed
    **    2b. Inputted text can be saved into a `.md` file
    3. There is a cursor that accurately tracks the position of the text being inputted
        3a. The cursor can be moved
        3b. When the cursor moves outside the bounds of the screen, the screen scrolls
    4. New files can be created

#### Intermediate
    1. Markdown formatting is rendered
        1a. Italics is rendered when `*text*` text is typed
        1b. Bold is rendered when `**bold**` text is typed
        1c. Headers are rendered with a colored text when  `# Header 1` - `###### Header 6` is typed
        1d. Code snippers are rendered with text surrounded by backticks (currently backticks cannot be rendered in code)
        1e. Code blocks are rendered with text surrounded by sets of 3 backticks on columns before and after
        1f. Asterisks can be escaped when `\*` is typed 
    2. Markdown formatting characters are hidden
    3. A word can be deleted in one input
    4. Cursor can be moved across a word in one input
    5. Typing ` -` or `{number}. ` at the beginning of a line will indent it
        5a. Pressing back and space again will indent extra levels
    6. A calendar can be viewed

#### Advanced
    1. **This** welcome page is moved to `/.local/share/neonote`  if the directory is empty when opened
    2. Bold and Italics can be toggled on with a keybind that automatically creates both bounds of the formatting
        2a. Bold and Italics can be toggled off with a keybind that will move to the position after the formatting characters
    3. Events can be created and deleted in Calendar 
    4. Tasks can be created and deleted in Kanban board
        4a. Tasks can be moved between statuses in Kanban board

# Keybinds
Keybinds can be modified in `Settings.h`. You will have to recompile after modifying it.
The default keybind are below.

###### General Navigation
    - **Arrow Keys**
        - ```UP`: Move cursor up
        - `DOWN`: Move cursor down
        - `LEFT`: Move cursor left
        - `RIGHT`: Move cursor right
    - **Fast Navigation**
        - `Ctrl + ]`: Move cursor one word left
        - `Ctrl + {backslash}`: Move cursor one word right
        - `Page Up`: Moves to top of page
        - `Page Down`: Moves to bottom of page

###### Editing
    - **Special Modifiers**
        - `Ctrl + Backspace`: Delete word
        - `Ctrl + Tab`: Tab
    - **Formatting**
        - `Ctrl + B`: Toggle Bold
        - `Ctrl + I`: Toggle Italics

###### File Management
    - `Ctrl + N`: New File
    - `Ctrl + S`: Save File
    - `Ctrl + R`: Rename File
    - `Del`: Delete File

###### UI Navigation
    - `Ctrl + O`: Switch Panel

###### Kanban
    - `LEFT`: Move task left
    - `RIGHT`: Move task right

# Customization
Indentation size and sidebard ratio can also be changed in `Settings.h`

```
Thanks for using neonote!
```

    - Group 14
