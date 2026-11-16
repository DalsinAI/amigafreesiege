#!/bin/sh
# Host tests of the Amiga port's paths (no SDL needed): common/utils.cpp
# built with the Amiga build's defines, and the .cfg files' data checked.
# MIT, Copyright (c) 2026 AmiCradle Team.
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$HERE/../.." && pwd)
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT
CXX=${CXX:-g++}
$CXX -std=gnu++11 -Wall -D__amigaos__ '-DFREESIEGE_DATA_DIR="PROGDIR:"' -I"$HERE/stubs" -I"$ROOT/common" \
    -o "$WORK/test_paths" "$HERE/test_paths.cpp" "$ROOT/common/utils.cpp"
"$WORK/test_paths" "$ROOT"
echo "ALL PASS"
