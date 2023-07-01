#include "triangle.h"

bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f orig = r.getOrigin();
    Vec3f dir = r.getDirection();
    Vec3f e1(mb,ma);
    Vec3f e2(mc,ma);
    Vec3f pvec;
    Vec3f::Cross3(pvec, dir, e2);
    float det = e1.Dot3(pvec);

    if (det > -0.00001 && det < 0.00001)
		return(false);

    float invDet = 1 / det;
    Vec3f tvec(orig, ma);
    float u = invDet * tvec.Dot3(pvec);

    if (u < 0.0 || u > 1.0)
		return(false);

    Vec3f qvec;
    Vec3f::Cross3(qvec, tvec, e1);
    float v = invDet * dir.Dot3(qvec);

    if (v < 0.0 || u + v > 1.0)
		return(false);

    float t = invDet * e2.Dot3(qvec);

    if (t > tmin && t < h.getT()) {
        h.set(t, mtl, normal, r);
        return true;
    }

    return false;
}