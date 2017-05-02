# About Sketch-a-Sketch

We aim to bring some of the basic fucntionality of [MS Paint](https://en.wikipedia.org/wiki/Microsoft_Paint) to the world of Linux. The most essential features are the ability to open a blank image, add shapes of various colors to it, and then save the image again. We used this project as an oportunity to explore the fascinating world of low-level graphics in C.

## Background

We explored a lot of different resources dedicated to working with images in C. There are a large number of tools that are designed to interact with images in various ways. The [graphics.h](http://www.programmingsimplified.com/c/graphics.h) and [gtk](https://developer.gnome.org/gtk-tutorial/stable/) libraries were two early ones that we tried - they were excellent higher level tools for building GUI's, but didn't allow accces to the base pixels in the way that we wanted them to.

Our project was a simpler tool, designed to edit a [pixmap](https://en.wiktionary.org/wiki/pixmap) at the pixel level. These object are very simple - just arrays that separately record each r, g, or b from a pixel, as well as an [alpha channel](https://en.wikipedia.org/wiki/Alpha_compositing), or transparency channel of the image. By acting on the individual image pixels, it is possible to implement useful algorithms like [floodfill](https://en.wikipedia.org/wiki/Flood_fill), which are not built into most of the higher level libraries that we found.

## Implementation
Sketch-a-Sketch is a large program built around editing a single, very long array (of size height\*width\*4). Starting from the left hand corner, every value in the array represents an (r,g,b,a) value, which are indexed and edited by various helper functions that we have created. We use the [SDL2 library](https://wiki.libsdl.org/) to display this in a new window, as well as access mouse and key commands that control what pixels are changing.

##### Finding the Right Library
We spent a lot of time identifying the SDL2 library as the one that we needed. It is important do discuss this, since much of the difficulty of the project came from finding tools that either contained the full suite of options that we needed, or were compatable with each other. Most of them had limeted, overly complex, or simply innacurate documentation. SDL (the one we used) is designed to provide access to low level mouse and key events, while displaying a canvas. This made it ideal for our purposes. For future students who may wish to work with pixel values in the same way, these were some of the ones that didn't work:

- GTK: GTK provided good tools and a canvas, but no access to individula pixel values
- GTK + Keras: The GTK documentation stated that Keras allowed access to individual pixel values, but the two libraries are not actually compatable. 
- \<graphics.h\> : Unfortunately, this was excellent, but it was a windows only library

##### The Pixmap
The core of the project is the implementation of the pixel map. The image is represented through a long, one-dimensional array of pixel values. We interact with these values using an abstraction on the x,y coordinate system, with x changing the index of the array by one pixel and y changing the index by whatever the length of the row is, providing the illusion of a two dimensional array. This implementation makes the pixmap far easier to work with and greatly simplified many of our image editing functions.

##### Opening and Saving Files
The implementaion of file saving is very simple. We open a standard file object, write the array to it bit by bit, and then close the file. We open the file in the same way - by assuming the file only contains uint8_t integers. This does require us to make assumptions about the size of the image, since we do not encode any of that information inside of the file itself.

##### Floodfill
The floodFill() function implements a basic flood filling algorithm. Given a start pixel, determined by the pixel the user right clicks on, the algorithm looks at the pixel value, determines if it is the same as the original pixel that was being filled, and either fills and recursively calls itself on all surrounding pixels if it is or ceases activity if it isn't.

There are a number of pitfalls that can occur when implementing floodfill on an actual canvas. For example, if the canvas isn't redrawn each time that a pixel is filled in, the state of the canvas never changes. This means that the function will never identify any pixel as being filled. This is why our implimentation of floodfill does redraw the canvas every time. The algorithm isn't particularly fast in its current implementation for filling huge areas, but it can fill small or medium sized shapes in less than a second.

This algorithm makes use of two helper functions, pix_comp() and draw2(). pix_comp() is responsible for neatening the comparison of two pixels, so instead of having a bulky 'if' statement, a simple function can be passed two Pixel structs and if the nature of their sameness is returned as an integer. draw2() is almost the same as draw(), but with the notable lack of any dependence on the radius of the tool being currently used. In other words, the floodFill() works the same way regardless of how large of a tool the user has chosen. This trades efficiency for exactness in a lot of cases, because though a large tool would fill many pixels at once, there would be some pixels lost in the effort, and perhaps some bleed over outside drawn shapes.

## Results

###### The Brush

The basic MVP of Sketch-a-Sketch was the simple brush tool. Here, you can see that you can click and drag to draw on the canvas. You can also adjust the size and color of the brush, making it possible to create interesting multi-colored pictures.
<iframe width="560" height="315" src="https://www.youtube.com/embed/nGr2cvvyMpo" frameborder="0" allowfullscreen></iframe>

###### Floodfill

We also have one other tool implimented - the "fill" tool, which is one of the most important tools for anyone dabbling with MS paint. Here, you can see how floodfill can quickly color in all of the pixels in a contained area.
<iframe width="560" height="315" src="https://www.youtube.com/embed/rpH6GT8m4so" frameborder="0" allowfullscreen></iframe>
