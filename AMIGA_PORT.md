FreeSiege for GFOS
==================

A port of FreeSiege, Pierre Gueth and Joel Schaerer's block war game for
two players, from https://gitlab.com/LibreGames/freesiege at commit
7205754874bdadde73191e396ad064ec2382bcb7, for GFOS: AmigaOS 3.2 with
GFOpenUp, on a 68040 or 68060 with an FPU, on GFOpenUp's SDL 2.

Swap blocks on your board to build patterns, and each pattern sends a
soldier, knight, ram, dragon or druid at the other player's castle. Two
players share the keyboard; there is also a survival mode for one.

Status: see the pull request (stove build and lab).


What you need
-------------

- GFOS: AmigaOS 3.2 with GFOpenUp, which installs LIBS:GFOpenGPU/
  SDL2.module, SDL2_image.module, SDL2_mixer.module and SDL2_ttf.module.
- A 68040 or 68060 with an FPU (not an LC040 or EC040), an RTG screen of
  800 x 600 or more (or the AmiCradle lab), and about 32 MB of free
  memory.
- About 20 MB on the disk.


How to play
-----------

Run Install-FreeSiege (or copy the FreeSiege drawer where you want it),
then double-click FreeSiege, or start it from a Shell. The keys are in the
drawer's Controls file. Your settings are kept in the drawer's User drawer
(User/freesiege.cfg); Install-FreeSiege keeps it when it updates the game.


Building
--------

    make -f amiga/Makefile.amigaos -j4 STOVE=/path/to/os32-gcc16-open6

or, in Cradle, stove_build with the os32-gcc16-open6 stove and target
package (package/build-amiga.sh), which also packs
FreeSiege-VERSION-68040.lha.

    tests/host/run.sh

runs the host tests of the port's paths (no SDL needed).


What was changed
----------------

- The data are read from the game's drawer: FREESIEGE_DATA_DIR is
  "PROGDIR:", as upstream's packaged build sets it to its share
  directory. Network play is left out, as in upstream's default build.
- common/utils.cpp: on AmigaOS the settings file is
  PROGDIR:User/freesiege.cfg, whatever HOME says.
  tests/host/test_paths.cpp checks it, and that every picture, frame and
  tune the game's .cfg files name is in the drawer.
- amiga/amiga_start.cpp: makes User/ when it is missing, a 256 KB stack
  and the $VER string.

Designed, tested and delivered by The AmiCradle Team.
