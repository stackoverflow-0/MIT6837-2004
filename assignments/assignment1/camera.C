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
