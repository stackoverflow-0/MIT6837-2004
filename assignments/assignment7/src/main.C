#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "raytracer.h"
#include "film.h"
#include "sampler.h"
#include "filter.h"

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

bool stats = false;

char *render_sample_file = nullptr;
int sample_zoom = 1;

SamplerType samplerType = SamplerType::UniformSamplerType;
int sample_num = 1;

FilterType filtertype = FilterType::BoxFilterType;
// for gaussian , filter_radius means sigma
float filter_radius = 0;

char *render_filter_file = nullptr;
int zoom_factor = 1;

RayTracer *rayTracer = nullptr;

SceneParser *scene = nullptr;

int main(int argc, char **argv)
{
    parser(argc, argv);

    scene = new SceneParser(input_file);
    Camera *camera = scene->getCamera();
    Object3D *group = scene->getGroup();

    rayTracer = new RayTracer(scene, max_bounces, cutoff_weight);

    if (gui)
    {
        Grid *grid = nullptr;
        if (nx != 0)
        {
            grid = new Grid(scene->getGroup()->getBoundingBox(), nx, ny, nz);
            scene->getGroup()->insertIntoGrid(grid, nullptr);
        }
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
    rayTracer->traceRay(ray, tmin, 0, 1.0, hit);

    Hit gridhit(INFINITY, nullptr, Vec3f(0, 0, 0));
    rayTracer->getGrid()->intersect(ray, gridhit, tmin);
}

void render(void)
{
    Image image(width, height);
    image.SetAllPixels(scene->getBackgroundColor());
    Camera *camera = scene->getCamera();

    Sampler *sampler = nullptr;

    switch (samplerType)
    {
    case SamplerType::UniformSamplerType:
        /* code */
        sampler = new UniormSampler(sample_num);
        break;
    case SamplerType::RandomSamplerType:
        /* code */
        sampler = new RandomSampler(sample_num);
        break;
    case SamplerType::JitterdSamplerType:
        /* code */
        sampler = new JitterdSampler(sample_num);
        break;
    default:
        break;
    }

    Filter *filter;

    switch (filtertype)
    {
    case FilterType::BoxFilterType:
        filter = new BoxFilter(filter_radius);
        break;
    case FilterType::TentFilterType:
        filter = new TentFilter(filter_radius);
        break;
    case FilterType::GaussianFliterType:
        filter = new GaussianFilter(filter_radius);
        break;
    default:
        break;
    }

    Film * film = new Film(width, height, sample_num);

    // RayTracer rayTracer(scene, max_bounces, cutoff_weight);
    if (nx != 0)
        RayTracingStats::Initialize(width, height, rayTracer->getGrid()->getBoundingBox(), nx, ny, nz);
    else
        RayTracingStats::Initialize(width, height, nullptr, 0, 0, 0);

    float frac = width / height;

    for (int i = 0; i < width; ++i)
    {
        if (i % 10 == 0)
        {
            printf("sample at %.2f %% ... \n", float(i) / float(width) * 100);
        }
        for (int j = 0; j < height; ++j)
        {
            for (int s = 0; s < film->getNumSamples(); ++s)
            {
                RayTracingStats::IncrementNumNonShadowRays();
                Vec2f random_offset = sampler->getSamplePosition(s);
                float x = float(i + random_offset.x()) / float(width);
                float y = (float(j + random_offset.y()) / float(height) + 1 - 1 / frac) / frac;
                Ray ray = camera->generateRay(Vec2f(x, y));
                float tmin = 0.001f;
                Hit hit(INFINITY, nullptr, Vec3f(0, 0, 0));
                Vec3f color = rayTracer->traceRay(ray, tmin, 0, 1.0, hit);
                film->setSample(i, j, s, random_offset, color);
                // image.SetPixel(i, j, film->getSample(x,y,s).getColor());
            }
        }
    }

    for (int i = 0; i < width; ++i)
    {
        if (i % 10 == 0)
        {
            printf("filter at %.2f %% ... \n", float(i) / float(width) * 100);
        }
        for (int j = 0; j < height; ++j)
        {
            image.SetPixel(i, j, filter->getColor(i, j, film));
            // cout << image.GetPixel(i, j) << endl;
        }
    }

    if (render_sample_file != nullptr)
    {
        film->renderSamples(render_sample_file, sample_zoom);
    }

    if (render_filter_file != nullptr)
    {
        film->renderFilter(render_filter_file, zoom_factor, filter);
    }

    if (output_file != nullptr)
    {
        image.SaveTGA(output_file);
    }

    if (stats)
        RayTracingStats::PrintStatistics();

    delete sampler;
    delete filter;
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
        else if (!strcmp(argv[i], "-stats"))
        {
            stats = true;
        }
        else if (!strcmp(argv[i], "-render_samples"))
        {
            i++;
            assert(i < argc);
            render_sample_file = argv[i];
            i++;
            assert(i < argc);
            sample_zoom = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-random_samples"))
        {
            i++;
            assert(i < argc);
            samplerType = SamplerType::RandomSamplerType;
            sample_num = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-uniform_samples"))
        {
            i++;
            assert(i < argc);
            samplerType = SamplerType::UniformSamplerType;
            sample_num = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-jittered_samples"))
        {
            i++;
            assert(i < argc);
            samplerType = SamplerType::JitterdSamplerType;
            sample_num = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-render_filter"))
        {
            i++;
            assert(i < argc);
            render_filter_file = argv[i];
            i++;
            assert(i < argc);
            zoom_factor = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-box_filter"))
        {
            i++;
            assert(i < argc);
            filtertype = FilterType::BoxFilterType;
            filter_radius = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-tent_filter"))
        {
            i++;
            assert(i < argc);
            filtertype = FilterType::TentFilterType;
            filter_radius = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-gaussian_filter"))
        {
            i++;
            assert(i < argc);
            filtertype = FilterType::GaussianFliterType;
            filter_radius = atof(argv[i]);
        }
    }
}