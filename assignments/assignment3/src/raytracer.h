#ifndef _RAYTRACER_H_

#define _RAYTRACER_H_

#include "scene_parser.h"
#include "image.h"
#include "camera.h"
#include "group.h"
#include "light.h"
class RayTracer
{
public:
    RayTracer(SceneParser * sce, Camera *cam, Object3D *group,
              const char *input_path, int w, int h, int dmin, int dmax, bool sb)
        : scene(sce), camera(cam), group(group), width(w), height(h), depth_min(dmin), depth_max(dmax), shade_back(sb)
    {
    };
    ~RayTracer()
    {
        // delete scene;
    };
    void render(const char *output_file, const char *depth_file, const char *normal_file);

private:
    SceneParser *scene;
    Camera *camera;
    Object3D *group;
    int width = 100;
    int height = 100;
    float depth_min = 0;
    float depth_max = 1;
    bool shade_back;
};

#endif