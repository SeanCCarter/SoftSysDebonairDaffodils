# Project 2 Proposal
### Software System
##### Sean, Gabe, Matt

---

#### Project goals:

We aim to create an image editing program of the caliber of Microsoft Paint. Our minimum viable product is a program that can open an image file/create an empty image in the form of a bitmap, let the user edit the image with mouse input, and save the resulting image. We would like to enhance this by adding new tool options (e.g. , colors, image formatting) with an input API that could then be attached to a GUI, etc. We are most interested in image manipulation tools, rather than building the perfect GUI.

#### Product Backlog:
Our backlog is located on our [trello board](https://trello.com/b/TqZ9wuM1/softsysdebonairdaffodils).

#### Current Research:
###### Useful Libraries:

    <graphics.h> (or openGL or Cairo)
    <unistd.h>
    <fcntl.h>
    <X11/Xlib.h> (gcc -lX11)
    <linux/input.h>

###### Annotated Bibliography:
- http://homepages.inf.ed.ac.uk/rbf/BOOKS/PHILLIPS/cips2ed.pdf
</br> This is an interesting resource for image processing in C - its basic chapters could be useful while we learn to interact with with images in C (loading, changing pixel values, saving), and later chapters could be foundation for the tools in our stretch goals.

- http://www.programmingsimplified.com/c/graphics.h
</br> This resource is a thorough exploration of the standard graphics library, with a large number of useful examples. We think that it could be another place to find code which we can expand upon.

##### Potential Roadblocks:
The biggest roadblocks will be making sure that the libraries which we have found are suitable for the purposes that we want to use them for. It may be more difficult to get mouse events and set up the appropriate callback functions in C than in other languages.
