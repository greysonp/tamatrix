#!/bin/bash

# Create a new tmux session named 'tamatrix'
SESSION="tamatrix"

# Check if session already exists
tmux has-session -t $SESSION 2>/dev/null

if [ $? != 0 ]; then
    # Create new session with first window for web server
    tmux new-session -d -s $SESSION -n "web"
    tmux send-keys -t $SESSION:0 "cd web && php -S localhost:5000" C-m

    # Create second window for tamaserver
    tmux new-window -t $SESSION -n "server"
    tmux send-keys -t $SESSION:1 "./server/tamaserver" C-m

    # Create a window for each .eep file in tamas directory
    if [ -d "tamas" ]; then
        for eepfile in tamas/*.eep; do
            if [ -f "$eepfile" ]; then
                # Extract filename without path for window name
                filename=$(basename "$eepfile")
                tmux new-window -t $SESSION -n "$filename"
                tmux send-keys -t $SESSION "cd emu && ./tamaemu -e ../$eepfile" C-m
            fi
        done
    fi

    echo "Tmux session '$SESSION' created"
else
    echo "Tmux session '$SESSION' already exists"
fi

