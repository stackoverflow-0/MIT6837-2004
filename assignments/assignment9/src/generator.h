#pragma once

#include "matrix.h"

#include "vectors.h"

#include "random.h"

#include "particle.h"

class Generator
{
public:
    Generator() { random = new Random(); }

    virtual ~Generator() { delete random; }

    void SetColors(Vec3f color, Vec3f dead_color, float color_randomness)
    {
        particleColor = color;
        particleDeadColor = dead_color;
        colorRandomness = color_randomness;
    }
    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles)
    {
        lifeSpan = lifespan;
        lifeSpanRandomness = lifespan_randomness;
        desiredNumParticles = desired_num_particles;
    }
    void SetMass(float mass, float mass_randomness)
    {
        particleMass = mass;
        massRandomness = mass_randomness;
    }

    // on each timestep, create some particles
    virtual int numNewParticles(float current_time, float dt) const = 0;
    virtual Particle *Generate(float current_time, int i) = 0;

    // for the gui
    virtual void Paint() const {};
    void Restart() {}

protected:
    Random *random;
    Vec3f particleColor;
    Vec3f particleDeadColor;
    float colorRandomness;
    float lifeSpan;
    float lifeSpanRandomness;
    int desiredNumParticles;
    float particleMass;
    float massRandomness;
};

class HoseGenerator : public Generator
{
public:
    HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness) : position(position), positionRandomness(position_randomness), velocity(velocity), velocityRandomness(velocity_randomness) {}

    virtual int numNewParticles(float current_time, float dt) const;
    virtual Particle *Generate(float current_time, int i);

private:
    Vec3f position;
    float positionRandomness;
    Vec3f velocity;
    float velocityRandomness;
};

class RingGenerator : public Generator
{
public:
    RingGenerator(float position_randomness, Vec3f velocity, float velocityRandomness) : positionRandomness(position_randomness), velocity(velocity), velocityRandomness(velocityRandomness) {}

    virtual int numNewParticles(float current_time, float dt) const;
    virtual Particle *Generate(float current_time, int i);

private:
    float positionRandomness;
    Vec3f velocity;
    float velocityRandomness;
};