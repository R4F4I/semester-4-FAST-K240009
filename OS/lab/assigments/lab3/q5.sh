#!/bin/bash
folder=$1
current_date=$(date +%Y-%m-%d)
backup_name="project_backup_"$current_date
mkdir $backup_name
cp $folder/* $backup_name
count=$(ls $backup_name | wc -l)
echo "Files backed up: $count"
echo "Backup successful"
