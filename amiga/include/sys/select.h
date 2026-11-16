/* KQ waits with select(0, NULL, NULL, NULL, &timeout) and nothing else
 * (src/timing.c): on AmigaOS that is Allegro's rest(), which sleeps on
 * the timer.device. MIT, Copyright (c) 2026 AmiCradle Team. */
#ifndef KQ_AMIGA_SYS_SELECT_H
#define KQ_AMIGA_SYS_SELECT_H
#include <sys/time.h>
#define select(n, r, w, e, t) (rest((unsigned int)((t)->tv_usec / 1000)), 0)
#endif
