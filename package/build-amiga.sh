#!/bin/sh
# KQ Lives for GFOS, for Cradle's stove_build (target package):
#  1. upstream KQ, GFOpenAllegro, acgame and GFOpenLua, each fetched at the
#     commit package/kqlives.lock names (none is kept in this repository);
#  2. allegro4.library + liballeg.a, acgame.library, lua51.library +
#     liblua51.a, built with their own build scripts;
#  3. the host tests (tests/host/run.sh): the scripts, the file paths;
#  4. the game (amiga/Makefile.amigaos);
#  5. the drawer build/amigaos/KQ: KQ, data/, maps/, scripts/ (KQ's Lua
#     scripts through cpp, as upstream's scripts/Makefile.am does, and
#     shipped as source: lua51.library reads source and bytecode alike),
#     Libs/ with the four libraries, User/ for saved games, the icons and
#     Install-KQ beside it, music/ with KQ's songs (played through
#     gfopenmusic.library, in Libs/ too);
#  6. KQ-VERSION-68040.lha (tools/pack_dist.py) and OUT/Parts/KQ, a
#     GFOpenUp part (package/write-part.sh).
# Nothing is published from here.
# MIT, Copyright (c) 2026 AmiCradle Team.
set -eu

HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$HERE/.." && pwd)
. "$HERE/kqlives.lock"
STOVE=${STOVE:-"$HOME/AmiCradle/kitchen/stoves/os32-gcc16-open6"}
OUT=${OUT:-"$ROOT/build-amiga-package"}
mkdir -p "$OUT"
OUT=$(CDPATH= cd -- "$OUT" && pwd)
# outside the checkout: Cradle cleans it (git clean -ffdx) before every job
WORK=${WORK:-"$HOME/.cache/amigakqlives"}
mkdir -p "$WORK"
cd "$ROOT"
t0=$(date +%s)

# 1. the sources at their locked commits
get() { # dir url commit
    if [ "$(git -C "$1" rev-parse HEAD 2>/dev/null)" != "$3" ]; then
        rm -rf "$1"; mkdir -p "$1"
        git -C "$1" init -q
        git -C "$1" fetch -q --depth 1 "$2" "$3"
        git -C "$1" checkout -q FETCH_HEAD
    fi
    git -C "$1" checkout -q -- . && git -C "$1" clean -qfdx
}
get "$WORK/kq" "$KQ_URL" "$KQ_COMMIT"
get "$WORK/gfopenallegro" "$ALLEGRO_URL" "$ALLEGRO_COMMIT"
get "$WORK/acgame" "$ACGAME_URL" "$ACGAME_COMMIT"
get "$WORK/gfopenlua" "$LUA51_URL" "$LUA51_COMMIT"
get "$WORK/gfopenmusic" "$GFMUSIC_URL" "$GFMUSIC_COMMIT"

# 2. our libraries
STOVE="$STOVE" OUT="$WORK/allegro-out" sh "$WORK/gfopenallegro/allegro4/build-amiga.sh" > "$WORK/allegro.log" 2>&1 ||
    { tail -n 40 "$WORK/allegro.log"; exit 1; }
STOVE="$STOVE" OUT="$WORK/acgame-out" sh "$WORK/acgame/build-amiga.sh" > "$WORK/acgame.log" 2>&1 ||
    { tail -n 40 "$WORK/acgame.log"; exit 1; }
STOVE="$STOVE" OUT="$WORK/lua51-out" LUA_TGZ="${LUA_TGZ:-}" sh "$WORK/gfopenlua/build-amiga.sh" > "$WORK/lua51.log" 2>&1 ||
    { tail -n 40 "$WORK/lua51.log"; exit 1; }
STOVE="$STOVE" OUT="$WORK/gfmusic-out" HOST_TESTS=0 sh "$WORK/gfopenmusic/build-amiga.sh" > "$WORK/gfmusic.log" 2>&1 ||
    { tail -n 40 "$WORK/gfmusic.log"; exit 1; }
ACGAME_LIB=$(find "$WORK/acgame-out" -name acgame.library | head -n 1)
[ -n "$ACGAME_LIB" ] || { echo "no acgame.library"; exit 1; }

# 3. the host tests
if command -v cc >/dev/null 2>&1 && command -v cpp >/dev/null 2>&1; then
    KQ="$WORK/kq" LUA_SRC="$WORK/lua51-out/lua-5.1.5" sh tests/host/run.sh
fi

# 4. the game
if [ -x "$STOVE/prefix/bin/m68k-amigaos-gcc" ]; then PREFIX="$STOVE/prefix"; else PREFIX="$STOVE"; fi
make -f amiga/Makefile.amigaos KQ="$WORK/kq" ALLEG="$WORK/allegro-out" \
    ALLEG_INC="$WORK/gfopenallegro/allegro4/include" LUA51="$WORK/lua51-out" \
    GFMUSIC="$WORK/gfopenmusic" GFMUSIC_OUT="$WORK/gfmusic-out" \
    STOVE="$STOVE" PREFIX="$PREFIX" -j4 all > build-output.txt 2>&1 || { tail -n 60 build-output.txt; exit 1; }

# 5. the drawer
D=build/amigaos/KQ
rm -rf "$D/data" "$D/maps" "$D/scripts" "$D/Libs" "$D/music"
mkdir -p "$D/data" "$D/maps" "$D/scripts" "$D/Libs" "$D/User" "$D/music"
cp "$WORK"/kq/music/* "$D/music/"
cp "$WORK"/kq/data/* "$D/data/"
cp "$WORK"/kq/maps/*.map "$D/maps/"
sh tools/scripts.sh "$WORK/kq" "$D/scripts"
cp "$WORK/allegro-out/allegro4.library" "$ACGAME_LIB" "$WORK/lua51-out/lib/lua51.library" "$WORK/gfmusic-out/lib/gfopenmusic.library" "$D/Libs/"
cp "$WORK/kq/COPYING" "$D/COPYING"
cp AMIGA_PORT.md "$D/ReadMe"
# the icons and the Installer script (amiga/icons, amiga/Install-KQ)
for i in KQ ReadMe COPYING User data maps scripts Libs music; do cp "amiga/icons/$i.info" "$D/"; done
cp amiga/icons/KQ-drawer.info build/amigaos/KQ.info
cp amiga/Install-KQ amiga/Install-KQ.info build/amigaos/
grep -a -q '[$]VER: KQ ' "$D/KQ" || { echo "FAIL: no \$VER string in KQ"; exit 1; }
ls -l "$D/KQ" "$D/Libs"
echo "drawer: $(find "$D" -type f | wc -l) files, $(du -sk "$D" | cut -f1) KB"

# 6. the archive (KQ-VERSION-68040.lha) and the GFOpenUp part
python3 tools/pack_dist.py --dist build/amigaos --name KQ --out "$OUT"
sh package/write-part.sh "$D" "$OUT/Parts/KQ" KQ "$PORT_VERSION" \
    "KQ Lives: a classic console-style RPG, on allegro4.library and lua51.library"
echo "part: $(grep -c '^copy' "$OUT/Parts/KQ/Part") files"
echo "total: $(( $(date +%s) - t0 )) s"
