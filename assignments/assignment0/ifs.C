#include "ifs.h"

IFS::IFS(const char *input_path, int num_points, int num_iters, int size)
{
    ifs_size = size;
    ifs_iters = num_iters;
    ifs_points = num_points;
    // open the file
    FILE *input = fopen(input_path, "r");
    assert(input != NULL);

    // read the number of transforms
    // int num_transforms;
    fscanf(input, "%d", &num_transforms);

    // < DO SOMETHING WITH num_transforms >

    tfms = new Matrix[num_transforms];
    probs = new double[num_transforms];

    // read in the transforms
    for (int i = 0; i < num_transforms; i++)
    {
        float probability;
        fscanf(input, "%f", &probability);
        // Matrix m;
        // m.Read3x3(input);
        // < DO SOMETHING WITH probability and m >
        probs[i] = probability;
        tfms[i].Read3x3(input);
        
    }

    // close the file
    fclose(input);
}

IFS::~IFS()
{
    delete [] tfms;
    delete [] probs;
}

void IFS::render(const char *output_path)
{
    assert(output_path != NULL);
    Image img(ifs_size, ifs_size);
    img.SetAllPixels(Vec3f(1, 1, 1));
    Vec2f *points = new Vec2f[ifs_points];

    int cnt[num_transforms];

    for(int i = 0;i < num_transforms;i++) {
        cnt[i] = 0;
    }

    for (int pidx = 0; pidx < ifs_points; pidx++)
    {
        points[pidx].Set(drand48(), drand48());
        for (int k = 0; k < ifs_iters; k++)
        {
            double rand_num = drand48();
            double range = 0;
            int pick_idx = 0;

            for (; pick_idx < num_transforms; pick_idx++)
            {
                range += probs[pick_idx];
                if (range >= rand_num)
                {
                    cnt[pick_idx]++;
                    break;
                }
            }
            Vec2f v2  = points[pidx];
            tfms[pick_idx].Transform(v2);
            points[pidx] = v2;
            
        }
        if(points[pidx].x() < 1 &&  points[pidx].x() >= 0 && points[pidx].y() < 1 &&  points[pidx].y() >= 0) {
            img.SetPixel(points[pidx].x() * ifs_size, points[pidx].y() * ifs_size, Vec3f(0, 0, 0));
        }
    }
    
    for(int i = 0;i < num_transforms;i++) {
        printf("mat %d pick %d\n", i,cnt[i]);
    }
    
    img.SaveTGA(output_path);
    delete [] points;
}
