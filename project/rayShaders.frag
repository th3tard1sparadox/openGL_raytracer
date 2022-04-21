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

struct Ray get_ray(double x, double y, struct Cam)
{
    struct Ray r;
    r.origin = Cam.origin;
    r.direction = Cam.lower_left + Cam.horizontal * x + Cam.vertical * y - Cam.origin;
    return r;
}

void main(void)
{
    out_Color = vec4(1.0, 0.0, 0.0, 1.0);
}
