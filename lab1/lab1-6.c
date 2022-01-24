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
// uses framework OpenGL
// uses framework Cocoa

// Globals
// Data would normally be read from files
GLfloat xTranslationMatrix[] = 
{
  1.0f,0.0f,0.0f,0.5f,
  0.0f,1.0f,0.0f,0.0f,
  0.0f,0.0f,1.0f,0.0f,
  0.0f,0.0f,0.0f,1.0f 
};

GLfloat xRotationMatrix[] = 
{
  1.0f,0.0f,0.0f,0.0f,
  0.0f,1.0f,0.0f,0.0f,
  0.0f,0.0f,1.0f,0.0f,
  0.0f,0.0f,0.0f,1.0f 
};

GLfloat zRotationMatrix[] = 
{
  1.0f,0.0f,0.0f,0.0f,
  0.0f,1.0f,0.0f,0.0f,
  0.0f,0.0f,1.0f,0.0f,
  0.0f,0.0f,0.0f,1.0f 
};

GLfloat color[] = 
{ 
  1.0f, 0.0f, 0.0f, //R
  0.0f, 1.0f, 0.0f, //G
  0.0f, 0.0f, 1.0f, //B
};

GLfloat vertices[] =
{
	0.0f,0.5f,0.0f,
	-0.5f,-0.5f,0.5f,
	0.5f,-0.5f,0.5f,

	0.0f,0.5f,0.0f,
	0.5f,-0.5f,0.5f,
	0.5f,-0.5f,-0.5f,

	0.0f,0.5f,0.0f,
	0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,

	0.0f,0.5f,0.0f,
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,0.5f,

	-0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,0.5f,

	0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,0.5f,
	-0.5f,-0.5f,0.5f
};

Model *m;

GLuint program;

// vertex array object
unsigned int bunnyVertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int colorBufferObjID;

  unsigned int bunnyVertexBufferObjID;
  unsigned int bunnyIndexBufferObjID;
  unsigned int bunnyNormalBufferObjID;
	// Reference to shader program

  m = LoadModel("bunny.obj");
	dumpInfo();

	// GL inits
	glClearColor(0.5,0.2,0.5,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glDisable(GL_CULL_FACE);

	// Load and compile shader
	program = loadShaders("lab1-6.vert", "lab1-6.frag");
  glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
  glGenVertexArrays(1, &bunnyVertexArrayObjID);
  glGenBuffers(1, &bunnyVertexBufferObjID);
  glGenBuffers(1, &bunnyIndexBufferObjID);
  glGenBuffers(1, &bunnyNormalBufferObjID);
  glBindVertexArray(bunnyVertexArrayObjID);

  // VBO for vertex data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

  // VBO for normal data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

  glGenBuffers(1, &colorBufferObjID);

  glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), color, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "Color"));

  glutRepeatingTimer(16);

	// End of upload of geometry
	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  xRotationMatrix[0] = (float)cos(t);
  xRotationMatrix[1] = (float)-sin(t); 
  xRotationMatrix[4] = (float)sin(t); 
  xRotationMatrix[5] = (float)cos(t); 

  zRotationMatrix[5] = (float)cos(t);
  zRotationMatrix[6] = (float)-sin(t); 
  zRotationMatrix[9] = (float)sin(t); 
  zRotationMatrix[10] = (float)cos(t); 

  glUniformMatrix4fv(glGetUniformLocation(program, "xRotationMatrix"), 1, GL_TRUE, xRotationMatrix);
  glUniformMatrix4fv(glGetUniformLocation(program, "zRotationMatrix"), 1, GL_TRUE, zRotationMatrix);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
  glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}
