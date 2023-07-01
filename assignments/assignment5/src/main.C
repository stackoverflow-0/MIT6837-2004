#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "raytracer.h"

void render(void);
void RayTraceRender(float x, float y);
void parser(int argc, char **argv);

char *input_file = nullptr;
int width = 100;
int height = 100;
char *output_file = nullptr;
float depth_min = 0;
float depth_max = 1;
char *depth_file = nullptr;
char *normal_file = nullptr;
bool shade_back = false;
bool shadows = false;
bool gui = false;

int max_bounces = 2;
float cutoff_weight = 1.0;

int theta_steps = 0;
int phi_steps = 0;
bool gouraud = false;

bool visualize_grid = false;
int nx = 0, ny = 0, nz = 0;

RayTracer * rayTracer = nullptr;

SceneParser *scene = nullptr;

int main(int argc, char **argv)
{
    parser(argc, argv);

    scene = new SceneParser(input_file);
    Camera *camera = scene->getCamera();
    Object3D *group = scene->getGroup();

    Grid *grid = nullptr;
    if (nx != 0)
    {
        grid = new Grid(scene->getGroup()->getBoundingBox(), nx, ny, nz);
        scene->getGroup()->insertIntoGrid(grid, nullptr);
    }
    rayTracer = new RayTracer(scene, max_bounces, cutoff_weight);

    if (gui)
    {
        glutInit(&argc, argv);
        GLCanvas glCanvas;
        printf("enter loop\n");
        glCanvas.initialize(scene, render, RayTraceRender, grid, visualize_grid);
        printf("over\n");
    }
    else
    {
        render();
    }
    delete rayTracer;
    return 0;
}

void RayTraceRender(float x, float y)
{
    Ray ray = scene->getCamera()->generateRay(Vec2f(x, y));
    // RayTracer rayTracer(scene, max_bounces, cutoff_weight);
    float tmin = 0.001f;
    Hit hit(INFINITY, nullptr, Vec3f(0, 0, 0));
    Vec3f color = rayTracer->traceRay(ray, tmin, 0, 1.0, hit);

    Hit gridhit(INFINITY, nullptr, Vec3f(0, 0, 0));
    rayTracer->getGrid()->intersect(ray, gridhit, tmin);
}

void render(void)
{
    Image image(width, height);
    image.SetAllPixels(scene->getBackgroundColor());
    Camera *camera = scene->getCamera();

    // RayTracer rayTracer(scene, max_bounces, cutoff_weight);

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            float x = float(i) / float(width);
            float y = float(j) / float(height);
            Ray ray = camera->generateRay(Vec2f(x, y));
            float tmin = 0.001f;
            Hit hit(INFINITY, nullptr, Vec3f(0, 0, 0));
            Vec3f color = rayTracer->traceRay(ray, tmin, 0, 1.0, hit);
            image.SetPixel(i, j, color);
            // printf("here at %d %d\n", i, j);
        }
        
    }
    // printf("here\n");
    if (output_file != nullptr)
        image.SaveTGA(output_file);
}

void parser(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-input"))
        {
            i++;
            assert(i < argc);
            input_file = argv[i];
        }
        else if (!strcmp(argv[i], "-size"))
        {
            i++;
            assert(i < argc);
            width = atoi(argv[i]);
            i++;
            assert(i < argc);
            height = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-output"))
        {
            i++;
            assert(i < argc);
            output_file = argv[i];
        }
        else if (!strcmp(argv[i], "-depth"))
        {
            i++;
            assert(i < argc);
            depth_min = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_max = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_file = argv[i];
        }
        else if (!strcmp(argv[i], "-normals"))
        {
            i++;
            assert(i < argc);
            normal_file = argv[i];
        }
        else if (!strcmp(argv[i], "-shade_back"))
        {
            shade_back = true;
        }
        else if (!strcmp(argv[i], "-shadows"))
        {
            shadows = true;
        }
        else if (!strcmp(argv[i], "-gui"))
        {
            gui = true;
        }
        else if (!strcmp(argv[i], "-tessellation"))
        {
            i++;
            assert(i < argc);
            theta_steps = atoi(argv[i]);
            i++;
            assert(i < argc);
            phi_steps = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-gouraud"))
        {
            gouraud = true;
        }
        else if (!strcmp(argv[i], "-bounces"))
        {
            i++;
            assert(i < argc);
            max_bounces = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-weight"))
        {
            i++;
            assert(i < argc);
            cutoff_weight = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-grid"))
        {
            i++;
            assert(i < argc);
            nx = atoi(argv[i]);
            i++;
            assert(i < argc);
            ny = atoi(argv[i]);
            i++;
            assert(i < argc);
            nz = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-visualize_grid"))
        {
            visualize_grid = true;
        }
    }
}