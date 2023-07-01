#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"

class Plane : public Object3D {
    public:
    Plane(Vec3f &n, float d, Material *m) : normal(n) , offset(d), mtl(m) {};
    bool intersect(const Ray & r, Hit & h, float tmin);
    virtual void insertIntoGrid(Grid *g, Matrix *m) { return; };
    void paint() const;
    private:
    Vec3f normal;
    float offset;
    Material * mtl;
};


#endif