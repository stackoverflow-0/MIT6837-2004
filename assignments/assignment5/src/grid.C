#include "grid.h"

bool Grid::intersect(const Ray &r, Hit &hit, float tmin) {
    Vec3f minP = bdBox->getMin();
    Vec3f maxP = bdBox->getMax();
    float cellx = (maxP - minP).x() / float(nx);
    float celly = (maxP - minP).y() / float(ny);
    float cellz = (maxP - minP).z() / float(nz);

    MarchingInfo mi;
    initializeRayMarch(mi, r, tmin);

    //ray trace
    // printf("t_cur : %f vs hit t : %f\n", mi.t_cur, hit.getT());
    if (mi.t_cur <= hit.getT()) {
        // printf("enter\n");
        while (mi.i >= 0 && mi.j >= 0 && mi.k >= 0 && mi.i < nx && mi.j < ny && mi.k < nz) {
            int i = mi.i;
            int j = mi.j;
            int k = mi.k;
            int index = nx * ny * k + nx * j + i;
            if (!opaque[index].empty()) {
                PhongMaterial *m = getMtl(opaque[index].size());
                hit.set(mi.t_cur, m, mi.normal, r);
                // delete m;
                return true;
            }
            mi.nextCell();
        }
    }
    // printf("empty !\n");
    return false;
}

void Grid::paint() const {
    Vec3f minP = bdBox->getMin();
    Vec3f maxP = bdBox->getMax();
    float cellx = (maxP - minP).x() / nx;
    float celly = (maxP - minP).y() / ny;
    float cellz = (maxP - minP).z() / nz;
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                int index = nx * ny * k + nx * j + i;
                if (!opaque[index].empty()) {
                    Vec3f v0 = minP + Vec3f(i * cellx       , j * celly         , k * cellz      );
                    Vec3f v1 = minP + Vec3f((i + 1) * cellx , j * celly         , k * cellz      );
                    Vec3f v2 = minP + Vec3f((i + 1) * cellx , (j + 1) * celly   , k * cellz      );
                    Vec3f v3 = minP + Vec3f(i * cellx       , (j + 1) * celly   , k * cellz      );
                    Vec3f v4 = minP + Vec3f(i * cellx       , j * celly         , (k + 1) * cellz);
                    Vec3f v5 = minP + Vec3f((i + 1) * cellx , j * celly         , (k + 1) * cellz);
                    Vec3f v6 = minP + Vec3f((i + 1) * cellx , (j + 1) * celly   , (k + 1) * cellz);
                    Vec3f v7 = minP + Vec3f(i * cellx       , (j + 1) * celly   , (k + 1) * cellz);
                    PhongMaterial *m = getMtl(opaque[index].size());                    
                    m->glSetMaterial();
                    glBegin(GL_QUADS);
                    glNormal3f(0, 0, -1);
                    glVertex3f(v0.x(), v0.y(), v0.z());
                    glVertex3f(v1.x(), v1.y(), v1.z());
                    glVertex3f(v2.x(), v2.y(), v2.z());
                    glVertex3f(v3.x(), v3.y(), v3.z());
                    glNormal3f(0, 0, 1);
                    glVertex3f(v4.x(), v4.y(), v4.z());
                    glVertex3f(v5.x(), v5.y(), v5.z());
                    glVertex3f(v6.x(), v6.y(), v6.z());
                    glVertex3f(v7.x(), v7.y(), v7.z());
                    glNormal3f(1, 0, 0);
                    glVertex3f(v1.x(), v1.y(), v1.z());
                    glVertex3f(v2.x(), v2.y(), v2.z());
                    glVertex3f(v6.x(), v6.y(), v6.z());
                    glVertex3f(v5.x(), v5.y(), v5.z());
                    glNormal3f(-1, 0, 0);
                    glVertex3f(v0.x(), v0.y(), v0.z());
                    glVertex3f(v3.x(), v3.y(), v3.z());
                    glVertex3f(v7.x(), v7.y(), v7.z());
                    glVertex3f(v4.x(), v4.y(), v4.z());
                    glNormal3f(0, -1, 0);
                    glVertex3f(v0.x(), v0.y(), v0.z());
                    glVertex3f(v1.x(), v1.y(), v1.z());
                    glVertex3f(v5.x(), v5.y(), v5.z());
                    glVertex3f(v4.x(), v4.y(), v4.z());
                    glNormal3f(0, 1, 0);
                    glVertex3f(v2.x(), v2.y(), v2.z());
                    glVertex3f(v3.x(), v3.y(), v3.z());
                    glVertex3f(v7.x(), v7.y(), v7.z());
                    glVertex3f(v6.x(), v6.y(), v6.z());
                    glEnd();
                    // delete m;
                }
            }
        }
    }
}

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const {

    Vec3f orig = r.getOrigin();
    Vec3f dir = r.getDirection();

    Vec3f minP = bdBox->getMin();
    Vec3f maxP = bdBox->getMax();

    // cout << "min max P :" << minP << maxP << endl;
    float cellx = (maxP - minP).x() / nx;
    float celly = (maxP - minP).y() / ny;
    float cellz = (maxP - minP).z() / nz;

    // xyz方向每次的位移
    mi.dt_x = fabs(cellx / dir.x());
    mi.dt_y = fabs(celly / dir.y());
    mi.dt_z = fabs(cellz / dir.z());
    // ray行进方向
    mi.sign_x = dir.x() > 0 ? 1 : -1;
    mi.sign_y = dir.y() > 0 ? 1 : -1;
    mi.sign_z = dir.z() > 0 ? 1 : -1;

    float t_near, t_far;
    float t1_x = (minP - orig).x() / dir.x();
    float t2_x = (maxP - orig).x() / dir.x();
    if (dir.x() < 0)
        swap(t1_x, t2_x);
    float t1_y = (minP - orig).y() / dir.y();
    float t2_y = (maxP - orig).y() / dir.y();
    if (dir.y() < 0)
        swap(t1_y, t2_y);
    float t1_z = (minP - orig).z() / dir.z();
    float t2_z = (maxP - orig).z() / dir.z();
    if (dir.z() < 0)
        swap(t1_z, t2_z);
    t_near = max3(t1_x, t1_y, t1_z);
    t_far = min3(t2_x, t2_y, t2_z);
    // cout << "t1 xyz :" << t1_x << " " << t1_y << " " << t1_z << endl;
    // cout << "t near :" << t_near << endl;
    if (t_near >= t_far)
        return;
    if (t_far <= tmin)
        return;
    //inside
    if (t_near < tmin) {
        cout << "inside" << endl;
        if (t1_x > -INFINITY) {
            while (t1_x < tmin)
                t1_x += mi.dt_x;
        }
        if (t1_y > -INFINITY) {
            while (t1_y < tmin)
                t1_y += mi.dt_y;
        }
        if (t1_z > -INFINITY) {
            while (t1_z < tmin)
                t1_z += mi.dt_z;
        }
        //note INFINITY and here use min3
        
        t1_x = t1_x > tmin ? t1_x : INFINITY;
        t1_y = t1_y > tmin ? t1_y : INFINITY;
        t1_z = t1_z > tmin ? t1_z : INFINITY;
        cout << t1_x << " " << t1_y << " " << t1_z << endl;
        t_near = min3(t1_x, t1_y, t1_z);

        mi.t_cur = t_near;

        if (t_near == t1_x) {
            mi.normal = Vec3f(-1, 0, 0) * mi.sign_x;
            mi.t_next_x = t1_x + mi.dt_x;
            mi.t_next_y = t1_y;
            mi.t_next_z = t1_z;
        }
        if (t_near == t1_y) {
            mi.normal = Vec3f(0, -1, 0) * mi.sign_y;
            mi.t_next_x = t1_x;
            mi.t_next_y = t1_y + mi.dt_y;
            mi.t_next_z = t1_z;
        }
        if (t_near == t1_z) {
            mi.normal = Vec3f(0, 0, -1) * mi.sign_z;
            mi.t_next_x = t1_x;
            mi.t_next_y = t1_y;
            mi.t_next_z = t1_z + mi.dt_z;
        }
    } else { //outside
        //cout << "outside" << endl;
        mi.t_cur = t_near;
        if (t_near == t1_x) mi.normal = Vec3f(-1, 0, 0) * mi.sign_x;
        if (t_near == t1_y) mi.normal = Vec3f(0, -1, 0) * mi.sign_y;
        if (t_near == t1_z) mi.normal = Vec3f(0, 0, -1) * mi.sign_z;
        //mi.t_next_x
        if (t1_x > -INFINITY) {
            while (t1_x <= t_near)
                t1_x += mi.dt_x;
            mi.t_next_x = t1_x;
        }
        if (t1_y > -INFINITY) {
            while (t1_y <= t_near)
                t1_y += mi.dt_y;
            mi.t_next_y = t1_y;
        }
        if (t1_z > -INFINITY) {
            while (t1_z <= t_near)
                t1_z += mi.dt_z;
            mi.t_next_z = t1_z;
        }
    }

    //mi.i j k
    Vec3f p = orig + dir * t_near - minP;
    mi.i = int(p.x() / cellx);
    if (mi.sign_x < 0 && mi.i == nx) mi.i--;
    mi.j = int(p.y() / celly);
    if (mi.sign_y < 0 && mi.j == ny) mi.j--;
    mi.k = int(p.z() / cellz);
    if (mi.sign_z < 0 && mi.k == nz) mi.k--;

}

PhongMaterial* Grid::getMtl(int i) const
{
    Vec3f diffus;
    switch (i){
        case 1 :  diffus = Vec3f(1, 1, 1); break;
        case 2 :  diffus = Vec3f(1, 0, 1); break;
        case 3 :  diffus = Vec3f(0, 1, 1); break;
        case 4 :  diffus = Vec3f(1, 1, 0); break;
        case 5 :  diffus = Vec3f(0.3, 0, 0.7); break;
        case 6 :  diffus = Vec3f(0.7, 0, 0.3); break;
        case 7 :  diffus = Vec3f(0, 0.3, 0.7); break;
        case 8 :  diffus = Vec3f(0, 0.7, 0.3); break;
        case 9 :  diffus = Vec3f(0, 0.3, 0.7); break;
        case 10:  diffus = Vec3f(0, 0.7, 0.3); break;
        case 11:  diffus = Vec3f(0, 1, 0); break;
        case 12:  diffus = Vec3f(0, 0, 1); break;
        default:  diffus = Vec3f(1, 0, 0); break;
    }
    return new PhongMaterial(diffus);
}
