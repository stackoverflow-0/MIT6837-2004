#include "object3d.h"
#include "grid.h"

void Object3D::insertIntoGrid(Grid *g, Matrix *m)
{
    assert(bdBox != nullptr);
    BoundingBox *bdBox_new = bdBox;
    // cout << bdBox_new->getMin() << " " << bdBox_new->getMax() << endl;
    if (m)
    {
        Vec3f obj_min = bdBox->getMin();
        Vec3f obj_max = bdBox->getMax();
        float x1 = obj_min.x(), y1 = obj_min.y(), z1 = obj_min.z();
        float x2 = obj_max.x(), y2 = obj_max.y(), z2 = obj_max.z();
        // 8个顶点
        Vec3f v1 = Vec3f(x1, y1, z1);
        Vec3f v2 = Vec3f(x1, y1, z2);
        Vec3f v3 = Vec3f(x1, y2, z1);
        Vec3f v4 = Vec3f(x1, y2, z2);
        Vec3f v5 = Vec3f(x2, y1, z1);
        Vec3f v6 = Vec3f(x2, y1, z2);
        Vec3f v7 = Vec3f(x2, y2, z1);
        Vec3f v8 = Vec3f(x2, y2, z2);
        // 应用坐标变换
        m->Transform(v1);
        m->Transform(v2);
        m->Transform(v3);
        m->Transform(v4);
        m->Transform(v5);
        m->Transform(v6);
        m->Transform(v7);
        m->Transform(v8);
        bdBox_new = new BoundingBox(
            Vec3f(INFINITY, INFINITY, INFINITY),
            Vec3f(-INFINITY, -INFINITY, -INFINITY));
        // 根据8个顶点 拓展bdBox的范围
        bdBox_new->Extend(v1);
        bdBox_new->Extend(v2);
        bdBox_new->Extend(v3);
        bdBox_new->Extend(v4);
        bdBox_new->Extend(v5);
        bdBox_new->Extend(v6);
        bdBox_new->Extend(v7);
        bdBox_new->Extend(v8);
    }
    // cout << bdBox_new->getMin() << " " << bdBox_new->getMax() << endl;

    BoundingBox *grid_bb = g->getBoundingBox();
    // 获取网格的绑定箱
    Vec3f grid_min = grid_bb->getMin();
    Vec3f grid_max = grid_bb->getMax();

    int nx = g->nx;
    int ny = g->ny;
    int nz = g->nz;

    float cellx = (grid_max - grid_min).x() / float(nx);
    float celly = (grid_max - grid_min).y() / float(ny);
    float cellz = (grid_max - grid_min).z() / float(nz);

    Vec3f obj_min = bdBox_new->getMin();
    Vec3f obj_max = bdBox_new->getMax();

    int start_i = int((obj_min - grid_min).x() / cellx);
    int start_j = int((obj_min - grid_min).y() / celly);
    int start_k = int((obj_min - grid_min).z() / cellz);

    int end_i = int((obj_max - grid_min).x() / cellx);
    int end_j = int((obj_max - grid_min).y() / celly);
    int end_k = int((obj_max - grid_min).z() / cellz);

    if (start_i > nx || start_j > ny || start_k > nz || end_i < 0 || end_j < 0 || end_k < 0)
        return;

    start_i = max(start_i, 0);
    start_j = max(start_j, 0);
    start_k = max(start_k, 0);
    end_i = min(end_i, nx - 1);
    end_j = min(end_j, ny - 1);
    end_k = min(end_k, nz - 1);

    // note this
    if (start_i == nx)
        start_i--;
    if (start_j == ny)
        start_j--;
    if (start_k == nz)
        start_k--;

    for (int k = start_k; k <= end_k; ++k)
    {
        for (int j = start_j; j <= end_j; ++j)
        {
            for (int i = start_i; i <= end_i; ++i)
            {
                int index = nx * ny * k + nx * j + i;
                g->opaque[index].push_back(this);
            }
        }
    }
}