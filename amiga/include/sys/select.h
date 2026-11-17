/* KQ waits with select(0, NULL, NULL, NULL, &timeout) and nothing else
 * (src/timing.c): on AmigaOS that is Allegro's rest(), which sleeps on
 * the timer.device. The C library's own header comes first (sys/types.h
 * reads it too); the macro only changes KQ's calls after it.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include_next <sys/select.h>
#ifndef KQ_AMIGA_SELECT
#define KQ_AMIGA_SELECT
void rest(unsigned int ms);
#define select(n, r, w, e, t) (rest((unsigned int)((t)->tv_usec / 1000)), 0)
#endif
