/* Host test: where KQ on AmigaOS looks for its files (amiga/kq_amiga.c).
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include <stdio.h>
#include <string.h>
#include <allegro.h>
#include <proto/dos.h>
#include "platform.h"

static int fails, checks;
static const char *present[8];
static int made_user, user_there;

int exists(const char *name)
{
	int i;

	for (i = 0; present[i]; i++)
		if (!strcmp(present[i], name))
			return 1;
	return 0;
}

BPTR Lock(CONST_STRPTR name, long mode)
{
	(void)mode;
	return !strcmp(name, "PROGDIR:User") && user_there;
}

BPTR CreateDir(CONST_STRPTR name)
{
	if (!strcmp(name, "PROGDIR:User"))
		made_user++;
	return 1;
}

void UnLock(BPTR lock) { (void)lock; }

static void same(const char *got, const char *want)
{
	checks++;
	if ((!got || !want) ? got != want : strcmp(got, want) != 0) {
		fails++;
		printf("FAIL: got %s, want %s\n", got ? got : "(null)", want ? want : "(null)");
	}
}

int main(void)
{
	same(kqres(DATA_DIR, "mpcx.dat"), "PROGDIR:data/mpcx.dat");
	same(kqres(MAP_DIR, "town1.map"), "PROGDIR:maps/town1.map");
	same(kqres(MUSIC_DIR, "town.s3m"), "PROGDIR:music/town.s3m");
	same(kqres(SAVE_DIR, "sg0.sav"), "PROGDIR:User/sg0.sav");
	checks++;
	if (made_user != 1) { fails++; printf("FAIL: User/ made %d times\n", made_user); }
	same(kqres(SETTINGS_DIR, "kq.cfg"), "PROGDIR:User/kq.cfg");
	checks++;
	if (made_user != 1) { fails++; printf("FAIL: User/ made again\n"); }
	/* scripts: .lob first, then .lua, else none */
	same(kqres(SCRIPT_DIR, "main"), NULL);
	present[0] = "PROGDIR:scripts/main.lua";
	same(kqres(SCRIPT_DIR, "main"), "PROGDIR:scripts/main.lua");
	present[1] = "PROGDIR:scripts/main.lob";
	same(kqres(SCRIPT_DIR, "main"), "PROGDIR:scripts/main.lob");
	same(get_resource_file_path("Work:KQ", "data", "x"), "Work:KQ/data/x");
	same(get_resource_file_path("Work:", "", "x"), "Work:x");
	same(kqres(99, "x"), NULL);
	printf("test_paths: %d checks, %d failed\n", checks, fails);
	return fails != 0;
}
