#include <SDL2/SDL.h>

#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include "pixel_functions.h"

int CANVAS_XWIDTH;
int CANVAS_YWIDTH;
int CANVAS_DEPTH;
int CANVAS_BYTES; // should get set to X*Y*depth
int STRIDE; // for ease of calculation later; ideally X*depth?..

typedef struct tool {
	int radius;
	uint8_t r; // more efficient to use an int8, space-wise.
	uint8_t g; // or perhaps a color enum?
	uint8_t b;
} Tool; // or could just do global variables, but anyways.


Tool* make_tool();
int min_of(int a, int b);
int max_of(int a, int b);
void draw(SDL_Surface *surface, int x, int y, Tool* t);
void draw2(SDL_Surface *surface, int x, int y, Tool* t);
void floodFill(int x,int y, Pixel* orig, Tool* fill, SDL_Surface *surface);
