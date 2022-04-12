// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

#include <math.h>
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"
#include "ray.h"
#define PI 3.1415f
#define near 1.0
#define far 10000.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

// uses framework OpenGL
// uses framework Cocoa

// Globals
// Data would normally be read from files
GLfloat projectionMatrix[] =
{
  2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
  0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
  0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
  0.0f, 0.0f, -1.0f, 0.0f
};

GLfloat wallColorMatrix[][3] = {
  {1.0, 0.0, 0.0},
  {1.0, 0.5, 0.0},
  {1.0, 0.5, 0.5},
  {1.0, 0.0, 0.5},
  {1.0, 1.0, 0.0},
  {1.0, 1.0, 0.5},
};

mat4 transSphere1, transSphere2;
mat4 transWalls[6];
mat4 lookMatrix, totalLookMatrix, zrot;
Model *sphere1, *sphere2;
Model *walls[6];
GLuint program;

void init(void)
{
  // Reference to shader program
  sphere1 = LoadModel("res/sphere.obj");
  sphere2 = LoadModel("res/sphere.obj");
  for(int i = 0; i < 6; i++){
    walls[i] = LoadModel("res/sphere.obj");
  }
  dumpInfo();

  // GL inits
  glClearColor(0.5,0.2,0.5,0);
  glEnable(GL_DEPTH_TEST);
  printError("GL inits");

  // Load and compile shader
  program = loadShaders("raytracing.vert", "raytracing.frag");
  printError("init shader");

  // Upload geometry to the GPU:
  glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

  glutRepeatingTimer(16);

  // End of upload of geometry

  printError("init arrays");
}


void display(void)
{
  printError("pre display");

  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 5000.0;

  transSphere1 = Mult(T(0,0,0), S(0.8, 0.8, 0.8));
  transSphere2 = Mult(T(1,0,1), S(0.5, 0.5, 0.5));

  transWalls[0] = Mult(T(0,0,-3000), S(5000, 5000, 5000));
  transWalls[1] = Mult(T(0,0,3000), S(5000, 5000, 5000));
  transWalls[2] = Mult(T(-3000,0,0), S(5000, 5000, 5000));
  transWalls[3] = Mult(T(3000,0,0), S(5000, 5000, 5000));
  transWalls[4] = Mult(T(0, -3000, 0), S(5000, 5000, 5000));
  transWalls[5] = Mult(T(0, 3000, 0), S(5000, 5000, 5000));

  lookMatrix = lookAt( 20*sin(t), 5.0, 20*sin(t-PI/2),
                       0.0, 0.0, 0.0,
                       0.0, 1.0, 0.0);

  const GLfloat color1[3] = {0.0, 1.0, 0.0};
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transSphere1.m);
  glUniform3fv(glGetUniformLocation(program, "inColor"), 1, color1);
  DrawModel(sphere1, program, "in_Position", "in_Normal", NULL);

  const GLfloat color2[3] = {0.0, 0.0, 1.0};
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transSphere2.m);
  glUniform3fv(glGetUniformLocation(program, "inColor"), 1, color2);
  DrawModel(sphere2, program, "in_Position", "in_Normal", NULL);

  for(int i = 0; i < 6; i++){
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transWalls[i].m);
    glUniform3fv(glGetUniformLocation(program, "inColor"), 1, wallColorMatrix[i]);
    DrawModel(walls[i], program, "in_Position", "in_Normal", NULL);
  }

  glUniformMatrix4fv(glGetUniformLocation(program, "lookMatrix"), 1, GL_TRUE, lookMatrix.m);

  printError("display");

  glutSwapBuffers();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitContextVersion(3, 2);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow ("not phong");
  glutDisplayFunc(display);
  init ();
  glutMainLoop();
  return 0;
}
