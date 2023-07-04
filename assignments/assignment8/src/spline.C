#include "spline.h"

Matrix Spline::B_bezier = Matrix(new float[16]{-1, 3, -3, 1,
                                               3, -6, 3, 0,
                                               -3, 3, 0, 0,
                                               1, 0, 0, 0});

Matrix Spline::B_bsplne = Matrix(new float[16]{-1, 3, -3, 1,
                                               3, -6, 0, 4,
                                               -3, 3, 3, 1,
                                               1, 0, 0, 0}) * (1.0f / 6.0f);
