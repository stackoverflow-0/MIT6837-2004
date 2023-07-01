#ifndef _GROUP_H_
#define _GROUP_H_
#include "object3d.h"
class Group : public Object3D
{
public:
    Group(int n) : num_obj(n)
    {
        objects = new Object3D *[num_obj];
    }
    ~Group()
    {
        for (int i = 0; i < num_obj; i++)
        {
            delete objects[i];
        }
        delete[] objects;
    }
    bool intersect(const Ray &r, Hit &h, float tmin);
    void addObject(int index, Object3D *obj);

private:
    int num_obj{0};
    Object3D ** objects;
};

#endif