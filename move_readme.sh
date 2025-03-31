#!/bin/bash

SOURCE="./Welcome.md"
TARGET="$HOME/.local/share/neonote/"

if [ -f "$SOURCE" ]; then
    if [ ! -d "$TARGET" ]; then
        mkdir -p "$TARGET"
    fi
    
    cp "$SOURCE" "$TARGET"
    echo "Welcome.md has been copied to $TARGET"
else
    echo "Error: $SOURCE does not exist."
fi

