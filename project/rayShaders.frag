#version 150

in vec4 gl_FragCoord;
in vec2 texCoord;

out vec4 out_Color;

uniform vec4 spheres[4];
uniform vec3 lights[4];

uniform int spheresN;
uniform int lightN;

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

Ray get_ray(in float x, in float y, in float FL)
{
  Ray r;
  r.origin = vec3(0.0, 0.0, 0.0);
  r.direction = normalize(vec3(x, y, FL));
  return r;
}

Ray get_shadow_ray(vec3 src, vec3 dest){
  return Ray(src, dest-src);
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
  // vec3 oc = r.origin - sphere.position;
  // float a = dot(r.direction, r.direction);
  // float b_half = dot(oc, r.direction);
  // float c = dot(oc, oc) - sphere.radius * sphere.radius;

  vec3 L = sphere.position - r.origin;
  float tca = dot(L, r.direction);
  float d2 = dot(L, L) - tca*tca;

  if(d2 > sphere.radius * sphere.radius){return false;}

  float thc = sqrt(sphere.radius * sphere.radius - d2);
  float t0 = tca - thc;
  float t1 = tca + thc;

  float t = t0;
  hit_r.front_face = true;
  if(t0 > t1){
    hit_r.front_face = false;
    t = t1;
  }

  if(t < min_t){
    hit_r.front_face = false;
    t = t1;
    if (t < min_t){
      return false;
    }
  }

  hit_r.t = t;
  hit_r.point = at(r, t);
  hit_r.normal = normalize(hit_r.point - sphere.position);
  if(!hit_r.front_face){
    hit_r.normal = -hit_r.normal;
  }

  return true;

  // float discriminant = b_half * b_half - a * c;
  // if(discriminant < 0) {
  //   return false;
  // }
  // float sqrt_disc = sqrt(discriminant);

  // float root = (-b_half + sqrt_disc) / a;
  // if(root < min_t || root > max_t) {
  //   root = (-b_half - sqrt_disc) / a;
  //   if(root < min_t || root > max_t){
  //       // return false;
  //   }
  // }

  // hit_r.t = root;
  // hit_r.point = at(r, root);
  // vec3 outward_normal = (hit_r.point - sphere.position) / sphere.radius;
  // set_face_normal(r, outward_normal, hit_r);

  // return true;
}

void main(void)
{
  const float INF = 60000;
  const float focal_length = 2.0;
  int max_depth = 3;

  Ray r = get_ray(texCoord.x, texCoord.y, focal_length);

  HitRecord hit_r;
  vec3 color = vec3(0.0, 0.0, 0.0);
  for(int i = 0; i < spheresN; i++){
    Sphere s = Sphere(vec3(spheres[i].r, spheres[i].g, spheres[i].b), spheres[i].a);

    if (hit(r, s, INF, 0.001, hit_r) ){
      // color = vec3(1.0);
      if(hit_r.front_face){
        Ray shadow_ray;
        vec3 light_col, light;
        for(int j = 0; j < 2; j++){
          light = lights[j*2];
          light_col = lights[(j*2) + 1];

          shadow_ray = get_shadow_ray(hit_r.point, light);
          for(int k = 0; k < spheresN; k++){
            Sphere ss = Sphere(vec3(spheres[k].r, spheres[k].g, spheres[k].b), spheres[k].a);

            if (!hit(shadow_ray, ss, INF, 0.001, hit_r)){
              float angle = dot(hit_r.normal, shadow_ray.direction);
              float len = get_ray_length(shadow_ray);
              color += angle * light_col * (1/(len));
              // color = vec3(1);
            }
          }
        }
      }
    }
  }


  out_Color = vec4(color, 1.0);
}
