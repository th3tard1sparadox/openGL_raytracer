// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

#include <math.h>
#include <cstdlib>
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"
#include "ray.h"

int WIDTH = 400;
int HEIGHT = 400;
GLubyte* PixelBuffer = new GLubyte[WIDTH * HEIGHT * 3];

void makePixel(int x, int y, GLubyte* pixels, int width, int height)
{
  if (0 <= x && x < width && 0 <= y && y < height) {
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;
    int position = (x + y * width) * 3;
    pixels[position] = r;
    pixels[position + 1] = g;
    pixels[position + 2] = b;
  }
}

void init(){
  glClearColor(0.5,0.2,0.5,0);
  glutRepeatingTimer(16);
}

void display()
{
  for(int y = 0; y < HEIGHT; y++){
    for(int x = 0; x < WIDTH; x++){
      makePixel(x, y, PixelBuffer, WIDTH, HEIGHT);
    }
  }

  glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
  glutSwapBuffers();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Hello Graphics!!");

  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;
}
