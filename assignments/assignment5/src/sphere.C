#include "sphere.h"
#include "grid.h"
#include "vectors.h"
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
            normal.push_back( -1 * pos);
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


void Sphere::insertIntoGrid(Grid *g, Matrix *m) {
    if (m) {
        Object3D::insertIntoGrid(g, m);
        return;
    }
    // no trans matrix - 0,0,0
    BoundingBox *grid_bb = g->getBoundingBox();
    assert(grid_bb != nullptr);
    Vec3f grid_min = grid_bb->getMin();
    Vec3f grid_max = grid_bb->getMax();
    int nx = g->nx;
    int ny = g->ny;
    int nz = g->nz;
    float cellx = (grid_max - grid_min).x() / nx;
    float celly = (grid_max - grid_min).y() / ny;
    float cellz = (grid_max - grid_min).z() / nz;
    float diagonal = sqrt(cellx * cellx + celly * celly + cellz * cellz);
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                float x = (i + 0.5) * cellx;
                float y = (j + 0.5) * celly;
                float z = (k + 0.5) * cellz;
                if ((Vec3f(x, y, z) - (center - grid_min)).Length() < radius + diagonal / 2) {
                    int index = nx * ny * k + nx * j + i;
                    g->opaque[index].push_back(this);
                }
            }
        }
    }
}