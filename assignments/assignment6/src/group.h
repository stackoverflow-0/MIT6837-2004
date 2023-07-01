#ifndef _GROUP_H_
#define _GROUP_H_
#include "object3d.h"

class Group : public Object3D
{
public:
    Group(int n) : num_obj(n)
    {
        objects = new Object3D *[num_obj];
        bdBox = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY),
                                      Vec3f(-INFINITY, -INFINITY, -INFINITY));
    };
    ~Group()
    {
        for (int i = 0; i < num_obj; i++)
        {
            delete objects[i];
        }
        delete[] objects;
    };
    bool intersect(const Ray &r, Hit &h, float tmin);
    bool intersectShadowRay(const Ray &r, Hit &h, float tmin);
    void addObject(int index, Object3D *obj);
    void insertIntoGrid(Grid *g, Matrix *m);
    void paint() const;

private:
    int num_obj;
    Object3D **objects;
};

#endif