# KQ Lives for GFOS

KQ, a classic console-style RPG (KQ Lives 0.99.3), on AmigaOS 3.2 with a 68040 or 68060.

- Run KQ from its drawer. Saved games and settings go to its User drawer, which an update leaves alone.
- It runs on our own libraries, in its Libs drawer: allegro4.library (the screen, keys, pads and sound effects, through acgame.library), lua51.library (the game's scripts) and gfopenmusic.library (the songs).
- The songs (MOD, S3M, XM in the music drawer) play through gfopenmusic.library, in Libs too, with gfopenmodule.datatype and AHI; without them the game plays silent. The sound effects are Allegro's.
- Keys: the cursor keys move, Return or Space acts, Esc opens the menu (change them in the game's setup).

KQ is Copyright (C) 2002 Josh Bolduc and the KQ Lives team, GPL-2 (COPYING). Source: https://github.com/OnlineCop/kq-fork (v0.99.3). This port: https://github.com/DalsinAI/amigakqlives. Port files MIT, Copyright (c) 2026 AmiCradle Team.
