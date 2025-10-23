#!/bin/bash

# Generate tama EEP files from tama04.eep to tama32.eep

for i in $(seq 1 32); do
    # Format number with leading zero if needed
    num=$(printf "%02d" $i)

    echo "Generating tama${num}.eep..."

    # Start tamaemu in background
    ./tamaemu &
    TAMAEMU_PID=$!

    # Wait 5 seconds
    sleep 5

    # Kill tamaemu
    kill $TAMAEMU_PID 2>/dev/null
    wait $TAMAEMU_PID 2>/dev/null

    # Copy the generated tama.eep to tamas folder
    mv tama.eep "../tamas/tama${num}.eep"
    echo "Created ../tamas/tama${num}.eep"

    # Small delay between iterations
    sleep 1
done

echo "Done!"
