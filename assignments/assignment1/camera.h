#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "float.h"

class Camera
{
public:
    Camera(){};
    virtual ~Camera(){};
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s) : center(c), direction(d), up(u), size(s)
    {
        direction.Normalize();
        Vec3f::Cross3(horizontal, direction, up);
        horizontal.Normalize();

        Vec3f::Cross3(up, horizontal, direction);
        up.Normalize();
    }
    Ray generateRay(Vec2f point);
    float getTMin() const;

private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float size;
};

#endif