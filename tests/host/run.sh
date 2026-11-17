#!/bin/sh
# KQ for GFOS: the host tests.
#  1. test_paths: amiga/kq_amiga.c's file paths (with stand-ins for Allegro
#     and dos.library);
#  2. test_music: amiga/music_gfopenmusic.c, KQ's songs on gfopenmusic
#     (with stand-ins for exec and gfopenmusic);
#  3. every one of KQ's scripts, through tools/scripts.sh as the build
#     ships it, parses as Lua 5.1 (luac -p, built here from Lua 5.1.5's
#     source, the Lua lua51.library is), and every script upstream's
#     scripts/Makefile.am installs is there.
#   KQ=<kq source> LUA_SRC=<lua-5.1.5 source> sh tests/host/run.sh
# MIT, Copyright (c) 2026 AmiCradle Team.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT

cc -Wall -Werror -I"$ROOT/tests/host/stubs" -I"$ROOT/amiga/include" -I"$KQ/include" \
    -o "$T/test_paths" "$ROOT/tests/host/test_paths.c" "$ROOT/amiga/kq_amiga.c"
"$T/test_paths"
cc -Wall -Werror -I"$ROOT/tests/host/stubs" -I"$ROOT/amiga/include" -I"$KQ/include" \
    -o "$T/test_music" "$ROOT/tests/host/test_music.c" "$ROOT/amiga/music_gfopenmusic.c"
"$T/test_music"

mkdir -p "$T/lua"
for f in "$LUA_SRC"/src/*.c; do
    case $(basename "$f") in lua.c) continue;; esac
    cc -O1 -w -DLUA_USE_POSIX -c "$f" -o "$T/lua/$(basename "$f" .c).o"
done
cc -o "$T/luac" "$T"/lua/*.o -lm
sh "$ROOT/tools/scripts.sh" "$KQ" "$T/scripts"
bad=0
for f in "$T"/scripts/*.lob; do
    "$T/luac" -p "$f" || bad=$((bad + 1))
done
for want in $(sed -n '/^cscripts_DATA/,/^$/p' "$KQ/scripts/Makefile.am" | tr -s ' \\\t=' '\n' | grep '\.lob$'); do
    [ -f "$T/scripts/$want" ] || { echo "FAIL: no $want"; bad=$((bad + 1)); }
done
echo "scripts: $(ls "$T"/scripts/*.lob | wc -l) parse checks, $bad failed"
[ $bad = 0 ]
