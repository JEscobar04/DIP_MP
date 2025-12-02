#!/bin/bash

# Usage: ./profile.sh [EXECUTABLE] [SIZE...]
# Profile an executable file given a list of input sizes.

if [ $# -lt 2 ]; then
    echo "Usage: $0 [EXECUTABLE] [SIZE...]"
    exit 1
fi

EXE=$1
shift
SIZES=("$@")

IMGDIR="images"

if [ ! -x "$EXE" ]; then
    echo "Error: '$EXE' not found or not executable."
    exit 1
fi

echo "Profiling $EXE with input sizes: ${SIZES[*]}"
# Set the OpenMP thread limit to the maximum number of available processors
OMP_THREAD_LIMIT=$(nproc)
export OMP_THREAD_LIMIT
echo "OMP_THREAD_LIMIT=$OMP_THREAD_LIMIT"
echo

# Clean up input files
echo "Cleaning input files..."
rm -f "$IMGDIR"/*.ppm
echo

run_index=1

for SIZE in "${SIZES[@]}"; do
    echo "=== Run $run_index | SIZE = $SIZE ==="

    # Download input images
    if ! ./download.sh "$SIZE"; then
        echo "Error: download.sh failed."
        exit 1
    fi

    # Run executable
    echo "Running $EXE"
    $EXE "$IMGDIR"/*.ppm
    EXITCODE=$?

    if [ $EXITCODE -ne 0 ]; then
        echo "Error: executable failed!"
        exit 1
    fi

    # Clean up input files
    echo "Cleaning input files..."
    rm -f "$IMGDIR"/*.ppm
    echo

    ((run_index++))
done

echo "Profiling complete."
