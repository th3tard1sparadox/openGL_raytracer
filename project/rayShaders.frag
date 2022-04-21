#version 150

in vec4 gl_FragCoord;

out vec4 out_Color;

uniform vec4 sphere1;
uniform vec3 light1;

struct Camera
{
    vec3 origin;
    vec3 lower_left;
    vec3 horizontal;
    vec3 vertical;
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
    bool front_face;

    inline void set_face_normal(struct Ray t, vec3 & outward_normal) {
        front_face = dot(r.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

struct Ray get_ray(double x, double y, struct Cam)
{
    struct Ray r;
    r.origin = Cam.origin;
    r.direction = Cam.lower_left + Cam.horizontal * x + Cam.vertical * y - Cam.origin;
    return r;
}

double length_squared(vec3 a)
{
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

vec3 at(struct Ray r, double t)
{
    return r.origin + t * r.direction;
}

bool hit(struct Ray r, struct Sphere sphere, double max_t, double min_t, struct HitRecord & hit_r)
{
    vec3 oc = r.origin - sphere.position;
    double a = length_squared(r.direction);
    double b_half = dot(oc, r.direction);
    double c = length_squared(oc) - sphere.radius * sphere.radius;

    double discriminant = b_half * b_half - a * c;
    if(discriminant < 0) return false;
    double sqrt_disc = sqrt(discriminant);

    double root = (-b_half - sqrt_disc) / a;
    if(root < min_t || root > max_t) {
        root = (-b_half + sqrt_disc) / a;
        if(root < min_t || root > max_t)
            return false;
    }

    hit_r.t = root;
    hit_r.point = at(r, root);
    hit_r.normal = (hit_r.point - sphere.position) / sphere.radius;

    return true;
}

void main(void)
{
    out_Color = vec4(1.0, 0.0, 0.0, 1.0);
}
