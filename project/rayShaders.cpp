#include <math.h>
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"

// uses framework OpenGL
// uses framework Cocoa

GLuint program;

void init(void){
  glClearColor(0.5,0.2,0.5,0);
  printError("GL inits");

  program = loadShaders("rayShaders.vert", "rayShaders.frag");
  printError("Program");

  glutRepeatingTimer(16);
}

void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 5000.0;

  printError("display");

  glutSwapBuffers();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitContextVersion(3, 2);
  glutCreateWindow ("not phong");
  glutDisplayFunc(display);
  init ();
  glutMainLoop();
  return 0;
}
