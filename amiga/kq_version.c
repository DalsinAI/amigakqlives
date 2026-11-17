/* Version's string for KQ on AmigaOS.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
const char kq_amiga_version[] __attribute__((used)) =
	"$VER: KQ 0.99.3-gfos1 (16.11.2026) KQ Lives, GFOS port by the AmiCradle Team";
/* Lua's C calls nest deep in KQ's scripts: libnix gives the game this
 * stack whatever the shell's */
unsigned long __stack = 262144;
