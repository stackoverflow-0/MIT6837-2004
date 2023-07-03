#include "filter.h"

Vec3f Filter::getColor(int i, int j, Film *film)
{
    int width = film->getWidth();
    int height = film->getHeight();

    int supportRadius = getSupportRadius();
    int i_start = max(0, i - supportRadius);
    int i_end = min(width - 1, i = supportRadius);
    int j_start = max(0, j - supportRadius);
    int j_end = min(height - 1, j + supportRadius);

    int sample_num = film->getNumSamples();

    Vec3f color(0, 0, 0);

    for (int ni = i_start; ni <= i_end; ++ni)
    {
        for (int nj = j_start; nj <= j_end; ++nj)
        {
            float fi = float(ni) / float(width);
            float fj = float(nj) / float(height);

            float weight = getWeight(fi - float(ni), fj - float(nj)) / float(sample_num);

            for (int n = 0; n < sample_num; ++n)
            {
                color += weight * film->getSample(ni, nj, n).getColor();
            }
        }
    }

    return color;
}

// float Filter::getWeight(float x, float y)
// {
//     return 1.0f / float(filte);
// }

float BoxFilter::getWeight(float x, float y)
{
    return 1.0f / total_weight;
}

float TentFilter::getWeight(float x, float y)
{
    float dis = 1 - max(fabs(x), fabs(y)) / float(radius);
    return dis / total_weight;
}

float GaussianFilter::getWeight(float x, float y)
{
    return expf(-(x * x + y * y) / (2 * sigma)) / total_weight;
}
