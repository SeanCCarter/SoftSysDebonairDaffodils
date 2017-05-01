/*
 trying out displaying a pixmap.


 	compile:
	$ gcc -Wall -g pixmap_test.c -o pmt `pkg-config --cflags gtk+-2.0` \
	$ `pkg-config --libs gtk+-2.0`

gcc -Wall -g pixmap_test.c -o pmt `pkg-config --cflags gtk+-2.0` \
`pkg-config --libs gtk+-2.0`

	todo: makefile that command.


	pipe chars into stdin from something else as an API. read from stdin to 
	do tool changes, etc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

int CANVAS_XWIDTH = 400;
int CANVAS_YWIDTH = 400;


static GdkPixmap* pixmap = NULL; // pixel map for target area.

typedef struct tool {
	int radius;
	int r; // more efficient to use an int8, space-wise.
	int g; // or perhaps a color enum?
	int b;
} Tool; // or could just do global variables, but anyways.


// returns a plain tool, initialized to black and radius 5 (drawing five pixels away).
Tool* make_tool() {
	Tool* t = (Tool*) malloc(sizeof(Tool));
	t->radius = 3;
	t->r = 0;
	t->g = 0;
	t->b = 0;
	return t;
}

// sets the rgb values for t to r, g, b.
void set_color(Tool* t, int r, int g, int b) {
	t->r = r;
	t->g = g;
	t->b = b;
}



// Configures the "widget" (basic class-y thing for Gtk) as a a pixmap.
static gboolean configure_event( GtkWidget* widget, GdkEventConfigure* event) {
	if (pixmap) {
		g_object_unref(pixmap); // detaches pixmap if it already exists, I think?
	}
	pixmap = gdk_pixmap_new (	widget->window, 
								widget->allocation.width,
								widget->allocation.height,
								-1); 
									// not sure about what -1 depth means.
									// (number of bits per pixel)
									// probably want enough to store colors.
									// -1: use the first arg's depth...?
	gdk_draw_rectangle(	pixmap,
						widget->style->white_gc, // white rectangle over its size?
						TRUE,
						0, 0, // start coords I believe
						widget->allocation.width,
						widget->allocation.height); 
						// TODO: look into cairo_rectangle, cairo_fill,
						// cairo_stroke, cairo_set_source_pixmap, etc.
						// gdk drawing primitives are sliiightly deprecated.
	return TRUE;		
}



// Draws the screen from the pixmap.
static gboolean blit_pixmap( GtkWidget* widget, GdkEventExpose *event) {
	gdk_draw_drawable(	widget->window,
						widget->style->fg_gc[gtk_widget_get_state (widget)], // no idea. foreground color...?
						pixmap, // the thing to draw!
						event->area.x, event->area.y,
						event->area.x, event->area.y, // just copypasted. yes, twice. not sure why - todo: understand.
						event->area.width, event->area.height);
	return FALSE;
}

// Draw a square from a given Tool onto the Widget at x, y.
static void draw_tool(GtkWidget* widget, gdouble x, gdouble y, Tool* t) {
	GdkRectangle update;
	update.x = x - (t->radius);
	update.y = y - (t->radius);
	update.width = 2*(t->radius);
	update.height = 2*(t->radius);

	gdk_draw_rectangle(	pixmap,
						widget->style->black_gc,
						TRUE, // FALSE to outline, TRUE to fill.
						update.x, update.y,
						update.width, update.height);
	gtk_widget_queue_draw_area(	widget,
								update.x, update.y,
								update.width, update.height);
}

// handle mousedown event ("button")
static gboolean mousedown_event(GtkWidget* widget, GdkEventButton* event, gpointer tool_data){
	Tool* t = tool_data;
	if (event->button == 1 && pixmap != NULL) {
		draw_tool(widget, event->x, event->y, t);
	}
	return TRUE;
}

// handles moving while the mouse is down.
static gboolean move_event(GtkWidget* widget, GdkEventMotion* event, gpointer tool_data){
	Tool* t = tool_data;
	int x, y;
	GdkModifierType state;
	if (event->is_hint) {
		gdk_window_get_pointer (event->window, &x, &y, &state); // makes sure it's actually the mousedown part.
	}
	else {
		state = event->state;
	}
	if (state & GDK_BUTTON1_MASK && pixmap != NULL) { // if it's the right button - mouse here
		draw_tool(widget, event->x, event->y, t);
	}
	return TRUE;
}


void quit() {
	fprintf(stdout, "Quitting...\n");
	exit(0);
}

int main(int argc, char *argv[]) {

	Tool* current_tool = make_tool();

	GtkWidget *window;
	GtkWidget *canvas;
	GtkWidget *vbox;

	// GtkWidget *clear_button;
	// GtkWidget *save_button;
	// GtkWidget *quit_button;

	gtk_init(&argc, &argv); // pulls out gtk arguments; adjusts argv and argc accordingly.

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_name(window, "drawing test whee");

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER (window), vbox);
	gtk_widget_show(vbox);

	g_signal_connect(window, "destroy", G_CALLBACK (quit), NULL); // connects "destroy" (x-button in corner) to quit function as a g_callback.

	canvas = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET (canvas), CANVAS_XWIDTH, CANVAS_YWIDTH);
	gtk_box_pack_start(GTK_BOX (vbox), canvas, TRUE, TRUE, 0);

	gtk_widget_show(canvas);

	// connecting everything to the associated functions!
	g_signal_connect(canvas, "expose_event", 
		G_CALLBACK (blit_pixmap), NULL);
	g_signal_connect(canvas, "configure_event", 
		G_CALLBACK (configure_event), NULL);
	g_signal_connect(canvas, "button_press_event", 
		G_CALLBACK (mousedown_event), current_tool);
	g_signal_connect(canvas, "motion_notify_event", 
		G_CALLBACK (move_event), current_tool);

	// gtk_widget_set_extension_events(canvas, GDK_EXTENSION_EVENTS_CURSOR); //handles extension for a widget.

	gtk_widget_set_events(canvas, GDK_EXPOSURE_MASK
						| GDK_LEAVE_NOTIFY_MASK
						| GDK_BUTTON_PRESS_MASK
						| GDK_POINTER_MOTION_MASK
						| GDK_POINTER_MOTION_HINT_MASK); // hint?...

	gtk_widget_show(window);

	gtk_main();

	return 0;


}