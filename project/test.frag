#version 150

// Planes
// Triangles
// Spheres
// Lights

in vec4 gl_FragCoord;
in vec2 texCoord;

out vec4 out_Color;

uniform mat2x3 planes[10];
uniform mat3 triangles_[10];
uniform vec4 spheres[10];
uniform vec3 lights[10];

uniform vec3 colors[40];

uniform int planesN;
uniform int triangles_N;
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

struct Plane
{
    vec3 point;
    vec3 normal;
};

struct Triangle
{
    vec3 p1;
    vec3 p2;
    vec3 p3;
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

vec3 get_triangle_normal(in Triangle t)
{
    return -normalize(cross((t.p2 - t.p1), (t.p3 - t.p1)));
}

Plane get_plane(in Triangle t)
{
    Plane p;
    p.point = t.p1;
    p.normal = get_triangle_normal(t);
    return p;
}

float triangle_area(Triangle t)
{
    return length(cross((t.p2 - t.p1), (t.p3 - t.p1))) / 2;
}

bool within_triangle(in vec3 p, in Triangle t)
{
    Triangle t1;
    t1.p1 = t.p1;
    t1.p2 = t.p2;
    t1.p3 = p;

    Triangle t2;
    t2.p1 = t.p2;
    t2.p2 = t.p3;
    t2.p3 = p;

    Triangle t3;
    t3.p1 = t.p1;
    t3.p2 = p;
    t3.p3 = t.p3;

    float A = triangle_area(t);
    float A1 = triangle_area(t1);
    float A2 = triangle_area(t2);
    float A3 = triangle_area(t3);

    float alpha = A1 / A;
    float beta = A2 / A;
    float gamma = A3 / A;

    if(alpha >= 0.0 && alpha <= 1.0 &&
       beta >= 0.0 && beta <= 1.0 &&
       gamma >= 0.0 && gamma <= 1.0 &&
       alpha + beta + gamma - 1 < 0.00001)
    {
        return true;
    }

    return false;
}

bool hit_plane(in Ray r, in Plane p, in float max_t, in float min_t, inout HitRecord h)
{
    float denom = dot(p.normal, r.direction);
    if(abs(denom) > min_t) {
        float t = dot((p.point - r.origin), p.normal) / denom;
        if(t > min_t) {
            h.point = ray_at(r, t);
            h.normal = p.normal;
            h.t = t;
            h.front_face = true;
            if(length(r.direction + p.normal) > length(r.direction))
            {
                h.front_face = false;
                h.normal = - p.normal;
            }

            return true;
        }
    }

    return false;
}

bool hit_triangle(in Ray r, in Triangle t, in float max_t, in float min_t, inout HitRecord h)
{
    Plane p = get_plane(t);
    if(hit_plane(r, p, max_t, min_t, h))
    {
        if(within_triangle(h.point, t))
        {
            return true;
        }
    }

    return false;
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
    HitRecord hits[30];
    int t_index = -1;
    vec3 color = vec3(0.0, 0.0, 0.0);

    // planes
    for(int i = 0; i < planesN; i++)
    {
        Plane p = Plane(planes[i][0], planes[i][1]);

        if(hit_plane(r, p, max_t, min_t, hits[i]))
        {
            if(length(hits[i].point) < t_smallest)
            {
                t_smallest = length(hits[i].point);
                t_index = i;
            }
        }
    }

    // triangles
    for(int i = 0; i < triangles_N; i++)
    {
        Triangle t = Triangle(triangles_[i][0], triangles_[i][1], triangles_[i][2]);

        if(hit_triangle(r, t, max_t, min_t, hits[i]))
        {
            if(length(hits[i].point) < t_smallest)
            {
                t_smallest = length(hits[i].point);
                t_index = i;
            }
        }
    }

    // spheres
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

    // lights
    if(t_index != -1)
    {
        for(int i = 0; i < lightN; i++)
        {
            Light l = Light(lights[i], colors[planesN + triangles_N + spheresN - 1 + i]);

            Ray shadow_ray = get_shadow_ray(hits[t_index].point, l);

            int next_j = 0;
            HitRecord hit_r;
            // planes
            for(int j = 0; j < planesN; j++)
            {
                Plane p = Plane(planes[j][0], planes[j][1]);
                if(!hit_plane(shadow_ray, p, max_t, min_t, hit_r))
                {
                    float angle = dot(hits[t_index].normal, shadow_ray.direction);
                    float len = length(l.position - shadow_ray.origin);
                    color += angle * l.color * (1/(len*len));
                }
                next_j++;
            }
            // triangles
            for(int j = 0; j < triangles_N; j++)
            {
                Triangle t = Triangle(triangles_[j][0], triangles_[j][1], triangles_[j][2]);
                if(!hit_triangle(shadow_ray, t, max_t, min_t, hit_r))
                {
                    float angle = dot(hits[t_index].normal, shadow_ray.direction);
                    float len = length(l.position - shadow_ray.origin);
                    color += angle * l.color * (1/(len*len));
                }
                next_j++;
            }
            // spheres
            for(int j = 0; j < spheresN; j++)
            {
                Sphere s = Sphere(vec3(spheres[j].r, spheres[j].g, spheres[j].b), spheres[j].a);
                if(!hit_sphere(shadow_ray, s, max_t, min_t, hit_r))
                {
                    float angle = dot(hits[t_index].normal, shadow_ray.direction);
                    float len = length(l.position - shadow_ray.origin);
                    color += angle * l.color * (1/(len*len));
                }
                next_j++;
            }
        }
    }

    out_Color = vec4(color, 1.0);
}
