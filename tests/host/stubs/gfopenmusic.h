/* Host test stand-in for gfopenmusic's <gfopenmusic.h>: the calls
 * amiga/music_gfopenmusic.c makes, recorded by tests/host/test_music.c. */
#ifndef STUB_GFOPENMUSIC_H
#define STUB_GFOPENMUSIC_H
#define GFOPENMUSICNAME "gfopenmusic.library"
#define GFOPENMUSIC_VERSION 1
#define GM_VOLUME_FULL 256
#define TAG_DONE 0UL
#define GMA_Volume 0x8DA16004UL
struct TagItem { unsigned long ti_Tag, ti_Data; };
struct GMSong;
struct Task;
int gm_available(void);
struct GMSong *gm_open(const char *name, const struct TagItem *tags);
void gm_close(struct GMSong *song);
int gm_play(struct GMSong *song, long loops, unsigned long fadems);
void gm_stop(struct GMSong *song);
void gm_pause(struct GMSong *song);
void gm_resume(struct GMSong *song);
void gm_set_volume(struct GMSong *song, unsigned long volume);
#endif
