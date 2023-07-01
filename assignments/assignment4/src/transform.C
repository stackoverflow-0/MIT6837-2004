#include "transform.h"

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f orig = r.getOrigin();
    Vec3f dir = r.getDirection();
    Matrix invM = mat;
    int can_inv = invM.Inverse();
    if (can_inv)
    {
        invM.Transform(orig);
        invM.TransformDirection(dir);
        Ray invR(orig, dir);
        if (object->intersect(invR, h, tmin))
        {
            Vec3f normal = h.getNormal();
            Matrix invMT = invM;

            invMT.Transpose();
            // 计算变换后的法向量
            invMT.TransformDirection(normal);
            normal.Normalize();
            h.set(h.getT(), h.getMaterial(), normal, r);
            return true;
        }
    }
    return false;
}

void Transform::paint() const
{
    glPushMatrix();
    GLfloat *glMatrix = mat.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
    object->paint();
    glPopMatrix();
}
