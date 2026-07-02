#!/usr/bin/env bash

echo "Setup required Environment"
echo "-------------------------------------"

RAYLIB_PATH="/c/raylib/raylib"
COMPILER_PATH="/c/raylib/w64devkit/bin"

export PATH="$COMPILER_PATH:$PATH"

CC=gcc

CFLAGS="-s -static -O3 -std=c99 -Wall -Wshadow -Wunused-parameter \
-I${RAYLIB_PATH}/src \
-Iexternal \
-DPLATFORM_DESKTOP \
-L."

LDFLAGS="-lraylib -lopengl32 -lgdi32 -lwinmm -lshcore"

SOURCE="$1"
OUTPUT="${SOURCE%.c}.exe"

echo
echo "Clean latest build"
echo "------------------------"

rm -f "$OUTPUT"

echo
echo "Compile program"
echo "-----------------------"

$CC --version

echo $CC -o "$OUTPUT" "$SOURCE" $CFLAGS $LDFLAGS

$CC -o "$OUTPUT" "$SOURCE" $CFLAGS $LDFLAGS

echo
echo "Execute program"
echo "-----------------------"

if [[ -f "$OUTPUT" ]]; then
    "./$OUTPUT"
fi

