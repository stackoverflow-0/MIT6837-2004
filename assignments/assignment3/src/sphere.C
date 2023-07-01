#include "sphere.h"
#include <vector>
bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f orig2center = center - r.getOrigin();
    Vec3f dir = r.getDirection();
    float l = orig2center.Dot3(dir) / dir.Length();
    float oc = orig2center.Dot3(orig2center);
    float minDis = oc - l * l;

    if (minDis > radius * radius)
    {
        return false;
    }
    float t;
    if (orig2center.Length() - radius > 0)
    {
        t = (l - sqrt(radius * radius - minDis)) / dir.Length();
    }
    else
    {
        t = (l + sqrt(radius * radius - minDis)) / dir.Length();
    }

    if (t > tmin && t < h.getT())
    {
        Vec3f normal = t * dir - orig2center;
        normal.Normalize();
        h.set(t, mtl, normal, r);
        return true;
    }
    return false;
}

extern int theta_steps;
extern int phi_steps;
extern bool gouraud;
const float PI = 3.14159265358979323846;

void Sphere::paint() const
{
    mtl->glSetMaterial();
    float dt = 2 * PI / theta_steps;
    float dp = PI / phi_steps;
    std::vector<Vec3f> position;
    std::vector<Vec3f> normal;
    // initialize all the position for points
    for (int p = 0; p <= phi_steps; p++)
    {
        for (int t = 0; t < theta_steps; t++)
        {
            float theta = t * dt;
            float phi = p * dp;
            Vec3f pos = Vec3f(sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta));
            position.push_back(center + pos * radius);
            normal.push_back(pos);
        }
    }
    glBegin(GL_QUADS);
    for (int phi = 0; phi < phi_steps; phi++)
    {
        for (int theta = 0; theta < theta_steps; theta++)
        {
            int index[4] = {
                phi * theta_steps + theta,
                (phi + 1) * theta_steps + theta,
                (phi + 1) * theta_steps + (theta + 1) % theta_steps,
                phi * theta_steps + (theta + 1) % theta_steps,
            };

            Vec3f n;
            if (!gouraud)
            {
                Vec3f::Cross3(n, position[index[0]] - position[index[1]], position[index[2]] - position[index[1]]);
                glNormal3f(n.x(), n.y(), n.z());
            }
            for (int i = 0; i < 4; i++)
            {
                if (gouraud)
                {
                    glNormal3f(normal[index[i]].x(), normal[index[i]].y(), normal[index[i]].z());
                }
                glVertex3f(position[index[i]].x(), position[index[i]].y(), position[index[i]].z());
            }
        }
    }
    glEnd();
}
