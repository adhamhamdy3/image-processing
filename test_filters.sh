#!/bin/bash

BINARY="./build/image-processing"
OUTPUT_DIR="./test_outputs"
LOG_FILE="./test_results.log"

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <samples_dir> [binary_path]"
    echo "  samples_dir   directory containing images to test"
    echo "  binary_path   optional, default: ./build/image-processing"
    exit 1
fi

SAMPLES_DIR="$1"
[ "$#" -ge 2 ] && BINARY="$2"

if [ ! -d "$SAMPLES_DIR" ]; then
    echo -e "${RED}Error:${NC} '$SAMPLES_DIR' is not a directory."
    exit 1
fi

if [ ! -x "$BINARY" ]; then
    echo -e "${RED}Error:${NC} Binary '$BINARY' not found or not executable."
    exit 1
fi

mkdir -p "$OUTPUT_DIR"
> "$LOG_FILE"

PASS=0
FAIL=0

run_filter() {
    local output="$1"
    shift
    local -a steps=("$@")

    ( IFS=$'\n'; printf "%s\n" "${steps[@]}" ) \
        | timeout 10 "$BINARY" > /dev/null 2>&1

    if [ -f "$output" ]; then
        echo -e "  ${GREEN}[PASS]${NC} $(basename "$output")"
        echo "PASS: $output" >> "$LOG_FILE"
        ((PASS++))
    else
        echo -e "  ${RED}[FAIL]${NC} $(basename "$output")"
        echo "FAIL: $output" >> "$LOG_FILE"
        ((FAIL++))
    fi
}

mapfile -t SAMPLES < <(find "$SAMPLES_DIR" -maxdepth 1 -type f \
    \( -iname "*.jpg" -o -iname "*.jpeg" -o -iname "*.png" \
       -o -iname "*.bmp" -o -iname "*.tga" \) | sort)

if [ "${#SAMPLES[@]}" -eq 0 ]; then
    echo -e "${YELLOW}No supported images found in '$SAMPLES_DIR'.${NC}"
    exit 1
fi

echo "Found ${#SAMPLES[@]} image(s) in '$SAMPLES_DIR'"
echo "Binary : $BINARY"
echo "Output : $OUTPUT_DIR"
echo ""

for INPUT in "${SAMPLES[@]}"; do
    SAMPLE=$(basename "$INPUT")
    SAFE=$(echo "$SAMPLE" | tr ' ' '_' | sed 's/\.[^.]*$//')
    OUT="$OUTPUT_DIR/$SAFE"
    mkdir -p "$OUT"

    echo "── $SAMPLE"
    echo "=== $SAMPLE ===" >> "$LOG_FILE"

    run_filter "$OUT/grayscale.jpg"    "1" "1"  "$INPUT"                       "$OUT/grayscale.jpg"    "16" "2"
    run_filter "$OUT/bw.jpg"           "1" "2"  "$INPUT"                       "$OUT/bw.jpg"           "16" "2"
    run_filter "$OUT/invert.jpg"       "1" "3"  "$INPUT"                       "$OUT/invert.jpg"       "16" "2"

    if [ "${#SAMPLES[@]}" -ge 2 ]; then
        INPUT2="${SAMPLES[1]}"
        run_filter "$OUT/merge_crop.jpg"   "1" "4" "$INPUT" "$INPUT2" "0" "$OUT/merge_crop.jpg"   "16" "2"
        run_filter "$OUT/merge_resize.jpg" "1" "4" "$INPUT" "$INPUT2" "1" "$OUT/merge_resize.jpg" "16" "2"
    fi

    run_filter "$OUT/flip_h.jpg"       "1" "5"  "$INPUT" "1"                   "$OUT/flip_h.jpg"       "16" "2"
    run_filter "$OUT/flip_v.jpg"       "1" "5"  "$INPUT" "2"                   "$OUT/flip_v.jpg"       "16" "2"
    run_filter "$OUT/rotate90.jpg"     "1" "6"  "$INPUT" "90"                  "$OUT/rotate90.jpg"     "16" "2"
    run_filter "$OUT/rotate180.jpg"    "1" "6"  "$INPUT" "180"                 "$OUT/rotate180.jpg"    "16" "2"
    run_filter "$OUT/rotate270.jpg"    "1" "6"  "$INPUT" "270"                 "$OUT/rotate270.jpg"    "16" "2"
    run_filter "$OUT/lighten.jpg"      "1" "7"  "$INPUT" "1"                   "$OUT/lighten.jpg"      "16" "2"
    run_filter "$OUT/darken.jpg"       "1" "7"  "$INPUT" "0"                   "$OUT/darken.jpg"       "16" "2"
    run_filter "$OUT/crop.jpg"         "1" "8"  "$INPUT" "1" "1" "50" "50"     "$OUT/crop.jpg"         "16" "2"
    run_filter "$OUT/frame_simple.jpg" "1" "9"  "$INPUT" "0" "1"               "$OUT/frame_simple.jpg" "16" "2"
    run_filter "$OUT/frame_fancy.jpg"  "1" "9"  "$INPUT" "1" "6"               "$OUT/frame_fancy.jpg"  "16" "2"
    run_filter "$OUT/edges.jpg"        "1" "10" "$INPUT"                       "$OUT/edges.jpg"        "16" "2"
    run_filter "$OUT/resize.jpg"       "1" "11" "$INPUT" "800" "600"           "$OUT/resize.jpg"       "16" "2"
    run_filter "$OUT/blur.jpg"         "1" "12" "$INPUT" "5"                   "$OUT/blur.jpg"         "16" "2"
    run_filter "$OUT/sunlight.jpg"     "1" "13" "$INPUT"                       "$OUT/sunlight.jpg"     "16" "2"
    run_filter "$OUT/oil.jpg"          "1" "14" "$INPUT"                       "$OUT/oil.jpg"          "16" "2"
    run_filter "$OUT/oldtv.jpg"        "1" "15" "$INPUT"                       "$OUT/oldtv.jpg"        "16" "2"
done

TOTAL=$((PASS + FAIL))
echo ""
echo "----------------------------------------"
echo -e "  Total : $TOTAL"
echo -e "  ${GREEN}Passed${NC} : $PASS"
echo -e "  ${RED}Failed${NC} : $FAIL"
echo "----------------------------------------"
echo "Outputs : $OUTPUT_DIR"
echo "Log     : $LOG_FILE"