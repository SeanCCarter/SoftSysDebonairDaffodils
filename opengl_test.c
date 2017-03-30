/*  
Hello World for openGL, checking to make sure it's installed correctly and such.
Run with:

sudo apt-get install freeglut3-dev
sudo apt-get install mesa-utils
gcc opengl_test.c -o ogt -lglut -lGL
./ogt

-lglut, -lGL is how it knows to use the appropriate freeglut3 stuff I think.

freeglut:
  " A crossplatform windowing and keyboard/mouse handler. Its API is a superset 
  of rhe GLUT API, and it is more stable and up to date than GLUT. It supports 
  creating a core OpenGL context."
solid documentation online which is nice!

*/


#include <GL/gl.h> // basic GL functions
#include <GL/glu.h> // higher level - GL utilities
#include <GL/glut.h> // highest level - GL utilities toolkit


void setup() {   
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
}

void display()
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glColor3f(0.0f, 0.0f, 0.0f);
      glRectf(-0.75f,0.75f, 0.75f, -0.75f);
      glutSwapBuffers();
   }
int main(int argc, char *argv[])
  {
    int width, height, topleftx, toplefty;
    width = 640;
    height = 480; // todo: make this argv-able.
    topleftx = 400;
    toplefty = 200;

    glutInit(&argc, argv); // todo: figure out what this means even
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(width, height); // pixels
    glutInitWindowPosition(topleftx, toplefty); // pxl from top left corner of screen
    // void glutInitWindowPosition (int x, int y) : position to open next window at, pixels from topleft.
     
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
     glutInitWindowSize(640,480);
     glutCreateWindow("test");

     setup();
     glutDisplayFunc(display);
     glutMainLoop();
     return 0;
  }