#!/bin/sh


if [ $# -ne 1 ]; then
    echo "Использование: $0 <имя_программы.c>"
    exit 1
fi


source_file="$1"


program_name="${source_file%.c}"


gcc "$source_file" -o "$program_name"


if [ $? -eq 0 ]; then
    
    ./"$program_name"
else
    
    vi "$source_file"
fi