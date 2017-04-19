/*
    gcc -o myprogram myprogram.c `sdl2-config --cflags --libs` 

    or

    gcc -o myprogram myprogram.c -lSDL2

*/


#include <SDL2/SDL.h>

#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>


int CANVAS_XWIDTH = 400;
int CANVAS_YWIDTH = 400;
int CANVAS_DEPTH = 4;
int CANVAS_BYTES; // should get set to X*Y*depth
int STRIDE; // for ease of calculation later; ideally X*depth?..

char C_WHITE = 0xFF;


typedef struct pixel {
	uint8_t a;
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Pixel;


void print_pixel(Pixel* p) {
	printf("a:%u\tr:%u\tg:%u\tb:%u\n", p->a, p->r, p->g, p->b);
}


typedef enum {false, true} bool;


Pixel* get_pixel_value(SDL_Surface *surface, int x, int y) {
	int depth = surface->format->BytesPerPixel; // should be CANVAS_DEPTH but better safe than sorry.
	uint8_t* p = (uint8_t) surface->pixels + y*surface->pitch + x*depth;
	Pixel* res = malloc(sizeof(Pixel));
	res->a = (uint8_t) p[3];
	res->r = (uint8_t) p[0];
	res->g = (uint8_t) p[1];
	res->b = (uint8_t) p[2];
	return res; // be sure to free() later.
}

void write_pixel_value(SDL_Surface *surface, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
	int depth = surface->format->BytesPerPixel;
	int offset = y * (surface->pitch) + x * depth;
	uint8_t* p = (uint8_t *) surface->pixels;
	// uint8_t* p = (uint8_t) surface->pixels + y * surface->pitch + x * depth;
	p[offset] = r;
	p[offset+1] = g;
	p[offset+2] = b;
}


int main(int argc, char* argv[]) {

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL; // not sure if NULL declaration is necessary.
	SDL_Surface* canvas = NULL;
	SDL_Event event; // or perhaps a different event handler?

	unsigned char r_v = 0xFF;
	unsigned char g_v = 0xFF;
	unsigned char b_v = 0xFF;

	int cursor_x = 0;
	int cursor_y = 0;


	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL couldn't init :(.  Error: %s\n", SDL_GetError());
		exit(1);
	}

	// SDL_CreateWindowAndRenderer(CANVAS_XWIDTH, CANVAS_YWIDTH,&window, &renderer);

	window = SDL_CreateWindow( "test window", 	SDL_WINDOWPOS_UNDEFINED, 	
												SDL_WINDOWPOS_UNDEFINED, 
												CANVAS_XWIDTH, 				
												CANVAS_YWIDTH, 
												SDL_WINDOW_SHOWN );


	if (window == NULL) {
		fprintf(stderr, "Couldn't make window. Error: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	canvas = SDL_GetWindowSurface(window);

	SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, r_v, g_v, b_v)); // is 0xFF the proper syntax here?
	
	SDL_UpdateWindowSurface(window);

	bool run_flag = true;

	while (run_flag) {

		while (SDL_PollEvent(&event) != 0) {

			if (event.type == SDL_QUIT) { // X in window corner.
				fprintf(stdout, "Quitting...\n");
				run_flag = false;
			}

			else if (event.type == SDL_KEYDOWN) {

				switch (event.key.keysym.sym) {
					case SDLK_UP:
						if (cursor_y > 0) {
							cursor_y--;
						}
						break;
					case SDLK_DOWN:
						if (cursor_y < (CANVAS_YWIDTH - 1)) {
							cursor_y++;
						}
						break;
					case SDLK_LEFT:
						if (cursor_x > 0) {
							cursor_x--;
						}
						break;
					case SDLK_RIGHT:
						if (cursor_x < (CANVAS_XWIDTH - 1)) {
							cursor_x++;
						}
						break;
					case SDLK_r:
						// SDL_SetRenderDrawColor(renderer, 255,0,0,255);
						break;
					case SDLK_k:
						// SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
						break;
					case SDLK_g:
						// SDL_SetRenderDrawColor(renderer, 0,255,0,255);
						break;
					case SDLK_b:
						// SDL_SetRenderDrawColor(renderer, 0,0,255,255);
						break;
					case SDLK_w:
						// SDL_SetRenderDrawColor(renderer, 255,255,255,255);
						break;
					default:
						fprintf(stdout, "Unrecognized key.\n");
						break;
				}
				// r_v = (r_v - 10);
				// g_v = (g_v - 10); // does this work with unsigned chars?
				// b_v = (b_v - 10);
			}
			// else if (event.type == )
		}
	// SDL_RenderDrawPoint(renderer, cursor_x, cursor_y);	
	// SDL_RenderPresent(renderer);
	puts("about to write pixel value\n");
	

	write_pixel_value(canvas, cursor_x, cursor_y, (uint8_t) 77, (uint8_t) 77, (uint8_t) 77);
	
	puts("wrote pixel value\n");

	SDL_UpdateWindowSurface(window);

	}


	SDL_FreeSurface(canvas);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;

	// SDL_Delay(s2000); // 2000 ms delay - like waiting for 2 seconds. 
	 				 // this is to see something for a bit then clean up on the way out.

}


// SDL_Window *window;
// SDL_Renderer *renderer;
// SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);

// //Probably on a loop
//   SDL_RenderDrawPoint(renderer, 400, 300); //Renders on middle of screen.
//   SDL_RenderPresent(renderer);


//  // SDL_RenderReadPixels(SDL_RENDERER* renderer, SDL_Rect* rect, Uint32 format, void* pixels, int pitch)