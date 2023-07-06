#include "sphere.h"

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f orig = r.getOrigin() - center;
    Vec3f dir = r.getDirection();
    float a = dir.Dot3(dir);
    float b = 2 * orig.Dot3(dir);
    float c = orig.Dot3(orig) - radius * radius;
    float delta = b * b - 4 * a * c;
    float t = INFINITY;
    if (delta >= 0) {
        float d = sqrt(delta);
        float t1 = (-b - d) / (2 * a);
        float t2 = (-b + d) / (2 * a);
        if (t1 >= tmin) {
            t = t1;
        } else if (t2 >= tmin) {
            t = t2;
        }
        if (t < h.getT()) {
            Vec3f normal = orig + t * dir;
            normal.Normalize();
            h.set(t, mtl, r);
            return true;
        }
    }
    return false;
}