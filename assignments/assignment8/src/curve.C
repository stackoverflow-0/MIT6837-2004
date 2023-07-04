#include "curve.h"
#include "GL/freeglut.h"
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

void Curve::drawCurves(ArgParser *args)
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
        }
    }
    glEnd();
}
