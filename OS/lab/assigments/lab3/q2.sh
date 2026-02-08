#!/bin/bash
file1=$1
file2=$2
if cmp $file1 $file2
then
    echo "The files are identical"
else
    echo "The files are different"
fi
