#include "transform.h"

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f ro = r.getOrigin();
    Vec3f rd = r.getDirection();
    Matrix invM = mat;
    if (invM.Inverse()) {
        invM.Transform(ro);
        invM.TransformDirection(rd);
        //rd.Normalize();
        Ray ray_local(ro, rd);
        if (object->intersect(ray_local, h, tmin)) {
            Vec3f normal = h.getNormal();
            Matrix normalMatrix = invM;
            normalMatrix.Transpose();
            normalMatrix.TransformDirection(normal); //trans normal to world space
            normal.Normalize();
            h.set(h.getT(), h.getMaterial(), normal, r);
            return true;
        }
    }
    return false;
}

void Transform::insertIntoGrid(Grid *g, Matrix *m)
{
    // cout << "add sth\n" ;
    if(m){
        Matrix t =  (*m) * mat;
        object->insertIntoGrid(g, &t);
    }
    else{
        object->insertIntoGrid(g, &mat);
    }
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

BoundingBox *Transform::getBoundingBox() {
    // cout << "get transform bd box\n";
    assert(bdBox != nullptr);
    if (object->is_triangle()) {
        return object->getTriangleBoundingBox(&mat);
    }
    Vec3f obj_min = bdBox->getMin();
    Vec3f obj_max = bdBox->getMax();
    float x1 = obj_min.x(), y1 = obj_min.y(), z1 = obj_min.z();
    float x2 = obj_max.x(), y2 = obj_max.y(), z2 = obj_max.z();

    Vec3f v1 = Vec3f(x1, y1, z1);
    Vec3f v2 = Vec3f(x1, y1, z2);
    Vec3f v3 = Vec3f(x1, y2, z1);
    Vec3f v4 = Vec3f(x1, y2, z2);
    Vec3f v5 = Vec3f(x2, y1, z1);
    Vec3f v6 = Vec3f(x2, y1, z2);
    Vec3f v7 = Vec3f(x2, y2, z1);
    Vec3f v8 = Vec3f(x2, y2, z2);

    mat.Transform(v1);
    mat.Transform(v2);
    mat.Transform(v3);
    mat.Transform(v4);
    mat.Transform(v5);
    mat.Transform(v6);
    mat.Transform(v7);
    mat.Transform(v8);

    BoundingBox *bb_new = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY), Vec3f(-INFINITY, -INFINITY, -INFINITY));
    
    bb_new->Extend(v1);
    bb_new->Extend(v2);
    bb_new->Extend(v3);
    bb_new->Extend(v4);
    bb_new->Extend(v5);
    bb_new->Extend(v6);
    bb_new->Extend(v7);
    bb_new->Extend(v8);
    // bb_new->Print();
    return bb_new;
}
