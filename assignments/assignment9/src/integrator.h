#pragma once

#include "vectors.h"
#include "particle.h"
#include "forcefield.h"
class Integrator
{
public:
    Integrator() {}

    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;

    virtual Vec3f getColor() const = 0;

protected:
};

class EulerIntegrator : public Integrator
{
public:
    EulerIntegrator() {}
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt)
    {

        Vec3f position = particle->getPosition();
        Vec3f velocity = particle->getVelocity();
        Vec3f acceleration = forcefield->getAcceleration(position, particle->getMass(), t);
        position += velocity * dt;
        velocity += acceleration * dt;

        particle->setPosition(position);
        particle->setVelocity(velocity);
        particle->increaseAge(dt);
    }
    virtual Vec3f getColor() const
    {
        return Vec3f(1, 0, 0);
    }
};

class MidpointIntegrator : public Integrator
{
public:
    MidpointIntegrator() {}
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt)
    {

        Vec3f position_n = particle->getPosition();
        Vec3f velocity_n = particle->getVelocity();
        Vec3f acceleration = forcefield->getAcceleration(position_n, particle->getMass(), t);

        Vec3f position_m = position_n + velocity_n * (dt * 0.5f);
        Vec3f velocity_m = velocity_n + acceleration * (dt * 0.5f);
        Vec3f acceleration_m = forcefield->getAcceleration(position_m, particle->getMass(), t + dt * 0.5f);

        particle->setPosition(position_n + velocity_m * dt);
        particle->setVelocity(velocity_n + acceleration_m * dt);
        particle->increaseAge(dt);
    }
    virtual Vec3f getColor() const
    {
        return Vec3f(0, 1, 0);
    }
};

class RungeKuttaIntegrator : public Integrator
{
public:
    RungeKuttaIntegrator() {}
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt)
    {
        float m = particle->getMass();
        Vec3f p_n = particle->getPosition();
        Vec3f v_n = particle->getVelocity();
        Vec3f a_n = forcefield->getAcceleration(p_n, m, t);

        Vec3f ka = Vec3f(0, 0, 0), kv = Vec3f(0, 0, 0);

        Vec3f p_1 = p_n;
        Vec3f v_1 = v_n;
        Vec3f a_1 = forcefield->getAcceleration(p_1, m, t + dt);

        ka += a_1;
        kv += v_1;

        Vec3f p_2 = p_n + v_1 * (dt * 0.5f);
        Vec3f v_2 = v_n + a_1 * (dt * 0.5f);
        Vec3f a_2 = forcefield->getAcceleration(p_2, m, t + dt * 0.5f);

        ka += 2 * a_2;
        kv += 2 * v_2;

        Vec3f p_3 = p_n + v_2 * (dt * 0.5f);
        Vec3f v_3 = v_n + a_2 * (dt * 0.5f);
        Vec3f a_3 = forcefield->getAcceleration(p_3, m, t + dt * 0.5f);

        ka += 2 * a_3;
        kv += 2 * v_3;

        Vec3f p_4 = p_n + v_3 * dt;
        Vec3f v_4 = v_n + a_3 * dt;
        Vec3f a_4 = forcefield->getAcceleration(p_4, m, t + dt);

        ka += a_4;
        kv += v_4;

        particle->setPosition(p_n + kv * (dt / 6.0f));
        particle->setVelocity(v_n + ka * (dt / 6.0f));
        particle->increaseAge(dt);
    }

    virtual Vec3f getColor() const
    {
        return Vec3f(0, 0, 1);
    }
};