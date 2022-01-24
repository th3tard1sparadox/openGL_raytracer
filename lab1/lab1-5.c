// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

#include <math.h>
#include "GL_utilities.h"
#include "MicroGlut.h"
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

  1.0f, 0.0f, 0.0f, //R
  0.0f, 0.0f, 1.0f, //B
  1.0f, 1.0f, 0.0f, //Y

  1.0f, 0.0f, 0.0f, //R
  1.0f, 1.0f, 0.0f, //Y
  1.0f, 0.0f, 1.0f, //P

  1.0f, 0.0f, 0.0f, //R
  1.0f, 0.0f, 1.0f, //P
  0.0f, 1.0f, 0.0f, //G

  1.0f, 0.0f, 1.0f, //P
  1.0f, 1.0f, 0.0f, //Y
  0.0f, 1.0f, 0.0f, //G

  1.0f, 1.0f, 0.0f, //Y
  0.0f, 0.0f, 1.0f, //B
  0.0f, 1.0f, 0.0f //G
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

GLuint program;

// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;
	// Reference to shader program

	dumpInfo();

	// GL inits
	glClearColor(0.5,0.2,0.5,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
  glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*12*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

  glGenBuffers(1, &colorBufferObjID);
  glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, 9*12*sizeof(GLfloat), color, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "Color"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "Color"));

  glUniformMatrix4fv(glGetUniformLocation(program, "xRotationMatrix"), 1, GL_TRUE, xRotationMatrix);
  glUniformMatrix4fv(glGetUniformLocation(program, "xTranslationMatrix"), 1, GL_TRUE, xTranslationMatrix);
	
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

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3*12);	// draw object
	
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
