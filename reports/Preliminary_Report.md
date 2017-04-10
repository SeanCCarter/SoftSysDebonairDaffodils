#Debbonair Daffodils Preliminary Report

##Project goals:
We aim to create an image editing program of the caliber of Microsoft Paint. Our minimum viable product is a program that can open an image file/create an empty image in the form of a bitmap, let the user edit the image with mouse input, and save the resulting image. We would like to enhance this by adding new tool options (e.g. , colors, image formatting) with an input API that could then be attached to a GUI, etc. We are most interested in image manipulation tools, rather than building the perfect GUI.

##Completed Trello Cards:
Completed Project proposal: [github](https://github.com/SeanCCarter/SoftSysDebonairDaffodils/blob/master/reports/proposal.md)
Implement a program that can view an image in C [github](https://github.com/SeanCCarter/SoftSysDebonairDaffodils/blob/master/pixmap_test.c)
Implement a program that uses mouse events to edit an image [github](https://github.com/SeanCCarter/SoftSysDebonairDaffodils/blob/master/pixmap_test.c)
Implement one extra tool [github](https://github.com/SeanCCarter/SoftSysDebonairDaffodils/blob/master/flood_fillDEV.c)

##Annotated Bibliography:
(http://homepages.inf.ed.ac.uk/rbf/BOOKS/PHILLIPS/cips2ed.pdf)
This is an interesting resource for image processing in C - its basic chapters could be useful while we learn to interact with with images in C (loading, changing pixel values, saving), and later chapters could be foundation for the tools in our stretch goals.
(http://www.programmingsimplified.com/c/graphics.h)
This resource is a thorough exploration of the standard graphics library, with a large number of useful examples. We think that it could be another place to find code which we can expand upon.
(https://developer.gnome.org/gtk-tutorial/stable/)
We are using gtk as a basis for image editing, and the documentation on what it can do is instrumental to our being able to work with it in creative ways. The tutorial was a helpful kicking off point for the project.


##Proof of Concept:
![A stunning example of art](https://github.com/SeanCCarter/SoftSysDebonairDaffodils/blob/master/reports/images/pixmap_screenshot.png)

##Current Backlog:
[Trello](https://trello.com/b/TqZ9wuM1/softsysdebonairdaffodils)


##Potential Difficulties:
Most of our difficulty lies in working directly with the images. In other words, using GTK to access exact pixel values is a little difficult, so we are working on getting the knowledge required to work with them directly. We need to be able to do this to fully integrate extra tools like the mostly completed “flood fill”, because they rely on knowing pixel values in order to work. Things like changing line thickness would also benefit from this ability.
