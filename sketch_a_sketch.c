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
#include "pixel_functions.h"
#include "tool_functions.h"
#include "save_and_load.h"

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

int main(int argc, char* argv[]) {
	bool to_clear = false;
	bool etch_mode = false;

	parse_start_args(argc, argv, &to_clear, &etch_mode);

	SDL_Window* window = NULL;
	SDL_Surface* canvas = NULL;
	SDL_Event event;

	Tool* user_tool = make_tool();

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
						SDL_GetMouseState(&mouse_x, &mouse_y);
						cursor_x = mouse_x;
						cursor_y = mouse_y;
						break;
					case SDL_BUTTON_RIGHT:
						SDL_GetMouseState(&mouse_x, &mouse_y);
						Pixel* pix = get_pixel_value(canvas, mouse_x,mouse_y);
						floodFill(mouse_x, mouse_y, pix, user_tool, canvas);
						free(pix);
						break;


					default:
						printf("%s\n", UNREC_KEY);
						break;
				}
			}

			else if (event.type == SDL_MOUSEMOTION && event.motion.state == SDL_PRESSED) {
				switch (event.type) {
					case SDL_MOUSEMOTION:
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

	draw(canvas, cursor_x, cursor_y, user_tool);


	SDL_UpdateWindowSurface(window);
	}

	SDL_FreeSurface(canvas);
	SDL_DestroyWindow(window);
	free(user_tool);
	SDL_Quit();

	return 0;
}
