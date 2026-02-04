#!/bin/bash

# Check if 2 arguments are provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 file1 file2"
    exit 1
fi

# Compare the files
if cmp -s "$1" "$2"; then
    echo "The files are identical."
else
    echo "The files are different."
fi