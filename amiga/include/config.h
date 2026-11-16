/* KQ's config.h for AmigaOS (what ./configure writes elsewhere).
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#define PACKAGE "kq"
#define PACKAGE_NAME "KQ"
#define PACKAGE_TARNAME "kq"
#define PACKAGE_VERSION "0.99.3"
#define PACKAGE_STRING "KQ 0.99.3"
#define PACKAGE_BUGREPORT ""
#define VERSION "0.99.3"
/* everything sits in the game's own drawer */
#define KQ_BIN "PROGDIR:KQ"
#define KQ_DATA "PROGDIR:"
#define KQ_LIB "PROGDIR:"
#define KQ_LOCALE "PROGDIR:locale"
/* timing.c waits through select(), which amiga/include/sys/select.h turns
 * into Allegro's rest() */
#define HAVE_SYS_SELECT_H 1
#define ENABLE_NLS 0
/* KQ 0.99.3 calls lua_load with Lua 5.2's fifth argument (the mode, always
 * NULL); lua51.library is Lua 5.1, whose lua_load takes four. lua.h
 * declares (lua_load) in brackets, so this macro only changes KQ's calls. */
#define lua_load(L, reader, data, name, mode) lua_load(L, reader, data, name)
