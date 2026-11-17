/* KQ on AmigaOS: where its files are (in place of src/unix.c). The game,
 * data/, maps/ and scripts/ sit in the game's drawer (PROGDIR:); the
 * player's saved games and settings go to its User/ drawer, made on the
 * first start and left alone by an update.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include <proto/dos.h>

#include "config.h"
#include "platform.h"

#define KQ_PATH_MAX 512

static const char *user_dir(void)
{
	static int made;

	if (!made) {
		BPTR lock = Lock((CONST_STRPTR)"PROGDIR:User", ACCESS_READ);

		if (!lock)
			lock = CreateDir((CONST_STRPTR)"PROGDIR:User");
		if (lock)
			UnLock(lock);
		made = 1;
	}
	return "PROGDIR:User";
}

/* str1/str2/file, or str1/file when str2 is empty */
static const char *join(char *ans, const char *str1, const char *str2, const char *file)
{
	if (*str2)
		snprintf(ans, KQ_PATH_MAX, "%s%s%s/%s", str1, str1[strlen(str1) - 1] == ':' ? "" : "/", str2, file);
	else
		snprintf(ans, KQ_PATH_MAX, "%s%s%s", str1, str1[strlen(str1) - 1] == ':' ? "" : "/", file);
	return ans;
}

const char *get_resource_file_path(const char *str1, const char *str2, const char *file)
{
	static char ans[KQ_PATH_MAX];

	return join(ans, str1, str2, file);
}

/* scripts/NAME.lob (the build ships the scripts as preprocessed Lua
 * source under that name), else scripts/NAME.lua */
const char *get_lua_file_path(const char *str1, const char *file)
{
	static char ans[KQ_PATH_MAX];
	char name[KQ_PATH_MAX];

	snprintf(name, sizeof name, "%s.lob", file);
	join(ans, str1, "scripts", name);
	if (exists(ans))
		return ans;
	snprintf(name, sizeof name, "%s.lua", file);
	join(ans, str1, "scripts", name);
	if (exists(ans))
		return ans;
	return NULL;
}

const char *kqres(enum eDirectories dir, const char *file)
{
	switch (dir) {
	case DATA_DIR:
		return get_resource_file_path(KQ_DATA, "data", file);
	case MUSIC_DIR:
		return get_resource_file_path(KQ_DATA, "music", file);
	case MAP_DIR:
		return get_resource_file_path(KQ_DATA, "maps", file);
	case SAVE_DIR:
	case SETTINGS_DIR:
		return get_resource_file_path(user_dir(), "", file);
	case SCRIPT_DIR:
		return get_lua_file_path(KQ_LIB, file);
	default:
		return NULL;
	}
}
