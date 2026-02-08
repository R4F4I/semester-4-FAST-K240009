#!/bin/bash
directory=$1
name=$directory"_archive.tar.gz"
tar -czf $name $directory
