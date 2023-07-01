#include "sphere.h"

bool Sphere::intersect(const Ray & r, Hit & h, float tmin)
{
    Vec3f orig2center = center - r.getOrigin();
    Vec3f dir = r.getDirection();
    float l = orig2center.Dot3(dir);
    float oc = orig2center.Dot3(orig2center);
    float minDis = oc - l * l;

    if(minDis > radius * radius) {
        return false;
    }
    float t;
    if (orig2center.Length() - radius > 0) {
        t = l - sqrt(radius * radius - minDis);
    }
    else {
        t = l + sqrt(radius * radius - minDis);
    }
    
    if (t > tmin && t < h.getT()) {
        Vec3f normal = t * dir - orig2center;
        normal.Normalize();
        h.set(t, mtl, normal, r);
        return true;
    }
    return false;
}