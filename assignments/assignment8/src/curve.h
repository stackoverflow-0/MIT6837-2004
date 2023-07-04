#pragma once

#include "spline.h"

#include <vector>

class ArgParser;

class TriangleMesh;

class Curve : public Spline
{
public:
    Curve(int num_vertices) : num_vertices(num_vertices)
    {
        vertexs.resize(num_vertices);
    }

    void set(int i, const Vec3f &v)
    {
        // cout << "i & n : " << i << " "<< num_vertices << endl;
        // assert(i < num_vertices);
        vertexs[i] = v;
    }

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args);

    // FOR CONTROL POINT PICKING
    virtual int getNumVertices() { return num_vertices; };
    virtual Vec3f getVertex(int i) { return vertexs[i]; };

    virtual void moveControlPoint(int selectedPoint, float x, float y);
    virtual void addControlPoint(int selectedPoint, float x, float y);
    virtual void deleteControlPoint(int selectedPoint);

protected:
    int num_vertices{0};
    std::vector<Vec3f> vertexs;

    void drawLines();

    void drawPoints();

    virtual void drawCurves(ArgParser *args) = 0;
};

class BezierCurve : public Curve
{
public:
    BezierCurve(int num_vertices) : Curve(num_vertices) {}

    virtual void OutputBezier(FILE *file);
    virtual void OutputBSpline(FILE *file);

    virtual TriangleMesh *OutputTriangles(ArgParser *args);

private:
    void drawCurves(ArgParser *args);
};

class BSplineCurve : public Curve
{
public:
    BSplineCurve(int num_vertices) : Curve(num_vertices) {}

    virtual void OutputBezier(FILE *file);
    virtual void OutputBSpline(FILE *file);

    virtual TriangleMesh *OutputTriangles(ArgParser *args);

private:
    void drawCurves(ArgParser *args);
};