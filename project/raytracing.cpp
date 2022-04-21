// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.


#include "MicroGlut.h"
#include "LittleOBJLoader.h"

#include "raytracing.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

const GLfloat ASPECT_RATIO = 16.0 / 9.0;
const GLint WIDTH = 400;
const GLint HEIGHT = static_cast<int>(WIDTH / ASPECT_RATIO);
const int SAMPLES = 10;
const int MAX_DEPTH = 50;

hittable_list* world = new hittable_list;
vec3 origin, horizontal, vertical, lower_left_corner;
camera cam;

/* OPEN GL */

void init(){
  glClearColor(0.5,0.2,0.5,0);

  world->add(make_shared<sphere>(point3(-1.5,0,-2), 0.5));
  world->add(make_shared<sphere>(point3(0,1,-2), 0.5));
  world->add(make_shared<sphere>(point3(0,-100.5,-1), 100));

  glutRepeatingTimer(16);
}

void display()
{
  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000.0;

  world->move(vec3(sin(t)/10, 0 , 0), 0);

  // Scan
  for (int j = HEIGHT-1; j >= 0; --j) {
    for (int i = 0; i < WIDTH; ++i) {
      color pixel_color(0.0, 0.0, 0.0);
      ray r = cam.get_ray(i, j);
      pixel_color += ray_color(r, world, MAX_DEPTH);
      makePixel(i, j, PixelBuffer, WIDTH, HEIGHT, pixel_color, SAMPLES);
    }
  }

  // Draw
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
  glutSwapBuffers();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Hello Graphics!!");

  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;
}
