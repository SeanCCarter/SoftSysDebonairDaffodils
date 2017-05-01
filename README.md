## About Sketchy-Sketch

We aim to bring some of the basic fucntionality of [MS Paint](https://en.wikipedia.org/wiki/Microsoft_Paint) to the world of Linux. The most essential features are the ability to open a blank image, add shapes of various colors to it, and then save the image again. We also used this project as an oportunity to explore the fascinating world of low-level graphics in C.

### Background

We explored a lot of different resources dedicated to working with images in C. There are a large number of tools that are designed to interact with images in various ways. The [graphics.h](http://www.programmingsimplified.com/c/graphics.h) and [gtk](https://developer.gnome.org/gtk-tutorial/stable/) libraries were two early ones that we tried - they were excellent higher level tools for building GUI's, but didn't allow accces to the base pixels in the way that we wanted them to.

Our project was a simpler tool, designed to edit a [pixmap](https://en.wiktionary.org/wiki/pixmap) at the pixel level. These object are very simple - just arrays that separately record each r, g, or b from a pixel, as well as an [alpha channel](https://en.wikipedia.org/wiki/Alpha_compositing), or transparency channel of the image. By acting on the individual image pixels, it is possible to impliment useful algorithms like [floodfill](https://en.wikipedia.org/wiki/Flood_fill), which are not built into most of the higher level libraries that we found.

### Implementation
Sketchy-Sketch is a large program built around editing a single, very long array (of size height\*width\*4). Starting from the left hand corner, every value in the array represents an (r,g,b,a) value, which are indexed and edited by various helper functions that we have created. We use the [SDL2 library](https://wiki.libsdl.org/) to display this in a new window, as well as access mouse and key commands that control what pixels are changing.

##### The Pixmap

### Results
<iframe width="560" height="315" src="https://www.youtube.com/embed/rpH6GT8m4so" frameborder="0" allowfullscreen></iframe>
