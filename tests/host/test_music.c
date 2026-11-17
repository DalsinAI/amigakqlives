/* Host test of amiga/music_gfopenmusic.c, KQ's music on gfopenmusic:
 * the songs' paths, looping, volume, a battle's song nested over the
 * map's and given back, and silence with no library.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include <stdio.h>
#include <string.h>
#include <proto/exec.h>
#include <gfopenmusic.h>
#include "music.h"

unsigned long kq_gm_volume(float v);

unsigned char is_sound = 1;
struct Library *__gfopenmusic_base;
static struct Library fake;
static int have_lib = 1, opened, closed, playing, paused[8];
static long last_loops;
static unsigned long last_vol, open_vol;
static char last_name[256];
static struct GMSong { int id; } songs[8];
static int nsongs;
static char log_[512];

const char *kqres(int dir, const char *file)
{
	static char b[256];
	snprintf(b, sizeof b, "PROGDIR:%s/%s", dir == 3 ? "music" : "?", file);
	return b;
}
struct Library *OpenLibrary(CONST_STRPTR name, unsigned long v)
{
	(void)v;
	return have_lib && !strcmp(name, "PROGDIR:Libs/gfopenmusic.library") ? &fake : 0;
}
int gm_available(void) { return __gfopenmusic_base != 0; }
struct GMSong *gm_open(const char *name, const struct TagItem *t)
{
	strcpy(last_name, name);
	open_vol = t[0].ti_Tag == GMA_Volume ? t[0].ti_Data : 999;
	opened++;
	songs[nsongs].id = nsongs;
	return &songs[nsongs++];
}
void gm_close(struct GMSong *s) { (void)s; closed++; }
int gm_play(struct GMSong *s, long loops, unsigned long f) { (void)f; last_loops = loops; playing = s->id; return 1; }
void gm_stop(struct GMSong *s) { (void)s; }
void gm_pause(struct GMSong *s) { paused[s->id] = 1; sprintf(log_ + strlen(log_), "P%d ", s->id); }
void gm_resume(struct GMSong *s) { paused[s->id] = 0; sprintf(log_ + strlen(log_), "R%d ", s->id); }
void gm_set_volume(struct GMSong *s, unsigned long v) { (void)s; last_vol = v; }

static int pass, fail;
#define CHECK(c, what) do { if (c) pass++; else { fail++; printf("FAIL: %s\n", what); } } while (0)

int main(void)
{
	CHECK(kq_gm_volume(1.0f) == 256 && kq_gm_volume(0.0f) == 0 && kq_gm_volume(0.5f) == 128, "volume scale");
	CHECK(kq_gm_volume(-1.0f) == 0 && kq_gm_volume(2.0f) == 256, "volume clamped");

	init_music();
	CHECK(__gfopenmusic_base == &fake, "library from PROGDIR:Libs");
	set_music_volume(0.75f);
	play_music("town1.s3m", 0);
	CHECK(!strcmp(last_name, "PROGDIR:music/town1.s3m"), "song path from kqres(MUSIC_DIR)");
	CHECK(last_loops == -1, "looped for ever");
	CHECK(open_vol == 192, "opened at the set volume");
	/* a battle: the map's song pauses, the battle's plays, then back */
	pause_music();
	CHECK(paused[0], "map song paused");
	play_music("aa_arofl.xm", 0);
	CHECK(playing == 1 && closed == 0, "battle song plays, map song kept");
	resume_music();
	CHECK(closed == 1 && !paused[0], "battle song closed, map song resumed");
	set_music_volume(0.25f);
	CHECK(last_vol == 64, "volume to the playing song");
	play_music("town2.s3m", 0);
	CHECK(closed == 2 && opened == 3, "a new song replaces the old");
	shutdown_music();
	CHECK(closed == 3, "shutdown closes the song");

	/* no library: silent, no calls */
	__gfopenmusic_base = 0; have_lib = 0; opened = 0;
	init_music();
	play_music("town1.s3m", 0);
	pause_music(); resume_music(); stop_music();
	CHECK(opened == 0, "no library: no song opened");
	printf("music: %d passed, %d failed\n", pass, fail);
	return fail != 0;
}
