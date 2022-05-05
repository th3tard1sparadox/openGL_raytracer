#version 150

in vec4 gl_FragCoord;
in vec2 texCoord;

out vec4 out_Color;

uniform vec4 spheres[3];
uniform vec3 lights[4];

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

struct Triangle
{
    vec3 p1;
    vec3 p2;
    vec3 p3;
}

struct Plane
{
  vec3 point;
  vec3 normal;
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

vec3 cal_normal(in Triangle t)
{
    vec3 dir = cross((t.p2 - t.p1), (t.p3, t.p1));
    return normalize(dir);
}

bool within_triangle(in vec3 hit_point, in Triangle triangle)
{
    Triangle t1;
    t1.p1 = triangle.p1;
    t1.p2 = triangle.p2;
    t1.p3 = hit_point;
    Triangle t2;
    t2.p1 = triangle.p2;
    t2.p2 = triangle.p3;
    t2.p3 = hit_point;
    Triangle t3;
    t3.p1 = triangle.p1;
    t3.p2 = hit_point;
    t3.p3 = triangle.p3;
    vec3 norm1 = cal_normal(t1);
    vec3 norm2 = cal_normal(t2);
    vec3 norm3 = cal_normal(t3);
    return (norm1 == norm2 && norm2 = norm3);
}

bool hit_plane(in Ray r, in Plane plane, inout vec3 point) {
    float denom = dot(plane.normal, ray.direction);
    if (denom > 0.0000001) {
        float t = dot((plane.point - ray.origin), plane.normal) / denom;
        if (t >= 0) {
            point = at(r, t);
            return true;
        }
    }
    return false;
}

Plane calculate_plane(in Triangle t)
{
    Plane p;
    p.point = t.p1;
    p.normal = cal_normal(t);
    return p;
}

bool hit_triangle(in Ray r, in Triangle triangle, inout HitRecord hit_r) {
    Plane plane = calculate_plane(triangle);
    vec3 hit_point = vec3(0,0,0);
    if (hit_plane(r, plane, hit_point)) {
        if (within_triangle(hit_point, triangle)) {
            hit_r.t = t;
            hit_r.point = hit_point;
            if (length_squared(ray.direction + plane.normal) > length_squared(ray.direction)) {
                hit_r.front_face = false;
                hit_r.normal = -plane.normal;
            } else {
                hit_r.front_face = true;
                hit_r.normal = plane.normal;
            }
            return true;
        }
    }
    return false;
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

  HitRecord hit_r;
  vec3 color = vec3(0.0, 0.0, 0.0);
  for(int i = 0; i < 3; i++){
    Sphere s = Sphere(vec3(spheres[i].r, spheres[i].g, spheres[i].b), spheres[i].a);

    if (hit(r, s, 0.001, INF, hit_r) ){
      if(hit_r.front_face){
        Ray shadow_ray;
        vec3 light_col, light;
        for(int j = 0; j < 2; j++){
          light = lights[j*2];
          light_col = lights[(j*2) + 1];

          shadow_ray = get_shadow_ray(hit_r.point, light);
          float angle = dot(hit_r.normal, shadow_ray.direction);
          float len = get_ray_length(shadow_ray);
          color += angle * light_col * (1/(len));
        }
      }
    }
  }


  out_Color = vec4(color, 1.0);
}
