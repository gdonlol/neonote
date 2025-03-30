#!/bin/bash

# Define the source and target locations
SOURCE="./README.md"
TARGET="$HOME/.local/share/neonote/"

if [ -f "$SOURCE" ]; then
    if [ ! -d "$TARGET" ]; then
        mkdir -p "$TARGET"
    fi
    
    mv "$SOURCE" "$TARGET"
    echo "README.md has been moved to $TARGET"
else
    echo "Error: $SOURCE does not exist."
fi

