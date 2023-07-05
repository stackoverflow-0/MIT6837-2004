#pragma once

#include "vectors.h"
#include "matrix.h"
#include "perlin_noise.h"

class ForceField
{
public:
    ForceField() {}

    virtual ~ForceField() {}

    virtual Vec3f getAcceleration(const Vec3f &position,
                                  float mass, float t) const = 0;
};

class RadialForceField : public ForceField
{

public:
    RadialForceField(float magnitude) : magnitude(magnitude) {}

    virtual Vec3f getAcceleration(const Vec3f &position,
                                  float mass, float t) const
    {
        return (-magnitude / mass) * position;
    }
    

private:
    float magnitude{0};
};

class GravityForceField : public ForceField
{

public:
    GravityForceField(Vec3f &g) : gravity(g) {}

    virtual Vec3f getAcceleration(const Vec3f &position,
                                  float mass, float t) const
    {
        return gravity;
    }

private:
    Vec3f gravity;
};

class ConstantForceField : public ForceField
{

public:
    ConstantForceField(Vec3f &force) : force(force) {}

    virtual Vec3f getAcceleration(const Vec3f &position,
                                  float mass, float t) const
    {
        return force * (1.0f / mass);
    }

private:
    Vec3f force;
};

class VerticalForceField : public ForceField
{

public:
    VerticalForceField(float magnitude) : magnitude(magnitude) {}

    virtual Vec3f getAcceleration(const Vec3f &position,
                                  float mass, float t) const
    {
        return magnitude * Vec3f(0, position.y() > 0 ? -1 : 1, 0);
    }

private:
    float magnitude;
};

class PerlinForceField : public ForceField
{

public:
    PerlinForceField(Matrix *mat,
                     ForceField *fd1, ForceField *fd2, int octaves) : mat(mat), fd1(fd1), fd2(fd2), octaves(octaves) {}

    virtual Vec3f getAcceleration(const Vec3f &position,
                                  float mass, float t) const
    {
        float N = getN(position);
        return fd1->getAcceleration(position, mass, t) * N + fd2->getAcceleration(position, mass, t) * (1 - N);
    }

    float getN(Vec3f p) const
    {
        mat->Transform(p);
        float N = 0;
        int t = 1;
        for (int i = 0; i < octaves; ++i)
        {
            N += PerlinNoise::noise(t * p.x(), t * p.y(), t * p.z()) / float(t);
            t = t << 1;
        }
        N = N < 0 ? 0 : N;
        N = N > 1 ? 1 : N;
        return N;
    }

private:
    ForceField *fd1{nullptr};
    ForceField *fd2{nullptr};
    Matrix *mat;
    int octaves{0};
};

class WindForceField : public ForceField
{

public:
    WindForceField(float magnitude) : magnitude(magnitude) {}

    virtual Vec3f getAcceleration(const Vec3f &position,
                                  float mass, float t) const
    {
        Vec3f f =  Vec3f(sin(10 * t), fabs(cos(10 * t)), 0);
        return f * (magnitude / mass);
    }

private:
    float magnitude;
};