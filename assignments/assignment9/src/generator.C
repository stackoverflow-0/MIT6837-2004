#include "generator.h"
#include "GL/freeglut.h"

int HoseGenerator::numNewParticles(float current_time, float dt) const
{
    return dt * desiredNumParticles / lifeSpan;
}

Particle *HoseGenerator::Generate(float current_time, int i)
{
    Vec3f p = position + positionRandomness * random->randomVector();
    Vec3f v = velocity + velocityRandomness * random->randomVector();
    Vec3f color = particleColor + colorRandomness * random->randomVector();
    Vec3f ddcolor = particleDeadColor + colorRandomness * random->randomVector();
    float m = particleMass + massRandomness * random->next();
    float lfspan = lifeSpan + lifeSpanRandomness * random->next();
    Particle *particle = new Particle(p, v, color, ddcolor, m, lfspan);
    return particle;
}

int RingGenerator::numNewParticles(float current_time, float dt) const
{
    return (current_time < 3.0f ? current_time / 3.0f : 1.0f) * desiredNumParticles;
}

Particle *RingGenerator::Generate(float current_time, int i)
{
    float radius = current_time < 3.0f ? current_time / 3.0f : 1.0f;
    Vec3f p = Vec3f(radius * cos(i) + 0.2f * positionRandomness * random->next(),
                    -4.0f,
                    radius * sin(i) + 0.2f * positionRandomness * random->next());
    Vec3f v = velocity + velocityRandomness * random->randomVector();
    Vec3f color = particleColor + colorRandomness * random->randomVector();
    Vec3f ddcolor = particleDeadColor + colorRandomness * random->randomVector();
    float m = particleMass + massRandomness * random->next();
    float lfspan = lifeSpan + lifeSpanRandomness * random->next();
    Particle *particle = new Particle(p, v, color, ddcolor, m, lfspan);
    return particle;
}