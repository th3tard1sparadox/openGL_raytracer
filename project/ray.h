#ifndef RAY_H
#define RAY_H


#include <math.h>
#include "GL_utilities.h"
#include "MicroGlut.h"

using namespace std;

using point3 = vec3;
using color = vec3;

class ray {
    public:
        ray() {}
        ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction)
        {}

        point3 origin() const  { return orig; }
        vec3 direction() const { return dir; }

        point3 at(double t) const {
            return orig + t*dir;
        }

    public:
        point3 orig;
        vec3 dir;
};

#endif
