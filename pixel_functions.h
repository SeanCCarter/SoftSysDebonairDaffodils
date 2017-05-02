#include <SDL2/SDL.h>

#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#ifndef PIXELFUNCTIONS
#define PIXELFUNCTIONS
typedef struct pixel { // tuple-esque way of returning pixel data.
	uint8_t a;
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Pixel;
#endif


void write_pixel_value(SDL_Surface *surface, int x, int y, uint8_t r, uint8_t g, uint8_t b);
int pix_comp(Pixel* pix1, Pixel* pix2);
void print_pixel(Pixel* p);
Pixel* get_pixel_value(SDL_Surface *surface, int x, int y);
