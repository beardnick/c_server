#!/usr/bin/env zsh

BASE="$HOME/.config/functions"

source "$BASE/ui.zsh"
source "$BASE/log.zsh"

for (( i = 0; i < 10000; i++ )); do
    echo "client: $i"
    ./client &
done

sleep_count 5

jobs
