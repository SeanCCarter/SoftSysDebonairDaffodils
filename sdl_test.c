/*
	Package Location: sudo apt-get install libsdl2-dev
	
    gcc -o myprogram myprogram.c `sdl2-config --cflags --libs` 

    or

    gcc -o myprogram myprogram.c -lSDL2


    (here specifically: 
    gcc sdl_test.c -o sdlt `sdl2-config --cflags --libs`
		todo: makefile that jazz!)

*/


#include <SDL2/SDL.h>

#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>


int CANVAS_XWIDTH = 360;
int CANVAS_YWIDTH = 240;
int CANVAS_DEPTH = 4;
int CANVAS_BYTES; // should get set to X*Y*depth
int STRIDE; // for ease of calculation later; ideally X*depth?..

char C_WHITE = 0xFF;
char SAVE_AS[80] = "saved.image"; // max filename length of 80 for now.


char* HELP = (	"Paint program in C.\n"
					"\t-x: x-width\n"
					"\t-y: y-width\n"
					"\t-f: filename to load/save as\n"
					"\t-c: clear image\n"
					"\t-e: etch-a-sketch mode (arrow keys to draw)\n"
					"\t-h: this help"
					);

// code cleanup todo: strings file.

char* INSTRUCTIONS = (	"Controls:\n"
							"\tr: Red; k: blacK; g: Green; b: Blue; w: White\n"
							"\t[: size down; ]: size up; s: Save; l: Load\n"
							"\tmouse: left click to draw, right click to fill."
						);

char* UNREC_FLAG = "Unrecognized flag.";

char* UNREC_KEY = "Unrecognized key or control.";

char* STARTUP_ERROR = "SDL couldn't init :(.  Error";

char* WINDOW_OPEN_ERROR = "Couldn't open window.";

int min_of(int a, int b) {
	// utility for getting the lower of two options.
	if (a < b) {
		return a;
	}
	return b;
}

int max_of(int a, int b) {
	// utility for getting the greater of two options.
	if (a > b) {
		return a;
	}
	return b;
}

typedef enum {false, true} bool; // utility because I like true/false.

typedef struct pixel { // tuple-esque way of returning pixel data.
	uint8_t a;
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Pixel;

typedef struct tool {
	int radius;
	uint8_t r; // more efficient to use an int8, space-wise.
	uint8_t g; // or perhaps a color enum?
	uint8_t b;
} Tool; // or could just do global variables, but anyways.

Tool* make_tool() {
	// initializes a tool of radius three.
	Tool* t = malloc(sizeof(Tool));
	t->radius = 3;
	t->r = 0;
	t->g = 0;
	t->b = 0;
	return t;
}

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


void parse_start_args(int argc, char* argv[], bool* to_clear, bool* etch_a_sketch_mode) {
	int option = 0;
	while ((option = getopt(argc, argv,"x:y:f:hce")) != -1) {
		switch (option) {
			case 'x':
				CANVAS_XWIDTH = atoi(optarg);
				break;
			case 'y':
				CANVAS_YWIDTH = atoi(optarg);
				break;
			case 'f':
				strcpy(SAVE_AS, optarg);
				break;
			case 'h':
				printf("%s\n", HELP);
				printf("%s\n", INSTRUCTIONS);
				exit(0);
				break;
			case 'c':
				*to_clear = true;
				break;
			case 'e':
				*etch_a_sketch_mode = true;
				break;
			default:

				printf("%s\n", UNREC_FLAG);
				printf("%s\n", HELP);
				exit(0);
				break;
		}
	}
	CANVAS_BYTES = CANVAS_XWIDTH*CANVAS_YWIDTH*CANVAS_DEPTH; // total size of canvas
	STRIDE = CANVAS_XWIDTH*CANVAS_DEPTH;
}


void process_arrow_key(int* cx, int* cy, SDL_Event e) {
	// Processes arrow keys, changing the values at cx and cy accordingly.
	switch (e.key.keysym.sym) {
		case SDLK_UP:
			if ((*cy) > 0) {
				(*cy)--;
			}
			break;
		case SDLK_DOWN:
			if ((*cy) < (CANVAS_YWIDTH - 1)) {
				(*cy)++;
			}
			break;
		case SDLK_LEFT:
			if ((*cx) > 0) {
				(*cx)--;
			}
			break;
		case SDLK_RIGHT:
			if ((*cx) < (CANVAS_XWIDTH - 1)) {
				(*cx)++;
			}
			break;
		default:
			printf("%s\n", UNREC_KEY);
			break;
	}
}


void process_color_key(Tool* user_tool, SDL_Event e) {
	switch (e.key.keysym.sym) {
		case SDLK_r:
			user_tool->r = 255;
			user_tool->g = 0;
			user_tool->b = 0;
			break;
		case SDLK_k:
			user_tool->r = 0;
			user_tool->g = 0;
			user_tool->b = 0;
			break;
		case SDLK_g:
			user_tool->r = 0;
			user_tool->g = 255;
			user_tool->b = 0;
			break;
		case SDLK_b:
			user_tool->r = 0;
			user_tool->g = 0;
			user_tool->b = 255;
			break;
		case SDLK_w:
			user_tool->r = 255;
			user_tool->g = 255;
			user_tool->b = 255;
			break;
		default:
			printf("%s\n", UNREC_KEY);
			break;
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

int main(int argc, char* argv[]) {
	bool to_clear = false;
	bool etch_mode = false;

	parse_start_args(argc, argv, &to_clear, &etch_mode);

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;  // not sure if NULL declaration is necessary.
									// also might not actually ever need a renderer.
	SDL_Surface* canvas = NULL;
	SDL_Event event;

	Tool* user_tool = make_tool();
	Tool* fill_tool = make_tool();
	fill_tool->radius = 1;

	int cursor_x = 0;
	int cursor_y = 0;
	int mouse_x;
	int mouse_y;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "%s:%s \n", STARTUP_ERROR, SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow( "window", 	SDL_WINDOWPOS_UNDEFINED, 	
												SDL_WINDOWPOS_UNDEFINED, 
												CANVAS_XWIDTH, 				
												CANVAS_YWIDTH, 
												SDL_WINDOW_SHOWN );


	if (window == NULL) {
		fprintf(stderr, "%s:%s \n", WINDOW_OPEN_ERROR, SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	canvas = SDL_GetWindowSurface(window);

	SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 0xFF, 0xFF, 0xFF)); // is 0xFF the proper syntax here?
	
	// checks if SAVE_AS exists; if it does, load it automatically.
	if (!to_clear) {
		FILE *sa = fopen(SAVE_AS,"r");
		if (sa != NULL) {
			fclose(sa);
			printf("Loading %s...\n", SAVE_AS);
			load_as(canvas, SAVE_AS);
		}
	}
	// is this a memory leak? should we free() sa?

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
					case SDLK_DOWN:
					case SDLK_LEFT:
					case SDLK_RIGHT:
						if (etch_mode) {
							process_arrow_key(&cursor_x, &cursor_y, event);
						}
						else {
							printf("Not in arrow-key-friendly mode, try '-e'!\n");
						}
						break;
					case SDLK_r:
					case SDLK_k:
					case SDLK_g:
					case SDLK_b:
					case SDLK_w:
						process_color_key(user_tool, event);
						break;
					case SDLK_LEFTBRACKET:
						user_tool->radius = max_of(user_tool->radius - 1, 0);
						break;
					case SDLK_RIGHTBRACKET:
						user_tool->radius = user_tool->radius + 1; // does ++ syntax work here?
						break;
					case SDLK_s: // save
						printf("Saving...\n");
						save_as(canvas, SAVE_AS);
						printf("Saved.\n");
						break;
					case SDLK_l:
						printf("Loading...\n");
						load_as(canvas, SAVE_AS);
						printf("Loaded.\n");
						break;
					


					default:
						printf("%s\n", UNREC_KEY);
						break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				// switch (event.type) {
				switch (event.button.button) {
					case SDL_BUTTON_LEFT:
					// case SDL_MouseButtonEvent:
					// case SDL_BUTTON(SDL_GetMouseState(NULL,NULL)) == SDL_BUTTON_LEFT:
						// printf("clicked.\n");
						SDL_GetMouseState(&mouse_x, &mouse_y);
						cursor_x = mouse_x;
						cursor_y = mouse_y;
						break;
					case SDL_BUTTON_RIGHT:
						SDL_GetMouseState(&mouse_x, &mouse_y);
						// printf("filling.\n");
						Pixel* pix = get_pixel_value(canvas, mouse_x,mouse_y);
						floodFill(mouse_x, mouse_y, pix, user_tool, canvas);
						break;


					default:
						printf("%s\n", UNREC_KEY);
						break;
				}
			}

			else if (event.type == SDL_MOUSEMOTION && event.motion.state == SDL_PRESSED) {
				switch (event.type) {
					case SDL_MOUSEMOTION:
						// printf("dragging.\n");
						SDL_GetMouseState(&mouse_x, &mouse_y);
						cursor_x = mouse_x;
						cursor_y = mouse_y;
						break;


					default:
						printf("%s\n", UNREC_KEY);
						break;
				}
			}
		}
	// if (etch_mode) {
	// 	draw(canvas, cursor_x, cursor_y, user_tool);
	// }
	draw(canvas, cursor_x, cursor_y, user_tool);

	
	SDL_UpdateWindowSurface(window);

	}

	SDL_FreeSurface(canvas);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}