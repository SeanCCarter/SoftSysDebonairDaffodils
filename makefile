make: pixmap_test.c
	gcc -Wall -g pixmap_test.c -o pmt `pkg-config --cflags gtk+-2.0` \
`pkg-config --libs gtk+-2.0`