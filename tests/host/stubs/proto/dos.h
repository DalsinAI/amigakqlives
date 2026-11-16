/* Host test stand-in: the dos.library calls amiga/kq_amiga.c makes. */
typedef long BPTR;
typedef const char *CONST_STRPTR;
#define ACCESS_READ -2
BPTR Lock(CONST_STRPTR name, long mode);
BPTR CreateDir(CONST_STRPTR name);
void UnLock(BPTR lock);
