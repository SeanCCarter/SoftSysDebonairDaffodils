# About Sketch-a-Sketch

We aim to bring some of the basic fucntionality of [MS Paint](https://en.wikipedia.org/wiki/Microsoft_Paint) to the world of Linux. The most essential features are the ability to open a blank image, add shapes of various colors to it, and then save the image again. We also used this project as an oportunity to explore the fascinating world of low-level graphics in C.

## Background

We explored a lot of different resources dedicated to working with images in C. There are a large number of tools that are designed to interact with images in various ways. The [graphics.h](http://www.programmingsimplified.com/c/graphics.h) and [gtk](https://developer.gnome.org/gtk-tutorial/stable/) libraries were two early ones that we tried - they were excellent higher level tools for building GUI's, but didn't allow accces to the base pixels in the way that we wanted them to.

Our project was a simpler tool, designed to edit a [pixmap](https://en.wiktionary.org/wiki/pixmap) at the pixel level. These object are very simple - just arrays that separately record each r, g, or b from a pixel, as well as an [alpha channel](https://en.wikipedia.org/wiki/Alpha_compositing), or transparency channel of the image. By acting on the individual image pixels, it is possible to impliment useful algorithms like [floodfill](https://en.wikipedia.org/wiki/Flood_fill), which are not built into most of the higher level libraries that we found.

## Implementation
Sketchy-Sketch is a large program built around editing a single, very long array (of size height\*width\*4). Starting from the left hand corner, every value in the array represents an (r,g,b,a) value, which are indexed and edited by various helper functions that we have created. We use the [SDL2 library](https://wiki.libsdl.org/) to display this in a new window, as well as access mouse and key commands that control what pixels are changing.

##### Finding the Right Library
We spent a lot of time identifying the SDL2 library as the one that we needed. It is important do discuss this , since much of the difficulty of the project came from finding tools that either contained the full suite of options that we needed, or were compatable with each other. Most of them had limeted, overly complex, or simply innacurate documentation. SDL (the one we used) is designed to provide access to low level mouse and key events, while displaying a canvas. This made it ideal for our purposes. For future students who may wish to work with pixel values in the same way, these were some of the ones that didn't work:

- GTK: GTK provided good tools and a canvas, but no access to individula pixel values
- GTK + Keras: The GTK documentation stated that Keras allowed access to individual pixel values, but the two libraries are not actually compatable. 
- \<graphics.h\> : Unfortunately, this was excellent, but it was a windows only library

##### The Pixmap

##### Opening and Saving Files

##### Floodfill

## Results

###### The Brush

###### Floodfill

Here, you can see how floodfill can quickly color in all of the pixels in a contained area.
<iframe width="560" height="315" src="https://www.youtube.com/embed/rpH6GT8m4so" frameborder="0" allowfullscreen></iframe>
