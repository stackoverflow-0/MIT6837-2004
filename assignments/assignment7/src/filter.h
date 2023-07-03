#pragma once

#include "vectors.h"
#include "film.h"

class Filter
{
public:
    Filter() {}
    virtual ~Filter() {}
    //
    Vec3f getColor(int i, int j, Film *film);

    virtual float getWeight(float x, float y) = 0;

    virtual int getSupportRadius() = 0;

    // private:
    //     int supportRadius{0};
};

class BoxFilter : public Filter
{
public:
    BoxFilter(int radius) : radius(radius)
    {
        int side = radius << 1 + 1;
        total_weight = float(side * side);
    }

    virtual float getWeight(float x, float y);

    virtual int getSupportRadius() { return radius; }

private:
    int radius{0};
    float total_weight{0};
};

class TentFilter : public Filter
{
public:
    TentFilter(int radius) : radius(radius)
    {
        for (int i = -radius; i <= radius; ++i)
        {
            for (int j = -radius; j <= radius; ++j)
            {
                float x = float(i) / float(radius);
                float y = float(j) / float(radius);
                total_weight += 1.0f - max(fabs(x), fabs(y) / float(radius));
            }
        }
    }

    virtual float getWeight(float x, float y);

    virtual int getSupportRadius() { return radius; }

private:
    int radius{0};
    float total_weight{0};
};

class GaussianFilter : public Filter
{
public:
    GaussianFilter(float sigma) : sigma(sigma)
    {
        radius = int(2 * sigma);
        for (int i = -radius; i <= radius; ++i)
        {
            for (int j = -radius; j <= radius; ++j)
            {
                float x = float(i) / float(radius);
                float y = float(j) / float(radius);
                total_weight += expf(-(x * x + y * y) / (2 * sigma));
            }
        }
    }

    virtual float getWeight(float x, float y);

    virtual int getSupportRadius() { return radius; }

private:
    float sigma{1.0f};
    int radius {0};
    float total_weight {0};
};