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
#define far 1000.0
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

mat4 mtvMat, transBlade, rotOri;
mat4 lookMatrix;
Model *balcony, *roof, *wall, *blade;
GLuint program;
GLuint texture[2];

// vertex array object
unsigned int bunnyVertexArrayObjID;

vec3 rot = {0,0,0};
float startX = 0;
void rotCam(int x, int y) {

  lookMatrix.m[3] += sin(x - startX);
  lookMatrix.m[5] += cos(x - startX);

  startX = x;

  glutPostRedisplay();
}

vec3 mov = {0,0,0};
void moveCam(unsigned char c, int x, int y){
  vec3 tmp = {lookMatrix.m[0]-lookMatrix.m[3], 0, lookMatrix.m[2]-lookMatrix.m[5]};
  switch(c){
  case 'w':
    mov = ScalarMult(normalize(tmp), 0.1);
    glutPostRedisplay();
    break;
  case 's':
    mov = ScalarMult(normalize(tmp), -0.1);
    glutPostRedisplay();
    break;
  default:
    mov = (vec3) {0,0,0};
  }
}

void init(void)
{

  lookMatrix = lookAt( 20, 10, 20,
                       0.0, 5.0, 0.0,
                       0.0, 1.0, 0.0);
  
  // vertex buffer object, used for uploading the geometry
  //glutPassiveMotionFunc(&rotCam);
  //glutKeyboardFunc(&moveCam);

  // Reference to shader program
  mtvMat = T(0,0,0);
  rotOri = T(4.6, 9.2, 0);
  blade = LoadModel("windmill/blade.obj");
  balcony = LoadModel("windmill/windmill-balcony.obj");
  roof = LoadModel("windmill/windmill-roof.obj");
  wall = LoadModel("windmill/windmill-walls.obj");
  dumpInfo();

  // GL inits
  glClearColor(0.5,0.2,0.5,-0.1);
  glEnable(GL_DEPTH_TEST);
  printError("GL inits");

  // Load and compile shader
  program = loadShaders("lab3-2.vert", "lab3-2.frag");
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

  vec3 p = {lookMatrix.m[0] + mov.x, lookMatrix.m[1] + mov.y, lookMatrix.m[2] + mov.z};
  vec3 r = {0.0, 5.0, 0.0};
  vec3 u = {0.0, 1.0, 0.0};

  lookMatrix = lookAtv(p, r, u);

  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000.0;

  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mtvMat.m);
  DrawModel(balcony, program, "in_Position", "in_Normal", "inTexCoord");

  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mtvMat.m);
  DrawModel(roof, program, "in_Position", "in_Normal", "inTexCoord");

  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mtvMat.m);
  DrawModel(wall, program, "in_Position", "in_Normal", "inTexCoord");

  transBlade = Mult(mtvMat, Mult(rotOri, Rx(t*5)));
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transBlade.m);
  DrawModel(blade, program, "in_Position", "in_Normal", "inTexCoord");

  transBlade = Mult(mtvMat, Mult(rotOri, Rx((t+PI/2)*5)));
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transBlade.m);
  DrawModel(blade, program, "in_Position", "in_Normal", "inTexCoord");

  transBlade = Mult(mtvMat, Mult(rotOri, Rx((t+PI)*5)));
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transBlade.m);
  DrawModel(blade, program, "in_Position", "in_Normal", "inTexCoord");

  transBlade = Mult(mtvMat, Mult(rotOri, Rx((t+PI*3/2)*5)));
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transBlade.m);
  DrawModel(blade, program, "in_Position", "in_Normal", "inTexCoord");

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
