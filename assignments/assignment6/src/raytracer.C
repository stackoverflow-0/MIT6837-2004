#include "raytracer.h"

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, Hit &hit) const
{
    // 渲染体素
    // if(visualize_grid){
    //     if (grid->intersect(ray, hit, tmin)) {
    //         Vec3f color(0.0, 0.0, 0.0);
    //         Vec3f pos = hit.getIntersectionPoint();
    //         color += scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
    //         for (int i = 0; i < scene->getNumLights(); i++) {
    //             Light *light = scene->getLight(i);
    //             Vec3f lightDir, lightColor;
    //             float dis;
    //             light->getIllumination(pos, lightDir, lightColor, dis);
    //             color += hit.getMaterial()->Shade(ray, hit, lightDir, lightColor);
    //         }
    //         return color;
    //     } else {
    //         return scene->getBackgroundColor();
    //     }
    // }

    if (grid)
    {
        return RayCastFast(ray, tmin, bounces, weight, hit);
    }
    else
    {
        return RayCast(ray, tmin, bounces, weight, hit);
    }
}

Vec3f RayTracer::RayCast(Ray &ray, float tmin, int bounces, float weight, Hit &hit) const
{

    Group *group = scene->getGroup();
    // 命中物体
    // 遍历所有物体 , 找到光线相交
    if (group->intersect(ray, hit, tmin))
    {
        // printf("rend rt\n");
        if (bounces == 0)
        {
            RayTree::SetMainSegment(ray, tmin, hit.getT());
        }
        Vec3f color(0, 0, 0);
        Vec3f pos = hit.getIntersectionPoint();
        Material *mtl = hit.getMaterial();
        // 环境光
        color += scene->getAmbientLight() * mtl->getDiffuseColor();

        // 光照
        int num_lights = scene->getNumLights();
        for (int i = 0; i < num_lights; i++)
        {
            Light *light = scene->getLight(i);
            Vec3f lightDir;
            Vec3f lightColor;
            float dis;
            // lightDir 表示从hit point到光源的方向向量
            light->getIllumination(pos, lightDir, lightColor, dis);

            Ray rshadow(pos, lightDir);
            Hit hshadow(dis, nullptr, Vec3f(0, 0, 0));
            // 阴影
            if (shadows)
            {
                RayTracingStats::IncrementNumShadowRays();
                if (!group->intersectShadowRay(rshadow, hshadow, tmin))
                    color += mtl->Shade(ray, hit, lightDir, lightColor);
            }
            else
                color += mtl->Shade(ray, hit, lightDir, lightColor);

            RayTree::AddShadowSegment(rshadow, tmin, hshadow.getT());
        }
        Ray scattered;
        Vec3f attenuation;
        // 跟踪光线的反射成分
        if (bounces < max_bounces && weight > cutoff_weight && mtl->reflect(ray, hit, attenuation, scattered))
        {
            Hit hit_ref(INFINITY, nullptr, Vec3f(0, 0, 0));
            color += attenuation * traceRay(scattered, tmin, bounces + 1, weight * attenuation.Length(), hit_ref);
            RayTree::AddReflectedSegment(scattered, tmin, hit_ref.getT()); // reflect---------------------------------
        }
        // 跟踪光线的折射成分
        if (bounces < max_bounces && weight > cutoff_weight && mtl->refract(ray, hit, attenuation, scattered))
        {
            Hit hit_ref(INFINITY, nullptr, Vec3f(0, 0, 0));
            color += attenuation * traceRay(scattered, tmin, bounces + 1, weight * attenuation.Length(), hit_ref);
            RayTree::AddTransmittedSegment(scattered, tmin, hit_ref.getT()); // trans---------------------------------
        }
        return color;
    }
    // 背景
    else
    {
        return scene->getBackgroundColor();
    }
}

Vec3f RayTracer::RayCastFast(Ray &ray, float tmin, int bounces, float weight, Hit &hit) const
{
    // 使用体素加速找到光线相交
    if (grid->intersect(ray, hit, tmin))
    {
        Vec3f color(0.0, 0.0, 0.0);
        Vec3f pos = hit.getIntersectionPoint();
        Material *material = hit.getMaterial();

        color += scene->getAmbientLight() * material->getDiffuseColor();
        for (int i = 0; i < scene->getNumLights(); i++)
        {
            Light *light = scene->getLight(i);
            Vec3f lightDir, lightColor;
            float dis;
            light->getIllumination(pos, lightDir, lightColor, dis);

            if (shadows)
            {
                Ray rshadow(pos, lightDir);
                Hit hshadow(dis, nullptr, Vec3f(0, 0, 0));
                RayTracingStats::IncrementNumShadowRays();
                // 使用体素加速光线相交检测
                if (!grid->intersectShadowRay(rshadow, hshadow, tmin))
                {
                    // cout << "shadow hit\n";
                    color += material->Shade(ray, hit, lightDir, lightColor);
                }
            }
            else
                color += material->Shade(ray, hit, lightDir, lightColor);
            // RayTree::AddShadowSegment(rshadow, tmin, hshadow.getT());
        }
        // reflective
        Ray scattered;
        Vec3f attenuation;
        if (bounces < max_bounces && weight > cutoff_weight && material->reflect(ray, hit, attenuation, scattered))
        {
            Hit hit_ref(INFINITY, nullptr, Vec3f(0, 0, 0));
            color += attenuation * RayCastFast(scattered, tmin, bounces + 1, weight * attenuation.Length(), hit_ref);
            RayTree::AddReflectedSegment(scattered, tmin, hit_ref.getT());
        }
        // refraction transparent
        if (bounces < max_bounces && weight > cutoff_weight && material->refract(ray, hit, attenuation, scattered))
        {
            Hit hit_ref(INFINITY, nullptr, Vec3f(0, 0, 0));
            color += attenuation * RayCastFast(scattered, tmin, bounces + 1, weight * attenuation.Length(), hit_ref);
            RayTree::AddTransmittedSegment(scattered, tmin, hit_ref.getT());
        }
        return color;
    }
    else
    {
        return scene->getBackgroundColor();
    }
}
