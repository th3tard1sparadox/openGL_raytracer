#version 150

in vec2 resolution;
in vec4 gl_FragCoord;
in vec2 texCoord;

out vec4 out_Color;

uniform vec4 sphere1;
uniform vec3 light1;
uniform vec3 light2;

struct Camera
{
  vec3 origin;
  vec3 lower_left;
  vec3 horizontal;
  vec3 vertical;
  float FL;
};

struct Ray
{
  vec3 origin;
  vec3 direction;
};

struct Sphere
{
  vec3 position;
  float radius;
};

struct HitRecord
{
  vec3 point;
  vec3 normal;
  float t;
  bool front_face;
};

void set_face_normal(inout Ray r, inout vec3 outward_normal, inout HitRecord h) {
  h.front_face = dot(r.direction, outward_normal) < 0;
  h.normal = h.front_face ? outward_normal : -outward_normal;
}

Ray get_ray(in float x, in float y, in Camera c)
{
  Ray r;
  r.origin = c.origin;
  r.direction = vec3(x, y, c.FL);
  return r;
}

Ray get_shadow_ray(vec3 src, vec3 dest){
  return Ray(src, src-dest);
}

float length_squared(in vec3 a)
{
  return a.x * a.x + a.y * a.y + a.z * a.z;
}

float get_ray_length(in Ray r){
  return sqrt(r.direction.x * r.direction.x + r.direction.y * r.direction.y + r.direction.z * r.direction.z);
}

vec3 at(in Ray r, in float t)
{
  return r.origin + t * r.direction;
}

bool hit(in Ray r, in Sphere sphere, in float max_t, in float min_t, inout HitRecord hit_r)
{
  vec3 oc = r.origin - sphere.position;
  float a = dot(r.direction, r.direction);
  float b_half = dot(oc, r.direction);
  float c = dot(oc, oc) - sphere.radius * sphere.radius;

  float discriminant = b_half * b_half - a * c;
  if(discriminant < 0) {
    return false;
  }
  float sqrt_disc = sqrt(discriminant);

  float root = (-b_half + sqrt_disc) / a;
  if(root < min_t || root > max_t) {
    root = (-b_half - sqrt_disc) / a;
    // if(root < min_t || root > max_t)
    //   return false;
  }

  hit_r.t = root;
  hit_r.point = at(r, root);
  vec3 outward_normal = (hit_r.point - sphere.position) / sphere.radius;
  set_face_normal(r, outward_normal, hit_r);

  return true;
}

void main(void)
{
  const float INF = 60000.0;
  int max_depth = 3;

  float aspect_ratio = 1.0 / 1.0;
  float viewport_height = 1.0;
  float viewport_width = aspect_ratio * viewport_height;
  float focal_length = -0.5;

  vec3 ori = vec3(0,0,0);
  vec3 hor = vec3(viewport_width, 0.0, 0.0);
  vec3 vert = vec3(0.0, viewport_height, 0.0);
  vec3 LLC = ori - hor/2 - vert/2 - vec3(0, 0, focal_length);

  Camera cam = Camera(ori, hor, vert, LLC, focal_length);
  Ray r = get_ray(texCoord.x, texCoord.y, cam);

  // vec3 unit_direction = normalize(r.direction);
  // float t = 0.5*(unit_direction.y + 1.0);
  // vec3 color = (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);

  HitRecord hit_r;
  vec3 color = vec3(0.0, 0.0, 0.0);
  Sphere s = Sphere(vec3(sphere1.r, sphere1.g, sphere1.b), sphere1.a);
  vec3 light = light1;

  if (hit(r, s, 0.001, INF, hit_r) ){
    if(hit_r.front_face){
      Ray shadow_ray1 = get_shadow_ray(hit_r.point, light);
      float angle = dot(hit_r.normal, shadow_ray1.direction);
      float l1 = get_ray_length(shadow_ray1);
      // color += angle*vec3(1.0, 0.5, 1.0)*(1/l1);
      Ray shadow_ray2 = get_shadow_ray(hit_r.point, light2);
      float angle2 = dot(hit_r.normal, shadow_ray2.direction);
      float l2 = get_ray_length(shadow_ray2);
      color += angle2*vec3(0.0, 0.0, 1.0)*(1/(l2*l2));
    }

  }

  out_Color = vec4(color, 1.0);
}
