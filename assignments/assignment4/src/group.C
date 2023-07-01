#include "group.h"

bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool is_intersect = false;
    for (int i = 0; i < num_obj; i++) {
        if (objects[i]->intersect(r, h, tmin)) {
            is_intersect = true;
        }
    }
    return is_intersect;
}

bool Group::intersectShadowRay(const Ray & r, Hit & h, float tmin)
{
    for (int i = 0; i < num_obj; i++) {
        // 找到一个相交即表示hit point和光源之间存在遮挡
        if (objects[i]->intersect(r, h, tmin)) {
            return true;
        }
    }
    return false;
}

void Group::addObject(int index, Object3D *obj)
{
    assert(index >= 0 && index < num_obj);
    objects[index] = obj;
}

void Group::paint() const {
    for (int i = 0; i < num_obj; ++i) {
        objects[i]->paint();
    }
}
