// glh2menu.h

#ifndef GLH2MENU_H
#define GLH2MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>
#include <SDL2/SDL.h>

#include <SDL2/SDL_ttf.h>
TTF_Font *font = NULL;

SDL_Texture *textTexture = NULL;
SDL_Texture *highlightTexture = NULL;

static int screenw = 480;
static int screenh = 320;

// VAR_INC and VAR_DEC courtesy of @kloptops
#define VAR_INC(variable, amount, total) \
    variable = (variable + amount) % total
#define VAR_DEC(variable, amount, total) \
    variable = (variable - amount + total) % total

// min & max clamp for pageup/pagedown courtesy of @kloptops
#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; })
#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
        _a < _b ? _a : _b; })

#define DISPLAY_HEIGHT 14
#define SCROLL_SIZE 8


#define CHAR_CLASS 5

const char *charClass[CHAR_CLASS] = {
    "Paladin",
    "Crusader",
    "Necromancer",
    "Assassin",
    "Demoness (PoP and specific mods only)"
};


#define SKILL_LEVEL 4

const char *skillLabels[SKILL_LEVEL] = {
    "Easy",
    "Normal",
    "Hard",
    "Very Hard"
};

int getClassLabel(int index);

#endif // GLH2MENU_H