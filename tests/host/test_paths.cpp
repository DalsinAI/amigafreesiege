// Host tests of the port's paths in common/utils.cpp, built as the Amiga
// build sets it (-D__amigaos__, FREESIEGE_DATA_DIR "PROGDIR:"): the data
// are read from the game's drawer and the settings go to its User drawer
// whatever HOME says (libnix's getenv reads ENV:, where HOME may exist).
// Also checks that every file the game's .cfg files name (anims.cfg: every
// frame, as engine/anim.cpp makes its names) is in the drawer the package
// copies.
// MIT, Copyright (c) 2026 AmiCradle Team.
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sys/stat.h>

#include "utils.h"

static int s_fails = 0;

static void check(bool ok, const std::string& what)
{
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", what.c_str());
    if (!ok) {
        ++s_fails;
    }
}

// The data files a .cfg names: the field after the last ':' on lines with one.
static void check_cfg(const std::string& root, const std::string& cfg)
{
    std::ifstream in(root + "/" + cfg);
    check(in.good(), cfg + " readable");
    std::string line;
    int n = 0;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        for (const char* ext : {".png", ".ogg", ".wav"}) {
            const size_t e = line.find(ext);
            if (e == std::string::npos) {
                continue;
            }
            size_t b = line.rfind(':', e);
            b = (b == std::string::npos) ? 0 : b + 1;
            const std::string rel = line.substr(b, e + 4 - b);
            struct stat st;
            if (stat((root + "/" + rel).c_str(), &st) != 0) {
                check(false, cfg + " names a missing file: " + rel);
            }
            ++n;
        }
    }
    check(n > 0, cfg + ": " + std::to_string(n) + " data files, all present");
}

// anims.cfg lines: name:frames:prefix:ext, frames prefix00.ext and on.
static void check_anims(const std::string& root)
{
    std::ifstream in(root + "/anims.cfg");
    check(in.good(), "anims.cfg readable");
    std::string line;
    int n = 0;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        const size_t a = line.find(':');
        const size_t b = line.find(':', a + 1);
        const size_t c = line.rfind(':');
        if (a == std::string::npos || b == std::string::npos || c <= b) {
            continue;
        }
        const int frames = std::atoi(line.substr(a + 1, b - a - 1).c_str());
        const std::string prefix = line.substr(b + 1, c - b - 1);
        const std::string ext = line.substr(c + 1);
        for (int f = 0; f < frames; ++f) {
            char num[12];
            std::snprintf(num, sizeof num, "%02d", f);
            const std::string rel = prefix + num + "." + ext;
            struct stat st;
            if (stat((root + "/" + rel).c_str(), &st) != 0) {
                check(false, "anims.cfg names a missing frame: " + rel);
            }
            ++n;
        }
    }
    check(n > 0, "anims.cfg: " + std::to_string(n) + " frames, all present");
}

int main(int argc, char** argv)
{
    setenv("HOME", "/home/somebody", 1);
    check(config_file() == "PROGDIR:User/freesiege.cfg", "config_file with HOME set: " + config_file());
    unsetenv("HOME");
    check(config_file() == "PROGDIR:User/freesiege.cfg", "config_file without HOME: " + config_file());
    check(get_base_dir() == "PROGDIR:", "get_base_dir: " + get_base_dir());
    check(get_base_dir() + "combi.cfg" == "PROGDIR:combi.cfg", "data path joins without a '/'");

    if (argc > 1) {
        for (const char* cfg : {"sprites.cfg", "musics.cfg"}) {
            check_cfg(argv[1], cfg);
        }
        check_anims(argv[1]);
    }

    std::printf("%s (%d failed)\n", s_fails ? "FAIL" : "PASS", s_fails);
    return s_fails ? 1 : 0;
}
