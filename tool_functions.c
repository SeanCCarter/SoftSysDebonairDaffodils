#include <SDL2/SDL.h>

#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include "pixel_functions.h"
#include "tool_functions.h"

Tool* make_tool() {
	// initializes a tool of radius three.
	Tool* t = malloc(sizeof(Tool));
	t->radius = 3;
	t->r = 0;
	t->g = 0;
	t->b = 0;
	return t;
}

void draw(SDL_Surface *surface, int x, int y, Tool* t) {
	// draws a... "circle" (actually a square), within the boundaries.
	uint8_t r = t->r;
	uint8_t g = t->g;
	uint8_t b = t->b;

	int i, j;

	int startx = max_of(x - t->radius, 0);
	int endx = min_of(x + t->radius, CANVAS_XWIDTH-1);
	int starty = max_of(y - t->radius, 0);
	int endy = min_of(y + t->radius, CANVAS_YWIDTH-1);

	for (i = startx; i <= endx; i++) {
		for (j = starty; j <= endy; j++) { // <= or <  -- todo: check.
			write_pixel_value(surface, i, j, r, g, b);
		}
	}
}

void draw2(SDL_Surface *surface, int x, int y, Tool* t) {
	// draws a... "circle" (actually a square), within the boundaries.
	// note: why is this a loop? as opposed to just one call?
	uint8_t r = t->r;
	uint8_t g = t->g;
	uint8_t b = t->b;

	int i, j;

	int startx = max_of(x, 0);
	int endx = min_of(x, CANVAS_XWIDTH-1);
	int starty = max_of(y, 0);
	int endy = min_of(y, CANVAS_YWIDTH-1);

	for (i = startx; i <= endx; i++) {
		for (j = starty; j <= endy; j++) { // <= or <  -- todo: check.
			write_pixel_value(surface, i, j, r, g, b);
		}
	}
}

//Experimental floodfill
void floodFill(int x,int y, Pixel* orig, Tool* fill, SDL_Surface *surface) {
	/*
	x,y - coordinates of the pixel
	orig - original color of pixel
	fill - new color for pixel
	surface - canvas user is working on
	*/
	// printf("recursing.\n");
	Pixel* pix = get_pixel_value(surface,x,y);
	Pixel* tool_color = malloc(sizeof(Pixel));
	tool_color->r = fill->r;
	tool_color->g = fill->g;
	tool_color->b = fill->b;

    if(pix_comp(orig, pix) == 1 && pix_comp(pix, tool_color) == 0) // make a function that compares
    {
        // putpixel(x,y,fill);
        // printf("writing.\n");
        write_pixel_value(surface,x,y,orig->r,orig->g,orig->b);
        if (x+1 < (CANVAS_XWIDTH)) {
        	draw2(surface, x, y, fill);
        	floodFill(x+1,y,orig,fill,surface);
        }
        if (x+1 < (CANVAS_YWIDTH)) {
        	draw2(surface, x, y, fill);
        	floodFill(x,y+1,orig,fill,surface);
        }
        if (x-1 > -1) {
        	draw2(surface, x, y, fill);
        	floodFill(x-1,y,orig,fill,surface);
    	}
        if (y-1 > -1) {
        	draw2(surface, x, y, fill);
        	floodFill(x,y-1,orig,fill,surface);
    	}
    }
    free(pix);
    free(tool_color);
    return;

     // either make a one pixel tool specifically for floodFill or make the step size depend on the tool being used

}
