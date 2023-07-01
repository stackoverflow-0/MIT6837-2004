#include "plane.h"


bool Plane::intersect(const Ray & r, Hit & h, float tmin){
    Vec3f orig = r.getOrigin();
    Vec3f dir = r.getDirection();
    float dir_norm = normal.Dot3(dir);
    //平行时没有交点
    if (dir_norm == 0)
        return false;
        
    float t = (offset - normal.Dot3(orig)) / dir_norm;

    if (t > tmin && t < h.getT()) {
        h.set(t, mtl, normal, r);
        return true;
    }

    return false;
}