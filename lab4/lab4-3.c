// Lab 4, terrain generation

	// uses framework Cocoa
	// uses framework OpenGL
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

mat4 projectionMatrix;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	vec3 *vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec3 *normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec2 *texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)].x = x / 1.0;
			vertexArray[(x + z * tex->width)].y = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
			vertexArray[(x + z * tex->width)].z = z / 1.0;
    }
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Normal vectors. You need to calculate these.
      int neighbors = 0;
      
      vec3 A = vertexArray[(x + z * tex->width)];
      vec3 neiNorms[4];
      if(x != 0) { // can check left
        if(z != 0) { // can check up
          vec3 B = vertexArray[((x - 1) + z * tex->width)];
          vec3 C = vertexArray[(x + (z - 1) * tex->width)];
          neiNorms[neighbors] = CalcNormalVector(A, C, B);
          neighbors++;
        }
        if(z != tex->height-1) { // can check down
          vec3 B = vertexArray[(x + (z + 1) * tex->width)];
          vec3 C = vertexArray[((x - 1) + z * tex->width)];
          neiNorms[neighbors] = CalcNormalVector(A, C, B);
          neighbors++;
        }
      }
      if(x == tex->width-1) { // can check right
        if(z != 0) { // can check up
          vec3 B = vertexArray[(x + (z - 1) * tex->width)];
          vec3 C = vertexArray[((x + 1) + z * tex->width)];
          neiNorms[neighbors] = CalcNormalVector(A, C, B);
          neighbors++;
        }
        if(z != tex->height-1) { // can check down
          vec3 B = vertexArray[((x + 1) + z * tex->width)];
          vec3 C = vertexArray[(x + (z + 1) * tex->width)];
          neiNorms[neighbors] = CalcNormalVector(A, C, B);
          neighbors++;
        }
      }

      vec3 norm = {0,0,0};
      for(int i = 0; i < neighbors; i++) {
        norm.x += neiNorms[i].x;
        norm.y += neiNorms[i].y;
        norm.z += neiNorms[i].z;
      }

      norm.x = norm.x / neighbors;
      norm.y = norm.y / neighbors;
      norm.z = norm.z / neighbors;
 
      norm = normalize(norm);
        
			normalArray[(x + z * tex->width)].x = norm.x;
			normalArray[(x + z * tex->width)].y = norm.y;
			normalArray[(x + z * tex->width)].z = norm.z;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)].x = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)].y = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain
vec3 pos, forward;

vec3 rot = {0,0,0};
float startX = 0;
void rotCam(int x, int y)
{

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
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 100000.0);

  pos = (vec3) {30, 50, -25};
  forward = (vec3) {-0.4, 0, 0.6};

  glutPassiveMotionFunc(&rotCam);
  glutKeyboardFunc(&moveCam);

	// Load and compile shader
	program = loadShaders("terrain_light.vert", "terrain_light.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);
	
// Load terrain data
	
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 total, modelView, camMatrix;
	
	printError("pre display");
	
	glUseProgram(program);

	// Build matrix
	
  vec3 u = {0,1,0};
	camMatrix = lookAtv(pos, VectorAdd(forward, pos), u);

	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

/*
void mouse(int x, int y)
{
	printf("%d %d\n", x, y);
}
*/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	//glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
