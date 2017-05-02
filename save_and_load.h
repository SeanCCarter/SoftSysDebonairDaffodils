#include <SDL2/SDL.h>

#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

void save_as(SDL_Surface *surface, char* filename);
void load_as(SDL_Surface *surface, char* filename);
