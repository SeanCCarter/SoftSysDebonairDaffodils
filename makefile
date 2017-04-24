make: sdl_test.c
	gcc sdl_test.c -o sdlt `sdl2-config --cflags --libs`