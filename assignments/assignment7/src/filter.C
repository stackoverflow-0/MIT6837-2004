#include "filter.h"

Vec3f Filter::getColor(int i, int j, Film *film)
{
    int width = film->getWidth();
    int height = film->getHeight();

    int supportRadius = getSupportRadius();
    int filter_side = 2 * supportRadius + 1;
    int i_start = max(0, i - supportRadius);
    int i_end = min(width - 1, i + supportRadius);
    int j_start = max(0, j - supportRadius);
    int j_end = min(height - 1, j + supportRadius);

    int sample_num = film->getNumSamples();

    Vec3f color(0, 0, 0);
    // cout << film->getSample(0, 0, 0).getColor() << endl;
    float total_weight = 0;
    for (int ni = i_start; ni <= i_end; ++ni)
    {
        for (int nj = j_start; nj <= j_end; ++nj)
        {
            // position in filter matrix
            float xoffset = float(ni - i) / float(filter_side);
            float yoffset = float(nj - j) / float(filter_side);

            // cout << "weight : " << weight << endl;

            for (int n = 0; n < sample_num; ++n)
            {
                // cout << "ni nj : " << ni << nj << endl;
                // cout << film->getSample(0, 0, 0).getColor() << endl;
                Vec2f sample_offset = film->getSample(ni, nj, n).getPosition();
                sample_offset /= float(filter_side);
                float weight = getWeight(xoffset + sample_offset.x(), yoffset + sample_offset.y());
                total_weight += weight;
                color += weight * film->getSample(ni, nj, n).getColor();
            }
        }
    }

    return color /= total_weight;
}

// float Filter::getWeight(float x, float y)
// {
//     return 1.0f / float(filte);
// }

float BoxFilter::getWeight(float x, float y)
{
    return 1.0f;
}

float TentFilter::getWeight(float x, float y)
{
    float w = radius > 0 ? 1 - max(fabs(x), fabs(y)) / float(radius) : 1.0f;
    return w;
}

float GaussianFilter::getWeight(float x, float y)
{
    // float d2 = sqrt(x * x + y * y);
    // float cutoff_dis = 2 * sigma;
    // if (d2 < cutoff_dis)
    // {
    //     return expf(-(x * x + y * y) / (2 * sigma * sigma));
    // }

    // return 0;
    // return d2 < cutoff_dis ? expf(-(x * x + y * y) / (2 * sigma * sigma)) : 0.0f;
    return expf(-(x * x + y * y) / (2 * sigma * sigma));
}
