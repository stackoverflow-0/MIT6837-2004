#ifndef _RAYTRACER_H_

#define _RAYTRACER_H_

#include "scene_parser.h"
#include "image.h"
#include "camera.h"
#include "group.h"

class RayTracer
{
public:
    RayTracer(const char *input_path, int w, int h, int dmin, int dmax) : width(w), height(h), depth_min(dmin), depth_max(dmax)
    {
        scene = new SceneParser(input_path);
        camera = scene->getCamera();
        group = scene->getGroup();
    }
    ~RayTracer()
    {
        delete scene;
    }
    void render(const char* output_file, const char *depth_file);

private:
    SceneParser *scene;
    Camera *camera;
    Group *group;
    int width = 100;
    int height = 100;
    float depth_min = 0;
    float depth_max = 1;
};

#endif