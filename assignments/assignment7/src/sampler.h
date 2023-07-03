#pragma once

#include "vectors.h"

enum SamplerType
{
    random,
    uniform,
    jitterd
};

class Sampler
{
public:
    Sampler(int sample_num) : sample_num(sample_num) {}

    virtual Vec2f getSamplePosition(int n) = 0;

protected:
    int sample_num{1};
};

class RandomSampler : public Sampler
{
public:
    RandomSampler(int sample_num) : Sampler(sample_num) {}

    virtual Vec2f getSamplePosition(int n)
    {
        return Vec2f(drand48(), drand48());
    }
};

class JitterdSampler : public Sampler
{
public:
    JitterdSampler(int sample_num) : Sampler(sample_num) {}

    virtual Vec2f getSamplePosition(int n)
    {
        int sample_side = sqrt(sample_num);
        int xoffset = n / sample_side;
        int yoffset = n % sample_side;
        return Vec2f(xoffset * sample_side + drand48() / float(sample_side),
                     yoffset * sample_side + drand48() / float(sample_side));
    }
};

class UniormSampler : public Sampler
{
public:
    UniormSampler(int sample_num) : Sampler(sample_num) {}

    virtual Vec2f getSamplePosition(int n)
    {
        int sample_side = sqrt(sample_num);
        int xoffset = n / sample_side;
        int yoffset = n % sample_side;
        return Vec2f(xoffset * sample_side,
                     yoffset * sample_side);
    }
};
