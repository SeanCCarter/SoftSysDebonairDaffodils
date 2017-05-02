#include <SDL2/SDL.h>

#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>


void save_as(SDL_Surface *surface, char* filename) {
	// Saves the given surface to the target filename.
	int w = surface->w;
	int h = surface->h;
	int depth = surface->format->BytesPerPixel;

	int pitch = surface->pitch;

	uint8_t* p = (uint8_t *) surface->pixels;

	FILE* fd = fopen(filename, "w");

	fwrite(p, sizeof(uint8_t), w*h*depth, fd);
	fclose(fd);
}

void load_as(SDL_Surface *surface, char* filename) {
	// Expects the file to already exist.
	// Since size is user-defined, this can't handle size changes.
	// Might be worth changing the save/load encoding to take size info too.
	int w = surface->w;
	int h = surface->h;
	int depth = surface->format->BytesPerPixel;

	int pitch = surface->pitch;

	uint8_t* p = (uint8_t *) surface->pixels;

	FILE* fd = fopen(filename, "r");

	fread(p, sizeof(uint8_t), w*h*depth, fd);
	fclose(fd);
}
