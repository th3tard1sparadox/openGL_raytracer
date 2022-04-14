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
const GLint WIDTH = 800;
const GLint HEIGHT = static_cast<int>(WIDTH / ASPECT_RATIO);
const int SAMPLES = 10;

GLubyte* PixelBuffer = new GLubyte[WIDTH * HEIGHT * 3];
hittable_list* world = new hittable_list;
vec3 origin, horizontal, vertical, lower_left_corner;
camera cam;

void makePixel(int x, int y, GLubyte* pixels, int width, int height, color c, int samples)
{
  if (0 <= x && x < width && 0 <= y && y < height) {
    int position = (x + y * width) * 3;
    float scale = 1.0 / samples;
    pixels[position]     = static_cast<int>(clamp(c.x * scale, 0.0, 1.0) * 255);
    pixels[position + 1] = static_cast<int>(clamp(c.y * scale, 0.0, 1.0) * 255);
    pixels[position + 2] = static_cast<int>(clamp(c.z * scale, 0.0, 1.0) * 255);
  } else {
    printf("OUTSIDE PICTURE!");
  }
}


color ray_color(const ray& r, const hittable* world) {
    hit_record rec;
    if (world->hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = normalize(r.direction());
    auto t = 0.5*(unit_direction.y + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

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
  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 2000.0;

  world->move(vec3(sin(t)/10, 0 , 0), 0);

  // Scan
  for (int j = HEIGHT-1; j >= 0; --j) {
    for (int i = 0; i < WIDTH; ++i) {
      color pixel_color(0.0, 0.0, 0.0);
      for (int s = 0; s < SAMPLES; s++) {
        double u = (double)i + (rand() / (RAND_MAX + 1.0)) / (WIDTH-1);
        double v = (double)j + (rand() / (RAND_MAX + 1.0)) / (HEIGHT-1);
        ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world);
      }
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