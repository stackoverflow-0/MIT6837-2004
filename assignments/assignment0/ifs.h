#ifndef _IFS_H_
#define _IFS_H_

#include "matrix.h"

#include "vectors.h"

#include "image.h"

#include <stdio.h>

#include <stdlib.h>


class IFS {
    public:

    IFS(const char* input_path, int num_points, int num_iters, int size);

    ~IFS();

    void render(const char* output_path);

    private:
    int num_transforms;
    int ifs_size;
    int ifs_iters;
    int ifs_points;
    Matrix * tfms;
    double  * probs;


};


#endif