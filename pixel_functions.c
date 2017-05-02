#include <SDL2/SDL.h>

#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include "pixel_functions.h"


void write_pixel_value(SDL_Surface *surface, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
	// writes r, g, b to location x, y on surface. Might mix up RGB vs ARGB, BGR, etc.
	int depth = surface->format->BytesPerPixel;
	int offset = y * (surface->pitch) + x * depth;
	uint8_t* p = (uint8_t *) surface->pixels;
	// uint8_t* p = (uint8_t) surface->pixels + y * surface->pitch + x * depth;
	p[offset + 0] = b;
	p[offset + 1] = g;
	p[offset + 2] = r;
}

int pix_comp(Pixel* pix1, Pixel* pix2) {
	// Helper function for determining whether two pixels are the same composition or not
	return ((pix1->r == pix2->r) && (pix1->g == pix2->g) && (pix1->b == pix2->b));
}

void print_pixel(Pixel* p) {
	// Utility for printing the values of a Pixel*.
	printf("a:%u\tr:%u\tg:%u\tb:%u\n", p->a, p->r, p->g, p->b);
}


Pixel* get_pixel_value(SDL_Surface *surface, int x, int y) {
	// Queries a given surface at x and y, returning the values of the pixel stored there as a Pixel.
	int depth = surface->format->BytesPerPixel; // should usually be CANVAS_DEPTH
	uint8_t* p = (uint8_t *) surface->pixels;
	int offset = y * (surface->pitch) + x * depth; // bytes until the relevant ones.
	Pixel* res = malloc(sizeof(Pixel));
	res->a = (uint8_t) p[offset + 3]; // might be mixing up the ARGB channel order. TODO: check.
	res->b = (uint8_t) p[offset + 0];
	res->g = (uint8_t) p[offset + 1];
	res->r = (uint8_t) p[offset + 2];
	return res; // be sure to free() the pixel later.
}
