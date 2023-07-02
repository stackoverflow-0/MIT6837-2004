#pragma once

#include "object3d.h"
#include "vectors.h"
#include "material.h"
#include "marchinfo.h"
#include <set>
#include <vector>

extern bool visualize_grid;

#define min3(a, b, c) (((a) < (b)) ? (((a) < (c)) ? (a) : (c)) : (((b) < (c)) ? (b) : (c)))
#define max3(a, b, c) (((a) > (b)) ? (((a) > (c)) ? (a) : (c)) : (((b) > (c)) ? (b) : (c)))

class Grid : public Object3D
{
public:
    Grid(BoundingBox *bb, int nx, int ny, int nz) : nx(nx), ny(ny), nz(nz)
    {
        bdBox = bb;
        opaque.resize(nx * ny * nz);

    };

    ~Grid(){};

    virtual bool intersect(const Ray &r, Hit &h, float tmin);

    virtual void paint() const;

    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;

    PhongMaterial* getMtl(int i) const;

    bool intersectShadowRay(const Ray &r, Hit &hit, float tmin);
    // float max3f(float a, float b, float c) const { return a > b ? (a > c ? a : c) : (b > c ? b : c); };
    // float min3f(float a, float b, float c) const { return a < b ? (a < c ? a : c) : (b < c ? b : c); };

    int nx, ny, nz;
    std::vector<std::vector<Object3D *>> opaque;
    std::set<Object3D *> is_intersected;
    std::vector<Object3D *> infinitePrimitives;
};