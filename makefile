make: pixel_functions.o tool_functions.o save_and_load.o sketch_a_sketch.c
	gcc -Wall pixel_functions.o tool_functions.o save_and_load.o sketch_a_sketch.c -o skask `sdl2-config --cflags --libs`

pixel_functions.o: pixel_functions.h pixel_functions.c
	gcc -c pixel_functions.c -o pixel_functions.o

tool_functions.o: tool_functions.h tool_functions.c pixel_functions.h pixel_functions.c
	gcc -c tool_functions.c -o tool_functions.o

save_and_load.o: save_and_load.h save_and_load.c
	gcc -c save_and_load.c -o save_and_load.o
