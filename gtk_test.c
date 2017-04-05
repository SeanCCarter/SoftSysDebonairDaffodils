/*
	basically the hello world thing for gtk2. 

	setup: 
	$ sudo apt-get install gtk2.0
	$ sudo apt-get install build-essential libgtk2.0-dev

	compile:
	$ gcc -Wall -g gtk_test.c -o g_test `pkg-config --cflags gtk+-2.0` \
	$ `pkg-config --libs gtk+-2.0`

	(those are ` forward ticks, not ' single quotes.)

	relevant libraries to link in: -lgtk (widget library), -lgdk (xlib wrapper),
	-lgdk_pixbuf (image manip library), -lX11 (used by GDK), -lglib (GLib stuff),
	-lm (math). other info, libs here: https://developer.gnome.org/gtk-tutorial/2.90/x111.html


	todo: makefile that compile command!
*/



#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>




// static void bmp_blit_callback( GtkWidget *widget, gpointer data) {}

static void hello(GtkWidget *widget, gpointer data) {
	fprintf(stdout, "wheeeee\n");
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	fprintf(stdout, "delete button (X) clicked.\n");
	return TRUE; // TRUE: don't actually leave, e.g. for a "Are you sure/did you save?" popup
	// FALSE: *actually* leave. these are gbooleans, btw.
}

static void destroy(GtkWidget *widget, gpointer data) {
	gtk_main_quit();
}

int main(int argc, char* argv[]) {
	GtkWidget* window;
	GtkWidget* button;

	gtk_init(&argc, &argv); // pretty sure this changes argv and argc to remove gtk-recognized params.
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // funciton returns pointer to window object.
	g_signal_connect(window, "delete-event", G_CALLBACK (delete_event), NULL); // attaches function as G_CALLBACK to delete_event to window's x button ("delete-event").
	g_signal_connect(window, "destroy", G_CALLBACK (destroy), NULL); // attaches G_CALLBACK of destroy to "destroy" event.

	gtk_container_set_border_width( GTK_CONTAINER (window), 10); // pixels I think
	button = gtk_button_new_with_label("hello!");
	g_signal_connect(button, "clicked", G_CALLBACK (hello), NULL); // G_CALLBACK passes the first widget pointer as the widget arg, and NULL as the argument. Can change NULL ofc.

	g_signal_connect_swapped(button, "clicked", G_CALLBACK (gtk_widget_destroy), window); // no idea.
	gtk_container_add (GTK_CONTAINER (window), button);
	gtk_widget_show(button);
	gtk_widget_show(window);
	gtk_main(); // control ends; waits for events like keypresses or mouse events.
	return 0;
	// fprintf(stdout, "Here.\n");
	// return 0;




}