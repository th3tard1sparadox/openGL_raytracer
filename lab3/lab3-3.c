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
#define kGroundSize 100.0f

GLfloat vertices[] =
{
  -kGroundSize,0.0f,-kGroundSize,
  -kGroundSize,0.0f,kGroundSize,
  kGroundSize,-0.0f,-kGroundSize,
  kGroundSize,-0.0f,kGroundSize
};


GLfloat vertex_normals[] =
{
  0.0f,1.0f,0.0f,
  0.0f,1.0f,0.0f,
  0.0f,1.0f,0.0f, 
  0.0f,1.0f,0.0f
};

GLfloat tex_coords[] =
{
  0.0f,0.0f,
  0.0f,20.0f,
  20.0f,0.0f, 
  20.0f,20.0f
};

GLuint indices[] = {0, 1, 2, 1, 3, 2};

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
Model *balcony, *roof, *wall, *blade, *ground, *sky;
Model *kettle;
GLuint program, texProgram;
GLuint skyboxTex;
vec3 pos, forward;

// vertex array object
unsigned int bunnyVertexArrayObjID;

vec3 rot = {0,0,0};
float startX = 0;
void rotCam(int x, int y) {

  float ang = (x - startX) * 0.005;
  float oldx = forward.x;
  float oldz = forward.z;
  forward.x = cos(ang)*oldx - sin(ang)*oldz;
  forward.z = sin(ang)*oldx + cos(ang)*oldz;

  startX = x;

  glutPostRedisplay();
}

void moveCam(unsigned char c, int x, int y){
  vec3 mov = {0,0,0};
  switch(c){
  case 'w':
    mov = normalize(forward);
    glutPostRedisplay();
    break;
  case 's':
    mov = ScalarMult(normalize(forward), -1);
    glutPostRedisplay();
    break;
  default:
    mov = (vec3) {0,0,0};
  }
  pos.x += mov.x;
  pos.z += mov.z;
}

void init(void)
{
  program = loadShaders("lab3-3.vert", "lab3-3.frag");
  glUseProgram(program);

  pos = (vec3) {20, 1, 20};
  forward = (vec3) {-0.5, 0, -0.5};

  // vertex buffer object, used for uploading the geometry
  glutPassiveMotionFunc(&rotCam);
  glutKeyboardFunc(&moveCam);

  // Reference to shader program
  mtvMat = T(0,0,0);
  rotOri = T(4.6, 9.2, 0);
  blade = LoadModel("windmill/blade.obj");
  balcony = LoadModel("windmill/windmill-balcony.obj");
  roof = LoadModel("windmill/windmill-roof.obj");
  wall = LoadModel("windmill/windmill-walls.obj");
  sky = LoadModel("skybox.obj");
  kettle = LoadModel("../lab2/res/teapot.obj");

  vec3 color = (vec3) {0,0,0};
  ground = LoadDataToModel((vec3*)vertices, (vec3*)vertex_normals, (vec2*)tex_coords, &color, indices, 4, 6);

  LoadTGATextureSimple("SkyBox512.tga", &skyboxTex);

  dumpInfo();

  // GL inits
  glClearColor(0.8,0.97,1,1);
  glEnable(GL_DEPTH_TEST);
  printError("GL inits");

  // Load and compile shader
  glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

  texProgram = loadShaders("lab3-3_tex.vert", "lab3-3_tex.frag");
  printError("init shader");

  // Upload geometry to the GPU:
  glUniformMatrix4fv(glGetUniformLocation(texProgram, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
  glUniform1i(glGetUniformLocation(texProgram, "texUnit"), 0);

  glutRepeatingTimer(16);

  // End of upload of geometry

  printError("init arrays");
}


void display(void)
{
  printError("pre display");

  vec3 u = {0,1,0};
  lookMatrix = lookAtv(pos, VectorAdd(forward, pos), u);

  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Skybox
  glDisable(GL_DEPTH_TEST);
  glUseProgram(texProgram);
  mtvMat = lookMatrix;
  mtvMat.m[3] = 0;
  mtvMat.m[7] = 0;
  mtvMat.m[11] = 0;
  glUniformMatrix4fv(glGetUniformLocation(texProgram, "mdlMatrix"), 1, GL_TRUE, mtvMat.m);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, skyboxTex);
  DrawModel(sky, texProgram, "in_Position", NULL, "inTexCoord");
  glEnable(GL_DEPTH_TEST);

  glUseProgram(program);

  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000.0;

  mtvMat = T(0,0,0);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mtvMat.m);
  DrawModel(ground, program, "in_Position", "in_Normal", NULL);
  DrawModel(balcony, program, "in_Position", "in_Normal", NULL);
  DrawModel(roof, program, "in_Position", "in_Normal", NULL);
  DrawModel(wall, program, "in_Position", "in_Normal", NULL);

  transBlade = Mult(mtvMat, Mult(rotOri, Rx(t*5)));
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transBlade.m);
  DrawModel(blade, program, "in_Position", "in_Normal", NULL);

  transBlade = Mult(mtvMat, Mult(rotOri, Rx((t+PI/2)*5)));
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transBlade.m);
  DrawModel(blade, program, "in_Position", "in_Normal", NULL);

  transBlade = Mult(mtvMat, Mult(rotOri, Rx((t+PI)*5)));
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transBlade.m);
  DrawModel(blade, program, "in_Position", "in_Normal", NULL);

  transBlade = Mult(mtvMat, Mult(rotOri, Rx((t+PI*3/2)*5)));
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, transBlade.m);
  DrawModel(blade, program, "in_Position", "in_Normal", NULL);

  mtvMat = T(-10, 0, -23);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mtvMat.m);
  DrawModel(kettle, program, "in_Position", "in_Normal", NULL);

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
