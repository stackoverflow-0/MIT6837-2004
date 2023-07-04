#include "curve.h"
#include "GL/freeglut.h"
#include "triangle_mesh.h"

void Curve::Paint(ArgParser *args)
{
    drawLines();
    drawPoints();
    drawCurves(args);
}
void Curve::drawLines()
{
    glLineWidth(3);
    glColor3f(0, 0, 1);

    glBegin(GL_LINE_STRIP);

    for (auto v : vertexs)
    {
        glVertex3f(v.x(), v.y(), v.z());
    }

    glEnd();
}

void Curve::drawPoints()
{
    glPointSize(6);
    glColor3f(1, 0, 0);
    glBegin(GL_POINT);

    for (auto v : vertexs)
    {
        glVertex3f(v.x(), v.y(), v.z());
    }

    glEnd();
}

void Curve::moveControlPoint(int selectedPoint, float x, float y)
{
    vertexs[selectedPoint].Set(x, y, 0);
}

void Curve::addControlPoint(int selectedPoint, float x, float y)
{
    num_vertices++;
    auto iter = vertexs.begin();
    while (selectedPoint--)
    {
        iter++;
    }
    vertexs.insert(iter, Vec3f(x, y, 0));
}

void Curve::deleteControlPoint(int selectedPoint)
{
    if (num_vertices <= 4)
    {
        return;
    }
    num_vertices--;
    auto iter = vertexs.begin();
    while (selectedPoint--)
    {
        iter++;
    }
    vertexs.erase(iter);
}

void BezierCurve::OutputBezier(FILE *file)
{
    fprintf(file, "\nbspline\nnum_vertices %d\n", num_vertices);
    for (auto v : vertexs)
    {
        fprintf(file, "%f %f %f\n", v.x(), v.y(), v.z());
    }
}

void BezierCurve::OutputBSpline(FILE *file)
{
    fprintf(file, "\nbspline\nnum_vertices %d\n", num_vertices);
    Matrix G_bezier;
    for (int i = 0; i < num_vertices; i++)
    {
        G_bezier.Set(i, 0, vertexs[i].x());
        G_bezier.Set(i, 1, vertexs[i].y());
        G_bezier.Set(i, 2, vertexs[i].z());
    }
    Matrix inv_B_bezier = B_bezier;
    inv_B_bezier.Inverse();
    Matrix G_bspline = G_bezier * B_bezier * inv_B_bezier;
    for (int i = 0; i < num_vertices; i++)
    {
        fprintf(file, "%f %f %f\n", G_bspline.Get(i, 0), G_bspline.Get(i, 1), G_bspline.Get(i, 2));
    }
}

TriangleMesh *BezierCurve::OutputTriangles(ArgParser *args)
{

    int curve_tessellation = args->curve_tessellation;

    float d_curve = 1.0f / float(curve_tessellation);

    int revolution_tessellation = args->revolution_tessellation;

    float d_revo = 1.0f / float(revolution_tessellation);

    TriangleNet *triangleNet = new TriangleNet(revolution_tessellation, (num_vertices - 3) * curve_tessellation);

    for (int i = 0; i < num_vertices; i += 3)
    {
        Matrix G;
        for (int j = 0; j < 4; j++)
        {
            G.Set(j, 0, getVertex(i + j).x());
            G.Set(j, 1, getVertex(i + j).y());
            G.Set(j, 2, getVertex(i + j).z());
        }
        float t = 0;
        for (int k = 0; k < curve_tessellation; k++)
        {
            float t2 = t * t;
            float t3 = t * t2;
            Vec4f tvec(t3, t2, t, 1);
            B_bezier.Transform(tvec);
            G.Transform(tvec);
            float theta = 0;
            for (int n = 0; n < revolution_tessellation; n++)
            {
                float x = cos(theta) * tvec.x() + sin(theta) * tvec.z();
                float y = tvec.y();
                float z = -sin(theta) * tvec.x() + cos(theta) * tvec.z();

                triangleNet->SetVertex(n, k + i * curve_tessellation, Vec3f(x, y, z));
            }
            t += d_curve;
        }
    }
    return triangleNet;
}

void BezierCurve::drawCurvs(ArgParser *args)
{
    glLineWidth(4);
    glColor3f(0, 1, 0);

    glBegin(GL_LINE_STRIP);

    int tessellation = args->curve_tessellation;

    float d = 1.0f / tessellation;

    for (int i = 0; i < num_vertices; i += 3)
    {
        Matrix G;
        for (int j = 0; j < 4; j++)
        {
            G.Set(j, 0, getVertex(i + j).x());
            G.Set(j, 1, getVertex(i + j).y());
            G.Set(j, 2, getVertex(i + j).z());
        }
        float t = 0;
        for (int k = 0; k < num_vertices; k++)
        {
            float t2 = t * t;
            float t3 = t * t2;
            Vec4f tvec(t3, t2, t, 1);
            B_bezier.Transform(tvec);
            G.Transform(tvec);
            glVertex3f(tvec.x(), tvec.y(), tvec.z());
            t += d;
        }
    }
    glEnd();
}

void BSplineCurve::OutputBezier(FILE *file)
{
    fprintf(file, "\nbspline\nnum_vertices %d\n", num_vertices);
    Matrix G_bspline;
    for (int i = 0; i < num_vertices; i++)
    {
        G_bspline.Set(i, 0, vertexs[i].x());
        G_bspline.Set(i, 1, vertexs[i].y());
        G_bspline.Set(i, 2, vertexs[i].z());
    }
    Matrix inv_B_bspline = B_bsplne;
    inv_B_bspline.Inverse();
    Matrix G_bszier = G_bspline * B_bezier * inv_B_bspline;
    for (int i = 0; i < num_vertices; i++)
    {
        fprintf(file, "%f %f %f\n", G_bspline.Get(i, 0), G_bspline.Get(i, 1), G_bspline.Get(i, 2));
    }
}

void BSplineCurve::OutputBSpline(FILE *file)
{
    fprintf(file, "\nbspline\nnum_vertices %d\n", num_vertices);
    for (auto v : vertexs)
    {
        fprintf(file, "%f %f %f\n", v.x(), v.y(), v.z());
    }
}

TriangleMesh *BSplineCurve::OutputTriangles(ArgParser *args)
{
    int curve_tessellation = args->curve_tessellation;

    float d_curve = 1.0f / float(curve_tessellation);

    int revolution_tessellation = args->revolution_tessellation;

    float d_revo = 1.0f / float(revolution_tessellation);

    TriangleNet *triangleNet = new TriangleNet(revolution_tessellation, (num_vertices - 3) * curve_tessellation);

    for (int i = 0; i < num_vertices; i += 3)
    {
        Matrix G;
        for (int j = 0; j < 4; j++)
        {
            G.Set(j, 0, getVertex(i + j).x());
            G.Set(j, 1, getVertex(i + j).y());
            G.Set(j, 2, getVertex(i + j).z());
        }
        float t = 0;
        for (int k = 0; k < curve_tessellation; k++)
        {
            float t2 = t * t;
            float t3 = t * t2;
            Vec4f tvec(t3, t2, t, 1);
            B_bsplne.Transform(tvec);
            G.Transform(tvec);
            float theta = 0;
            for (int n = 0; n < revolution_tessellation; n++)
            {
                float x = cos(theta) * tvec.x() + sin(theta) * tvec.z();
                float y = tvec.y();
                float z = -sin(theta) * tvec.x() + cos(theta) * tvec.z();

                triangleNet->SetVertex(n, k + i * curve_tessellation, Vec3f(x, y, z));
            }
            t += d_curve;
        }
    }
    return triangleNet;
}

void BSplineCurve::drawCurvs(ArgParser *args)
{
    glLineWidth(4);
    glColor3f(0, 1, 0);

    glBegin(GL_LINE_STRIP);

    int tessellation = args->curve_tessellation;

    float d = 1.0f / tessellation;

    for (int i = 0; i < num_vertices; i += 3)
    {
        Matrix G;
        for (int j = 0; j < 4; j++)
        {
            G.Set(j, 0, getVertex(i + j).x());
            G.Set(j, 1, getVertex(i + j).y());
            G.Set(j, 2, getVertex(i + j).z());
        }
        float t = 0;
        for (int k = 0; k < num_vertices; k++)
        {
            float t2 = t * t;
            float t3 = t * t2;
            Vec4f tvec(t3, t2, t, 1);
            B_bsplne.Transform(tvec);
            G.Transform(tvec);
            glVertex3f(tvec.x(), tvec.y(), tvec.z());
        }
    }
    glEnd();
}
