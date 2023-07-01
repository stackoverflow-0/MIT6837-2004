#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "raytracer.h"

void render();

char *input_file = nullptr;
int width = 100;
int height = 100;
char *output_file = nullptr;
float depth_min = 0;
float depth_max = 1;
char *depth_file = nullptr;
char *normal_file = nullptr;
bool shade_back = false;
bool gui = false;

int theta_steps = 0;
int phi_steps = 0;
bool gouraud = false;

SceneParser *scene = nullptr;

RayTracer *rt;

int main(int argc, char **argv)
{

    // sample command line:
    // raytracer -input scene1_01.txt -size 200 200 -output output1_01.tga -depth 9 10 depth1_01.tga

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
    }

    scene = new SceneParser(input_file);
    Camera *camera = scene->getCamera();
    Object3D *group = scene->getGroup();
    rt = new RayTracer(scene, camera, group, input_file, width, height, depth_min, depth_max, shade_back);
    if (gui)
    {
        glutInit(&argc, argv);
        GLCanvas glCanvas;
        printf("enter loop\n");
        glCanvas.initialize(scene, render);
        printf("over\n");
        return 0;
    }
    
    rt->render(output_file, depth_file, normal_file);
}

void render()
{
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // scene->getGroup()->paint();
    rt->render(output_file, depth_file, normal_file);
}