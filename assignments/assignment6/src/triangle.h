#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"

class Triangle : public Object3D
{
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) : ma(a), mb(b), mc(c), mtl(m)
    {
        Vec3f::Cross3(normal, a - b, a - c);
        normal.Normalize();

        bdBox = new BoundingBox(Vec3f(min(a.x(), b.x()), min(a.y(), b.y()), min(a.z(), b.z())),
                                      Vec3f(max(a.x(), b.x()), max(a.y(), b.y()), max(a.z(), b.z())));
        bdBox->Extend(c);
    };
    bool intersect(const Ray &r, Hit &h, float tmin);
    void insertIntoGrid(Grid *g, Matrix *m);
    BoundingBox *getTriangleBoundingBox(const Matrix *m) const;
    void paint() const;

    virtual bool is_triangle() { return true; }

private:
    Vec3f ma;
    Vec3f mb;
    Vec3f mc;
    Vec3f normal;
    Material *mtl;
};

#endif