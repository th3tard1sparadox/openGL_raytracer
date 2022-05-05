#include <math.h>
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"

// uses framework OpenGL
// uses framework Cocoa

GLfloat vertices[] =
{
  1.0f,  1.0f,  0.5f,
  1.0f,  -1.0f,  0.5f,
  -1.0f, -1.0f, 0.5f,

  -1.0f,  1.0f,  0.5f,
  1.0f,   1.0f,  0.5f,
  -1.0f,  -1.0f, 0.5f,
};

GLfloat texCoords[] = {
  1.0f,  1.0f,
  1.0f,  -1.0f,
  -1.0f, -1.0f,

  -1.0f,  1.0f,
   1.0f,  1.0f,
  -1.0f,  -1.0f,
};

// vertex array object
unsigned int vertexArrayObjID;
unsigned int texCoordArrayObjID;
GLfloat resolution[] = {400, 400};
GLuint program;
GLfloat sphere1[4] = {0.0, 0.0, 1.0, 0.5};
GLfloat light1[3] = {0.0, 3, 0.0};
GLfloat light2[3] = {0.0, -0.5, 1.0};

void init(void){

  unsigned int vertexBufferObjID;
  unsigned int texCoordBufferObjID;

  dumpInfo();

  glClearColor(0.5,0.2,0.5,0);

  glEnable(GL_DEPTH_TEST);


  printError("GL inits");

  program = loadShaders("rayShaders.vert", "rayShaders.frag");
  printError("Program");

  // Allocate and activate Vertex Array Object
  glGenVertexArrays(1, &vertexArrayObjID);
  glBindVertexArray(vertexArrayObjID);
  // Allocate Vertex Buffer Objects
  glGenBuffers(1, &vertexBufferObjID);

  // Allocate Texcoord Buffer Objects
  glGenBuffers(1, &texCoordBufferObjID);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, 9*2*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

  printError("vertices input");

  glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, 6*2*sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_texCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_texCoord"));

  printError("texCoord input");
  glutRepeatingTimer(16);
}

void display(void){

  printError("pre-display");

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 500.0;


  glUniformMatrix2fv(glGetUniformLocation(program, "resolution"), 1, GL_TRUE, resolution);

  glBindVertexArray(vertexArrayObjID);	// Select VAO
  glDrawArrays(GL_TRIANGLES, 0, 3*2);	// draw object

  printError("mid-display");

  // sphere1[0] = sin(t);
  light1[2] = cos(t);
  light1[0] = sin(t);

  light2[1] = sin(t)*10;

  glUniform4fv(glGetUniformLocation(program, "sphere1"), 1, sphere1);
  glUniform3fv(glGetUniformLocation(program, "light1"), 1, light1);
  glUniform3fv(glGetUniformLocation(program, "light2"), 1, light2);

  printError("display");

  glutSwapBuffers();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitContextVersion(3, 2);
  glutCreateWindow ("GL3 white triangle example");
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutDisplayFunc(display);
  init ();
  glutMainLoop();
  return 0;
}
