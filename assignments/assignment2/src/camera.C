#include "camera.h"

Ray OrthographicCamera::generateRay(Vec2f point)
{
    Vec3f orig = center + (point.x() - 0.5) * horizontal * size + (point.y() - 0.5) * up * size ;
    Ray ray(orig, direction);
    return ray;
}

float OrthographicCamera::getTMin() const
{
    return -INFINITY;
}


Ray PerspectiveCamera::generateRay(Vec2f point) {
    float d = 0.5 / tan(angle / 2);
    Vec3f v = d * direction + (point.x() - 0.5) * horizontal + (point.y() - 0.5) * up;
    v.Normalize();
    Ray ray(center, v);
    return ray;
}

float PerspectiveCamera::getTMin() const {
    return 0;
}