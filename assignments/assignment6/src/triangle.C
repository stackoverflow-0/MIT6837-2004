#include "triangle.h"
#include "grid.h"
#include "transform.h"
#include "raytracing_stats.h"

bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
  RayTracingStats::IncrementNumIntersections();
  Vec3f orig = r.getOrigin();
  Vec3f dir = r.getDirection();
  Vec3f e1(mb, ma);
  Vec3f e2(mc, ma);
  Vec3f pvec;
  Vec3f::Cross3(pvec, dir, e2);
  float det = e1.Dot3(pvec);

  if (det > -0.00001 && det < 0.00001)
    return (false);

  float invDet = 1 / det;
  Vec3f tvec(orig, ma);
  float u = invDet * tvec.Dot3(pvec);

  if (u < 0.0 || u > 1.0)
    return (false);

  Vec3f qvec;
  Vec3f::Cross3(qvec, tvec, e1);
  float v = invDet * dir.Dot3(qvec);

  if (v < 0.0 || u + v > 1.0)
    return (false);

  float t = invDet * e2.Dot3(qvec);

  if (t > tmin && t < h.getT())
  {
    h.set(t, mtl, normal, r);
    return true;
  }

  return false;
}

void Triangle::paint() const
{
  // printf("triangle draw \n");
  mtl->glSetMaterial();
  glBegin(GL_TRIANGLES);
  glNormal3f(normal.x(), normal.y(), normal.z());
  glVertex3f(ma.x(), ma.y(), ma.z());
  glVertex3f(mb.x(), mb.y(), mb.z());
  glVertex3f(mc.x(), mc.y(), mc.z());
  glEnd();
}

BoundingBox *Triangle::getTriangleBoundingBox(const Matrix *m) const {
    if(!m) return bdBox;
    Vec3f aa = ma, bb = mb, cc = mc;
    m->Transform(aa);
    m->Transform(bb);
    m->Transform(cc);
   BoundingBox *t = new BoundingBox(Vec3f(min(aa.x(), bb.x()), min(aa.y(), bb.y()), min(aa.z(), bb.z())),
                                     Vec3f(max(aa.x(), bb.x()), max(aa.y(), bb.y()), max(aa.z(), bb.z())));
    t->Extend(cc);
    return t;
}

void Triangle::insertIntoGrid(Grid *g, Matrix *m) {
    BoundingBox *bb_new = getTriangleBoundingBox(m);
    BoundingBox *grid_bb = g->getBoundingBox();
    // grid_bb->Extend(bb_new);
    Vec3f grid_min = grid_bb->getMin();
    Vec3f grid_max = grid_bb->getMax();
    int nx = g->nx;
    int ny = g->ny;
    int nz = g->nz;
    float cellx = (grid_max - grid_min).x() / float(nx);
    float celly = (grid_max - grid_min).y() / float(ny);
    float cellz = (grid_max - grid_min).z() / float(nz);
    Vec3f obj_min = bb_new->getMin();
    Vec3f obj_max = bb_new->getMax();
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
    //note this
    if (start_i == nx) start_i--;
    if (start_j == ny) start_j--;
    if (start_k == nz) start_k--;

    for (int k = start_k; k <= end_k; ++k) {
        for (int j = start_j; j <= end_j; ++j) {
            for (int i = start_i; i <= end_i; ++i) {
                int index = nx * ny * k + nx * j + i;
                if(m)
                    g->opaque[index].push_back(new Transform(*m, this));
                else
                    g->opaque[index].push_back(this);
            }
        }
    }
}
