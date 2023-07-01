#ifndef _SPHERE_H_

#define _SPHERE_H_

#include "object3d.h"

class Sphere : public Object3D
{
public:
    Sphere(Vec3f c, float r, Material * m) : center(c), radius(r){
        mtl = m;
    };
    bool intersect(const Ray &r, Hit &h, float tmin);
    void paint() const;
private:
    Vec3f center;
    float radius;
    // Material * mtl;
};

#endif