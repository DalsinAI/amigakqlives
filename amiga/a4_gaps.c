/* Allegro calls KQ makes that allegro4.library does not have yet; they go
 * into GFOpenAllegro, and this file goes when the lock moves to it.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include <allegro.h>
#include "config.h"

/* allegro4.library's config calls give the defaults and keep nothing, so
 * there is no state to push or pop */
void push_config_state(void) {}
void pop_config_state(void) {}
