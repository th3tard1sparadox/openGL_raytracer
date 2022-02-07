// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

#include <math.h>
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"
#define PI 3.1415f
#define near 1.0
#define far 30.0
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

mat4 transBunny, transTeddy;
mat4 lookMatrix, totalLookMatrix, zrot;
Model *bunny, *teddy;
GLuint program;
GLuint texture[2];

// vertex array object
unsigned int bunnyVertexArrayObjID;

void init(void)
{
  // vertex buffer object, used for uploading the geometry
  unsigned int colorBufferObjID;

  // Reference to shader program

  bunny = LoadModel("res/bunnyplus.obj");
  teddy = LoadModel("res/teapot.obj");
  dumpInfo();

  // GL inits
  glClearColor(0.5,0.2,0.5,0);
  glEnable(GL_DEPTH_TEST);
  printError("GL inits");

  // Load and compile shader
  program = loadShaders("lab2-7.vert", "lab2-7.frag");
  printError("init shader");

  // Upload geometry to the GPU:
  glGenTextures(2, texture);

  LoadTGATextureSimple("res/rutor.tga", &texture[0]);
  LoadTGATextureSimple("res/maskros512.tga", &texture[1]);

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

  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000.0;

  transBunny = T(0.7,0,0);
  transTeddy = Mult(T(-1,-0.5,0),S(0.1, 0.1, 0.1));

  lookMatrix = lookAt( 4*sin(t), 2.0, 4*sin(t-PI/2),
                       0.0, 0.0, 0.0,
                       0.0, 1.0, 0.0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transBunny.m);
  glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
  DrawModel(bunny, program, "in_Position", "in_Normal", "inTexCoord");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transTeddy.m);
  glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
  DrawModel(teddy, program, "in_Position", "in_Normal", "inTexCoord");

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
