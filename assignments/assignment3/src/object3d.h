#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "ray.h"
#include "hit.h"
#include "material.h"

class Object3D
{
public:
    Object3D() : mtl(nullptr){};
    virtual ~Object3D(){};
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() const = 0;
    Material *mtl;
};

#endif