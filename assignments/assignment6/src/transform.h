#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "object3d.h"
#include "matrix.h"
class Transform : public Object3D
{
public:
    Transform(Matrix &m, Object3D *obj) : mat(m), object(obj){
        // 使用变换物体的绑定箱
        // cout << "init transform\n";
        mtl = obj->getMaterial();
        bdBox = object->getBoundingBox();
    };
    ~Transform(){};

    bool intersect(const Ray &r, Hit &h, float tmin);

    virtual void insertIntoGrid(Grid *g, Matrix *m);

    virtual BoundingBox *getBoundingBox();

    virtual bool is_transform() { return true; }

    void paint() const;

private:
    Matrix mat;
    Object3D *object;
};

#endif