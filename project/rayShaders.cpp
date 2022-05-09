#include <math.h>
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"

// uses framework OpenGL
// uses framework Cocoa

GLfloat vertices[] =
{
  1.0f,  1.0f,   0.0f,
  1.0f,  -1.0f,  0.0f,
  -1.0f, -1.0f,  0.0f,

  -1.0f,  1.0f,  0.0f,
  1.0f,   1.0f,  0.0f,
  -1.0f,  -1.0f, 0.0f,
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
GLuint program;
GLfloat planesN = 6;
GLfloat planes[6][2][3] = {
    {{0.0, 0.0, -1.0}, {0.0, 0.0, 1.0}},
    {{0.0, 0.0, 7.5}, {0.0, 0.0, -1.0}},
    {{0.0, -3.0, 0.0}, {0.0, 1.0, 0.0}},
    {{0.0, 3.0, 0.0}, {0.0, -1.0, 0.0}},
    {{-3.0, 0.0, 0.0}, {1.0, 0.0, 0.0}},
    {{3.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}},
};
GLfloat triangles_N = 2;
GLfloat triangles_[6][3][3] = {
  0.0, 0.5, 3.0,
   -0.5, -0.5, 2.5,
   -0.5, -0.5, 3.5,

  10.0, 0.5, 3.0,
   10.5, -0.5, 3.5,
   10.5, -0.5, 2.5,

  0.5, -0.5, 2.5,
   0.5, -0.5, 3.5,
   -0.5, -0.5, 3.5,

  -0.5, -0.5, 2.5,
   0.5, -0.5, 2.5,
   -0.5, -0.5, 3.5,

  0.0, 0.5, 3.0,
   0.5, -0.5, 2.5,
   -0.5, -0.5, 2.5,

  0.0, 0.5, 3.0,
   -0.5, -0.5, 3.5,
   0.5, -0.5, 3.5,

};
// GLfloat triangles_[6][3][3] = {
//   {{0.0, 0.5, 3.0},
//    {-0.5, -0.5, 2.5},
//    {-0.5, -0.5, 3.5}},

//   {{0.0, 0.5, 3.0},
//    {0.5, -0.5, 3.5},
//    {0.5, -0.5, 2.5}},

//   {{0.5, -0.5, 2.5},
//    {0.5, -0.5, 3.5},
//    {-0.5, -0.5, 3.5}},

//   {{-0.5, -0.5, 2.5},
//    {0.5, -0.5, 2.5},
//    {-0.5, -0.5, 3.5}},

//   {{0.0, 0.5, 3.0},
//    {0.5, -0.5, 2.5},
//    {-0.5, -0.5, 2.5}},

//   {{0.0, 0.5, 3.0},
//    {-0.5, -0.5, 3.5},
//    {0.5, -0.5, 3.5}},

// };
GLfloat spheresN = 0;
GLfloat spheres[2][4] = {
  {0.0, 0.0, 4.5, 1.0},
  {0.0, 1.0, 3.0, 0.2},
};
GLfloat lightN = 3;
GLfloat lights[6][3] = {
  {0.0, 0.0, 7.0},
  {0.0, 2.0, 2.0},
  {2.0, -2.0, 4.5},
};
GLfloat colors[17][3] = {
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},

};

void init(void){

  unsigned int vertexBufferObjID;
  unsigned int texCoordBufferObjID;

  dumpInfo();

  glClearColor(0.5,0.2,0.5,0);

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

  glClearDepth(10.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 500.0;

  glBindVertexArray(vertexArrayObjID);	// Select VAO
  glDrawArrays(GL_TRIANGLES, 0, 3*2);	// draw object

  printError("mid-display");

  spheres[1][1] = cos(t/2) * 1.5;
  spheres[1][2] = sin(t/2) * 1.5 + 4.5;
  //lights[2][0] = cos(t/3) * 15;

  glUniform1i(glGetUniformLocation(program, "planesN"), planesN);
  glUniformMatrix2x3fv(glGetUniformLocation(program, "planes"), planesN, GL_FALSE, (GLfloat*) planes[0][0]);
  glUniform1i(glGetUniformLocation(program, "triangles_N"), triangles_N);
  glUniformMatrix3fv(glGetUniformLocation(program, "triangles_"), triangles_N, GL_FALSE, (GLfloat*) triangles_[0][0]);
  glUniform1i(glGetUniformLocation(program, "spheresN"), spheresN);
  glUniform4fv(glGetUniformLocation(program, "spheres"), spheresN, (GLfloat*) spheres[0]);
  glUniform1i(glGetUniformLocation(program, "lightN"), lightN);
  glUniform3fv(glGetUniformLocation(program, "lights"), lightN, (GLfloat*) lights[0]);
  glUniform3fv(glGetUniformLocation(program, "colors"), planesN + triangles_N + spheresN + lightN, (GLfloat*) colors[0]);

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
