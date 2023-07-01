#ifndef _RAYTRACER_H_

#define _RAYTRACER_H_

#include "scene_parser.h"
#include "image.h"
#include "camera.h"
#include "group.h"
#include "light.h"
#include "rayTree.h"
#include "math.h"
#include "grid.h"

extern bool shadows;
extern bool shade_back;
extern bool visualize_grid;
extern int nx, ny, nz;

class RayTracer
{
public:
    RayTracer(SceneParser *s, int max_bounces, float cutoff_weight)
        : scene(s), max_bounces(max_bounces), cutoff_weight(cutoff_weight)
    {
        if (nx)
        {
            grid = new Grid(scene->getGroup()->getBoundingBox(), nx, ny, nz);
            scene->getGroup()->insertIntoGrid(grid, nullptr);
        }
    };

    ~RayTracer()
    {
    };

    Grid *getGrid() { return grid; }
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, Hit &hit) const;

private:
    SceneParser *scene;
    int max_bounces;
    float cutoff_weight;
    bool shadows;
    Grid *grid{nullptr};
};

#endif