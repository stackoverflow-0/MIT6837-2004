#include "raytracer.h"

void RayTracer::render(const char* output_file, const char *depth_file, const char * normal_file)
{
    Image image(width, height);
    image.SetAllPixels(scene->getBackgroundColor());
    Image depthMap(width, height);
    depthMap.SetAllPixels(Vec3f(0.0, 0.0, 0.0));
    Image normalMap(width, height);
    normalMap.SetAllPixels(Vec3f(0.0, 0.0, 0.0));

    Vec3f ambientColor = scene->getAmbientLight();

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            float x = float(i) / float(width);
            float y = float(j) / float(height);
            Ray ray = camera->generateRay(Vec2f(x, y));
            Hit hit(INFINITY, nullptr, Vec3f(0.0, 0.0, 0.0));
            bool flag = group->intersect(ray, hit, camera->getTMin());
            if (flag) {
                // image.SetPixel(i, j, hit.getMaterial()->getDiffuseColor());
                // do shade here
                Vec3f normal = hit.getNormal();
                Vec3f dir = ray.getDirection();
                if (normal.Dot3(dir) > 0) {
                    // printf("did shade back\n");
                    if (shade_back) {
                        normal = -1 * normal;
                    } else {
                        image.SetPixel(i, j, Vec3f(0.0, 0.0, 0.0));
                        continue;
                    }
                }
                Vec3f color(0.0, 0.0, 0.0);
                // color = ambient + diffuse 
                Vec3f diffuseColor = hit.getMaterial()->getDiffuseColor();
                Vec3f ambient = diffuseColor * ambientColor;
                color += ambient;

                for (int k = 0; k < scene->getNumLights(); k++) {
                    Light *light = scene->getLight(k);
                    Vec3f pos = hit.getIntersectionPoint();
                    Vec3f l, lightColor;
                    light->getIllumination(pos, l, lightColor);
                    float d = l.Dot3(normal);
                    if (d < 0)
                        d = 0;
                    Vec3f diffuse = d * diffuseColor * lightColor;
                    color += diffuse;
                }

                image.SetPixel(i, j, color);
                // 根据作业要求取绝对值 , 不取绝对值也行
                normalMap.SetPixel(i, j, Vec3f(fabs(normal.x()), fabs(normal.y()), fabs(normal.z())));

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
    if (normal_file != nullptr) 
        normalMap.SaveTGA(normal_file);
}