#include "raytracer.h"

void RayTracer::render(const char* output_file, const char *depth_file)
{
    Image image(width, height);
    image.SetAllPixels(scene->getBackgroundColor());
    Image depthMap(width, height);
    depthMap.SetAllPixels(Vec3f(0.0, 0.0, 0.0));

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            float x = float(i) / float(width);
            float y = float(j) / float_t(height);
            Ray ray = camera->generateRay(Vec2f(x, y));
            Hit hit(INFINITY, nullptr);
            bool flag = group->intersect(ray, hit, camera->getTMin());
            if (flag) {
                image.SetPixel(i, j, hit.getMaterial()->getDiffuseColor());
                float t = hit.getT();
                if (t > depth_max) t = depth_max;
                if (t < depth_min) t = depth_min;
                t = (depth_max - t) / (depth_max - depth_min);
                depthMap.SetPixel(i, j, Vec3f(t, t, t));
            }
        }
    }
    if (output_file != nullptr)
        image.SaveTGA(output_file);
    if (depth_file != nullptr)
        depthMap.SaveTGA(depth_file);
}