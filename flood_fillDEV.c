/*
Experimental function for flood filling recursively. Includes slight modifications to mousedown_event that 
allow use of the flood fill function by right clicking with any tool/color.
*/

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <graphics.h>
#include <dos.h>


//Experimental floodfill
void floodFill(int x,int y,int orig,int fill)
{
    if(getpixel(x,y) == orig)
    {
        putpixel(x,y,fill);
        floodFill(x+1,y,orig,fill);
        floodFill(x,y+1,orig,fill);
        floodFill(x-1,y,orig,fill);
        floodFill(x,y-1,orig,fill);
    }
}

// handle mousedown event ("button")
static gboolean mousedown_event(GtkWidget* widget, GdkEventButton* event, gpointer tool_data){
	Tool* t = tool_data;
	if (event->button == 1 && pixmap != NULL) {
		draw_tool(widget, event->x, event->y, t);
	}
	if (event->button == 2 && pixmap != NULL) { ///
		floodFill(event->x,event->y,getpixel(event->x,event->y),t->color); //TODO
	} ///
	return TRUE;
}


