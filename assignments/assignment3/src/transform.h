#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "object3d.h"
#include "matrix.h"
class Transform : public Object3D
{
public:
    Transform(Matrix &m, Object3D *obj) : mat(m), object(obj){};
    ~Transform(){};
    bool intersect(const Ray &r, Hit &h, float tmin);
    void paint() const;

private:
    Matrix mat;
    Object3D *object;
};

#endif