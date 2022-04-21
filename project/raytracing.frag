#version 150
out vec4 out_Color;
in float shading;
in ray in_ray;

uniform hittable* world;
uniform vec3 inColor;
void main(void)
{
    ray r = in_r;
    color col = color(0, 0, 0);
    int hit_count = 0;


    for(int i = 0; i < MAX_REC; i++) {
        hit_record rec;

        if (world->hit(r, 0.001, infinity, rec)) {
            point3 target = rec.p + rec.normal + normalize(random_in_unit_sphere());
            r = ray(rec.p, target - rec.p);
            hit_count += 1;
        } else {
            vec3 unit_direction = normalize(r.direction());
            vec3 t = 0.5*(unit_direction.y + 1.0);
            col = (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
            break;
        }
    }

    for(int i = 0; i < hit_count; i++) {
        col *= 0.5;
    }

    out_Color = vec4(col, 1.0);
}
