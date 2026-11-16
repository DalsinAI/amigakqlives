#!/bin/sh
# KQ's Lua scripts as the game reads them: each scripts/NAME.lua through
# cpp with include/itemdefs.h (the item numbers), as upstream's
# scripts/Makefile.am makes NAME.plua, kept as Lua source in NAME.lob.
#   tools/scripts.sh <kq source> <out dir>
# MIT, Copyright (c) 2026 AmiCradle Team.
set -eu
KQ=$1; OUT=$2
mkdir -p "$OUT"
n=0
for f in "$KQ"/scripts/*.lua; do
    b=$(basename "$f" .lua)
    cpp -P -undef -include "$KQ/include/itemdefs.h" "$f" > "$OUT/$b.lob"
    n=$((n + 1))
done
echo "scripts: $n"
