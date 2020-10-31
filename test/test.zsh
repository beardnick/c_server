#!/usr/bin/env zsh

for (( i = 0; i < 10; i++ )){
    if (( i> 6 )) {
        print -P "%F{red}" "true"
        continue
    }
    if (( i> 4 )) {
        print -P "%F{yellow}" "true"
        continue
    }
    if (( i> 2 )) {
        print -P "%F{green}" "true"
        continue
    }
}
