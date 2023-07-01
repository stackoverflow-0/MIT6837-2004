#ifndef _SPHERE_H_

#define _SPHERE_H_

#include "object3d.h"

extern int theta_steps;
extern int phi_steps;
extern bool gouraud;
const float PI = 3.14159265358979323846;

class Sphere : public Object3D
{
public:
    Sphere(Vec3f c, float r, Material *m) : center(c), radius(r)
    {
        mtl = m;
        bdBox = new BoundingBox(center - Vec3f(radius, radius, radius),
                                center + Vec3f(radius, radius, radius));
    };
    bool intersect(const Ray &r, Hit &h, float tmin);
    void insertIntoGrid(Grid *g, Matrix *m);
    void paint() const;

private:
    Vec3f center;
    float radius;
};

#endif