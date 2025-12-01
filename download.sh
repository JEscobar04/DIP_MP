#!/bin/bash

# Usage: ./download.sh [N]
# Downloads one PPM file and makes N sequential copies

if [ $# -ne 1 ]; then
    echo "Usage: $0 [NUMBER_OF_COPIES]"
    exit 1
fi

COUNT=$1

SOURCE_URL="https://filesamples.com/samples/image/ppm/sample_1920%C3%971280.ppm"
OUTDIR="images"
mkdir -p "$OUTDIR"

ORIGINAL="$OUTDIR/original.ppm"

echo "Downloading PPM file: $(basename $SOURCE_URL)"
if command -v curl >/dev/null 2>&1; then
    curl -s -L "$SOURCE_URL" -o "$ORIGINAL"
elif command -v wget >/dev/null 2>&1; then
    wget -q "$SOURCE_URL" -O "$ORIGINAL"
else
    echo "Error: Need curl or wget."
    exit 1
fi

# Make N copies
echo "Creating $COUNT copies..."
for ((i=1; i<=COUNT; i++)); do
    FNAME=$(printf "%s/img_%03d.ppm" "$OUTDIR" "$i")
    cp "$ORIGINAL" "$FNAME"
done

rm $ORIGINAL

echo "$COUNT copies in: $OUTDIR/"
