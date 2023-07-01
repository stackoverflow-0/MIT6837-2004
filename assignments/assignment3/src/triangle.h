#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"

class Triangle : public Object3D {
    public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) : ma(a), mb(b), mc(c), mtl(m) {
        Vec3f::Cross3(normal, a - b, a - c);
        normal.Normalize();
    };
    bool intersect(const Ray & r, Hit & h, float tmin);
    void paint() const;

    private:
    Vec3f ma;
    Vec3f mb;
    Vec3f mc;
    Vec3f normal;
    Material * mtl;

};


#endif