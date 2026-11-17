/* KQ's music on AmigaOS through gfopenmusic.library (in place of
 * src/music.c, which plays through DUMB): the same calls, the same nesting
 * (a battle pauses the map's song and plays its own; resume_music() stops
 * that and resumes the map's), the songs in the drawer's music/ played
 * looped, MOD, S3M and XM alike, by gfopenmodule.datatype or the host's
 * decoder. With no gfopenmusic.library (or no AHI) the game plays silent,
 * as it did before. The sound effects are Allegro's.
 *
 * DUMB's start position (play_music's second argument) is only ever 0 in
 * KQ, so a song always starts at its beginning.
 * GPL-2.0-or-later, like KQ. Copyright (c) 2026 AmiCradle Team. */
#include <string.h>
#include <proto/exec.h>
#include <gfopenmusic.h>

#include "music.h"

/* from kq.h, which needs all of Allegro: this file is tested on its own */
extern unsigned char is_sound;
const char *kqres(int dir_id, const char *file);
#define MUSIC_DIR 3

/* the stub's library base: set here when the library is in the drawer */
extern struct Library *__gfopenmusic_base;

#define MAX_MUSIC_PLAYERS 3

static struct GMSong *song[MAX_MUSIC_PLAYERS];
static unsigned long volume = GM_VOLUME_FULL;
static int current;

/* KQ's 0.0 to 1.0, as gfopenmusic's 0 to GM_VOLUME_FULL */
unsigned long kq_gm_volume(float v);
unsigned long kq_gm_volume(float v)
{
	if (!(v > 0.0f))
		return 0;
	if (v >= 1.0f)
		return GM_VOLUME_FULL;
	return (unsigned long)(v * GM_VOLUME_FULL + 0.5f);
}

void init_music(void)
{
	int i;

	for (i = 0; i < MAX_MUSIC_PLAYERS; i++)
		song[i] = NULL;
	current = 0;
	/* LIBS: first, then the drawer's own Libs/ */
	if (is_sound && !gm_available())
		__gfopenmusic_base = OpenLibrary((CONST_STRPTR)"PROGDIR:Libs/" GFOPENMUSICNAME, GFOPENMUSIC_VERSION);
}

void shutdown_music(void)
{
	do
		stop_music();
	while (current-- > 0);
	current = 0;
}

void set_music_volume(float v)
{
	volume = kq_gm_volume(v);
	if (song[current])
		gm_set_volume(song[current], volume);
}

/* the library's own task plays the song: nothing to poll */
void poll_music(void) {}

void play_music(const char *music_name, long position)
{
	struct TagItem tags[2];

	(void)position;
	stop_music();
	if (!is_sound || !gm_available())
		return;
	tags[0].ti_Tag = GMA_Volume;
	tags[0].ti_Data = volume;
	tags[1].ti_Tag = TAG_DONE;
	tags[1].ti_Data = 0;
	song[current] = gm_open(kqres(MUSIC_DIR, music_name), tags);
	if (!song[current])
		return;
	if (!gm_play(song[current], -1, 0)) {
		gm_close(song[current]);
		song[current] = NULL;
	}
}

void pause_music(void)
{
	if (current < MAX_MUSIC_PLAYERS - 1) {
		if (song[current])
			gm_pause(song[current]);
		current++;
	}
}

void resume_music(void)
{
	if (current > 0) {
		stop_music();
		current--;
		if (song[current])
			gm_resume(song[current]);
	}
}

void stop_music(void)
{
	if (song[current]) {
		gm_stop(song[current]);
		gm_close(song[current]);
		song[current] = NULL;
	}
}
