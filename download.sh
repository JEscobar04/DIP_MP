#!/bin/bash

# Usage: ./download.sh [N]
# Downloads 4 PPM files and makes N cyclic copies.

if [ $# -ne 1 ]; then
    echo "Usage: $0 [NUMBER_OF_COPIES]"
    exit 1
fi

COUNT=$1

URLS=(
"https://filesamples.com/samples/image/ppm/sample_640%C3%97426.ppm"
"https://filesamples.com/samples/image/ppm/sample_1280%C3%97853.ppm"
"https://filesamples.com/samples/image/ppm/sample_1920%C3%971280.ppm"
"https://filesamples.com/samples/image/ppm/sample_5184%C3%973456.ppm"
)

OUTDIR="images"
mkdir -p "$OUTDIR"

echo "Downloading 4 PPM files..."

ORIGINALS=()

for idx in "${!URLS[@]}"; do
    url="${URLS[$idx]}"
    fname="$OUTDIR/src_$((idx+1)).ppm"

    # Store name
    ORIGINALS+=("$fname")

    echo "Downloading: $(basename "$url")"

    if command -v curl >/dev/null 2>&1; then
        curl -s -L "$url" -o "$fname"
    elif command -v wget >/dev/null 2>&1; then
        wget -q "$url" -O "$fname"
    else
        echo "Error: Need curl or wget."
        exit 1
    fi
done

for ((i=1; i<=COUNT; i++)); do
    src_index=$(( (i-1) % 4 ))
    src_file="${ORIGINALS[$src_index]}"

    out_file=$(printf "%s/img_%03d.ppm" "$OUTDIR" "$i")
    cp "$src_file" "$out_file"
done

for f in "${ORIGINALS[@]}"; do
    rm -f "$f"
done

echo "$COUNT files created in: $OUTDIR/"
