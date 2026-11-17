/* Host test stand-in: the exec.library call amiga/music_gfopenmusic.c makes. */
#ifndef STUB_PROTO_EXEC_H
#define STUB_PROTO_EXEC_H
typedef const char *CONST_STRPTR;
struct Library { int dummy; };
struct Library *OpenLibrary(CONST_STRPTR name, unsigned long version);
#endif
