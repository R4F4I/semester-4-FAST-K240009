#!/bin/bash

# Check if a directory path is provided
if [ -z "$1" ]; then
    echo "Usage: $0 /path/to/directory"
    exit 1
fi

# Get the base name of the directory (removes trailing slashes)
DIR_NAME=$(basename "$1")
ARCHIVE_NAME="${DIR_NAME}_archive.tar.gz"

# Create the compressed archive
tar -czf "$ARCHIVE_NAME" "$1"

echo "Archive created successfully: $ARCHIVE_NAME"