#version 150

in vec4 gl_FragCoord;
in vec2 texCoord;

out vec4 out_Color;

uniform vec4 spheres[4];
uniform vec3 lights[6];

uniform int spheresN;
uniform int lightN;

// structs
struct Ray
{
    vec3 origin;
    vec3 direction;
};

struct Sphere
{
    vec3 center;
    float radius;
};

struct HitRecord
{
    vec3 point;
    vec3 normal;
    float t;
    bool front_face;
};

struct Light
{
    vec3 position;
    vec3 color;
};

// functions
Ray get_camera_ray(in float x, in float y, in float focal_length)
{
    Ray r;
    r.origin = vec3(0.0, 0.0, 0.0);
    r.direction = normalize(vec3(x, y, focal_length));
    return r;
}

vec3 ray_at(in Ray r, in float t)
{
    return r.origin + t * r.direction;
}

bool hit_sphere(in Ray r, in Sphere s, in float max_t, in float min_t, inout HitRecord h)
{
    // math
    vec3 L = s.center - r.origin;
    float tc = dot(L, r.direction);
    if(tc < min_t)
    {
        return false;
    }
    float d2 = dot(L, L) - tc*tc;

    if(d2 > s.radius*s.radius)
    {
        return false;
    }

    float t1c = sqrt(s.radius*s.radius - d2);

    float t1 = tc - t1c;
    float t2 = tc + t1c;

    // selecting t
    float t = t1;
    if(t1 > t2)
    {
        t = t2;
    }

    if(t < min_t)
    {
        t = t2;
        if (t < min_t){
            return false;
        }
    }

    // recording the hit
    h.point = ray_at(r, t);
    h.normal = normalize(h.point - s.center);
    h.t = t;
    h.front_face = true;
    if(t == t2)
    {
        h.front_face = false;
        h.normal = - h.normal;
    }

    return true;
}

Ray get_shadow_ray(in vec3 origin, in Light l)
{
    Ray r;
    r.origin = origin;
    r.direction = normalize(l.position - origin);
    return r;
}

void main(void)
{
    const float max_t = 60000;
    const float min_t = 0.001;
    const float focal_length = 2.0;

    Ray r = get_camera_ray(texCoord.x, texCoord.y, focal_length);

    float t_smallest = max_t;
    HitRecord hits[10];
    int t_index = -1;
    vec3 color = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < spheresN; i++)
    {
        Sphere s = Sphere(vec3(spheres[i].r, spheres[i].g, spheres[i].b), spheres[i].a);

        if(hit_sphere(r, s, max_t, min_t, hits[i]))
        {
            if(length(hits[i].point) < t_smallest)
            {
                t_smallest = length(hits[i].point);
                t_index = i;
            }
        }
    }

    if(t_index != -1)
    {
        for(int i = 0; i < lightN; i++)
        {
            Light l = Light(lights[i*2], lights[(i*2) + 1]);

            Ray shadow_ray = get_shadow_ray(hits[t_index].point, l);

            for(int j = 0; j < spheresN; j++)
            {
                Sphere s = Sphere(vec3(spheres[j].r, spheres[j].g, spheres[j].b), spheres[j].a);
                HitRecord hit_r;
                if(!hit_sphere(shadow_ray, s, max_t, min_t, hit_r))
                {
                    // calculate color here
                    float angle = dot(hits[t_index].normal, shadow_ray.direction);
                    float len = length(l.position - shadow_ray.origin);
                    color += angle * l.color * (1/(len*len));
                } else {
                    //color = vec3(0.5);
                }
            }
        }
    }

    out_Color = vec4(color, 1.0);
}
