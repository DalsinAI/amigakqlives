/* KQ on AmigaOS plays no music yet (in place of src/music.c, which needs
 * DUMB): the calls are kept, silent. The sound effects are Allegro's.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include "kq.h"
#include "music.h"

void init_music(void) {}
void shutdown_music(void) {}
void poll_music(void) {}
void set_music_volume(float volume) { (void)volume; }
void play_music(const char *music_name, long position) { (void)music_name; (void)position; }
void pause_music(void) {}
void resume_music(void) {}
void stop_music(void) {}
