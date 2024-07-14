#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Использование: $0 <имя_процесса>"
    exit 1
fi

process_name="$1"

ps | grep "$process_name"










