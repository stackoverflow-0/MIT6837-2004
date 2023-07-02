#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "float.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "matrix.h"
  

class Camera
{
public:
    Camera(){};
    virtual ~Camera(){};
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;
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
    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);
    virtual void dollyCamera(float dist);
    virtual void truckCamera(float dx, float dy);
    virtual void rotateCamera(float rx, float ry);

private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float size;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(Vec3f &c, Vec3f &d, Vec3f &u, float a) : center(c), direction(d), up(u), angle(a)
    {
        direction.Normalize();
        Vec3f::Cross3(horizontal, direction, up);
        horizontal.Normalize();

        Vec3f::Cross3(up, horizontal, direction);
        up.Normalize();
    }
    ~PerspectiveCamera() {}

    Ray generateRay(Vec2f point);

    float getTMin() const;
    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);
    virtual void dollyCamera(float dist);
    virtual void truckCamera(float dx, float dy);
    virtual void rotateCamera(float rx, float ry);

private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float angle;
};

#endif