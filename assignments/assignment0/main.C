#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ifs.h"


int main(int argc, char *argv[])
{
    char *input_file = NULL;
    int num_points = 10000;
    int num_iters = 10;
    int size = 100;
    char *output_file = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-input"))
        {
            i++;
            assert(i < argc);
            input_file = argv[i];
        }
        else if (!strcmp(argv[i], "-points"))
        {
            i++;
            assert(i < argc);
            num_points = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-iters"))
        {
            i++;
            assert(i < argc);
            num_iters = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-size"))
        {
            i++;
            assert(i < argc);
            size = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-output"))
        {
            i++;
            assert(i < argc);
            output_file = argv[i];
        }
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
    IFS ifs(input_file, num_points, num_iters, size);
    ifs.render(output_file);
}