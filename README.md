# amigakqlives

KQ Lives 0.99.3 (OnlineCop/kq-fork), the classic console-style RPG, for GFOS: AmigaOS 3.2 with GFOpenUp on a 68040 or 68060. It is built on our own libraries:
- allegro4.library (DalsinAI/gfopenallegro) for the screen, keys, pads, sound effects and datafiles, drawing through acgame.library;
- lua51.library (DalsinAI/gfopenlua) for the game's Lua scripts.

Upstream is fetched at build time at the commit in `package/kqlives.lock` and built unchanged. The Amiga parts are in `amiga/`:
- `kq_amiga.c`: where the files are, in place of `src/unix.c`. Everything sits in the game's drawer, and saves go to its `User/` drawer.
- `music_gfopenmusic.c`: KQ's music calls (in place of `src/music.c`, which needs DUMB) on gfopenmusic.library (DalsinAI/gfopenmusic): songs looped, volume, a battle's song nested over the map's.
- `include/config.h`: what configure would write, plus KQ's Lua 5.2 `lua_load` call mapped onto Lua 5.1.
- `include/sys/select.h`: KQ's waits go to Allegro's `rest()`.

The scripts are run through cpp as upstream does, and shipped as Lua source (`tools/scripts.sh`).

- Build: `package/build-amiga.sh` (Cradle stove_build, target package). It makes the drawer, `KQ-0.99.3-gfos1-68040.lha` with Install-KQ, and the GFOpenUp part.
- Host tests: `tests/host/run.sh` checks the file paths (13 checks), the music glue (13 checks) and that all 41 scripts parse as Lua 5.1.

KQ is GPL-2 (Josh Bolduc and the KQ Lives team). The port's own files are MIT, Copyright (c) 2026 AmiCradle Team.
