#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "ray.h"
#include "hit.h"
#include "material.h"
#include "boundingbox.h"
#include "assert.h"
#include "vectors.h"
#include "matrix.h"

class Grid;

class Object3D
{
public:
    Object3D() : mtl(nullptr), bdBox(nullptr){};

    virtual ~Object3D(){};

    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    
    virtual void paint() const = 0;

    virtual void insertIntoGrid(Grid *g, Matrix *m);

    virtual BoundingBox *getBoundingBox() { return bdBox; };
    virtual Material *getMaterial() const { return mtl; }

    virtual bool is_triangle() { return false; }

    virtual BoundingBox *getTriangleBoundingBox(const Matrix *m) const { return nullptr; }

    Material *mtl;
    BoundingBox *bdBox;
};



#endif