// FreeSiege for GFOS: the program's version string, its stack and its
// User directory (settings), made before main() when it is missing.
// MIT, Copyright (c) 2026 AmiCradle Team.
#include <sys/stat.h>

// libnix gives the program this much stack when it starts.
extern "C" unsigned long __stack = 256 * 1024;

extern "C" const char fs_amiga_version[] __attribute__((used)) =
    "$VER: FreeSiege 1.0 (16.11.2026)";

__attribute__((constructor)) static void fs_amiga_make_user_dir()
{
    mkdir("PROGDIR:User", 0755);
}
