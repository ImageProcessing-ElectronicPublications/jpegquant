/*
 * jpegcstat.c
 * based dct.c
 *
 * Copyright (C) 2012, Owen Campbell-Moore.
 * Modify 2020, zvezdochiot
 *
 * This program allows you to take JPEG, modify it's DCT coefficients and
 * then output the new coefficients to a new JPEG.
 *
 * gcc -Wall -o jpegcstat jpegcstat.c -ljpeg -s
 */

#include <sys/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jerror.h>
#include <jpeglib.h>
#include <unistd.h>
#define SIZEOF(object)((size_t) sizeof(object))
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#ifndef EXIT_WARNING
#define EXIT_WARNING 2
#endif

void usage (char *cmd)
/* complain about bad command line */
{
    printf("\nUsage:\n %s [options] input.jpg\n\n", cmd);
    printf(" options:\n");
    printf("         -l N    lower bound (int, optional, default = -65535)\n");
    printf("         -u N    upper bound (int, optional, default = 65535)\n");
    printf("         -h      this help\n\n");
    exit(EXIT_FAILURE);
}

/*
 * The main program.
 */

int
main (int argc, char **argv)
{
    struct jpeg_decompress_struct inputinfo;
    struct jpeg_error_mgr jerr;
    jvirt_barray_ptr *coef_arrays;
    JDIMENSION compnum, rownum, blocknum;
    JBLOCKARRAY coef_buffers[MAX_COMPONENTS];
    JBLOCKARRAY row_ptrs[MAX_COMPONENTS];
    FILE * input_file;
    char *inputname;
    int opt, fhelp = 0, lower = -65535, upper = 65535, cmax = 0, cmin = 0, coef;
    int i, *hist;

    /* Handle arguments */
    while ((opt = getopt(argc, argv, ":l:u:h")) != -1)
    {
        switch(opt)
        {
            case 'l':
                lower = atoi(optarg);
                break;
            case 'u':
                upper = atoi(optarg);
                break;
            case 'h':
                fhelp = 1;
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }
    if (optind + 1 > argc || fhelp) usage(argv[0]);

    inputname = argv[optind];

    /* Open the input and output files */
    input_file = (strcmp("-", inputname) == 0) ? stdin : fopen(inputname, "rb");
    if (input_file == NULL)
    {
        fprintf(stderr, "ERROR: Can't open %s\n", inputname);
        exit(EXIT_FAILURE);
    }

    /* Initialize the JPEG compression and decompression objects with default error handling. */
    inputinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&inputinfo);

    /* Specify data source for decompression and recompression */
    jpeg_stdio_src(&inputinfo, input_file);

    /* Read file header */
    (void) jpeg_read_header(&inputinfo, TRUE);
    fprintf(stderr, "Read DCT coefficients successfully written to %s\n", inputname);

    /* Allocate memory for reading out DCT coeffs */
    for (compnum=0; compnum<inputinfo.num_components; compnum++)
        coef_buffers[compnum] = ((&inputinfo)->mem->alloc_barray)
                            ((j_common_ptr) &inputinfo, JPOOL_IMAGE,
                             inputinfo.comp_info[compnum].width_in_blocks,
                             inputinfo.comp_info[compnum].height_in_blocks);

    /* Read input file as DCT coeffs */
    coef_arrays = jpeg_read_coefficients(&inputinfo);

    /* Copy DCT coeffs to a new array */
    int num_components = inputinfo.num_components;
    int width_in_blocks[num_components];
    int height_in_blocks[num_components];
    for (compnum=0; compnum<num_components; compnum++)
    {
        height_in_blocks[compnum] = inputinfo.comp_info[compnum].height_in_blocks;
        width_in_blocks[compnum] = inputinfo.comp_info[compnum].width_in_blocks;
        for (rownum=0; rownum<height_in_blocks[compnum]; rownum++)
        {
            row_ptrs[compnum] = ((&inputinfo)->mem->access_virt_barray)
                                ((j_common_ptr) &inputinfo, coef_arrays[compnum],
                                    rownum, (JDIMENSION) 1, FALSE);
            for (blocknum=0; blocknum<width_in_blocks[compnum]; blocknum++)
            {
                for (i=0; i<DCTSIZE2; i++)
                {
                    coef_buffers[compnum][rownum][blocknum][i] = row_ptrs[compnum][0][blocknum][i];
                }
            }
        }
    }

    for (compnum=0; compnum<num_components; compnum++)
    {
        for (rownum=0; rownum<height_in_blocks[compnum]; rownum++)
        {
            for (blocknum=0; blocknum<width_in_blocks[compnum]; blocknum++)
            {
                for (i=0; i<DCTSIZE2; i++)
                {
                    coef = (int)coef_buffers[compnum][rownum][blocknum][i];
                    cmax = (coef > cmax) ? coef : cmax;
                    cmin = (coef < cmin) ? coef : cmin;
                }
            }
        }
    }
    printf("Max = %d, Min = %d\n", cmax, cmin);
    hist = (int*)malloc ((cmax - cmin) * sizeof(int));
    for (compnum=0; compnum<num_components; compnum++)
    {
        for (rownum=0; rownum<height_in_blocks[compnum]; rownum++)
        {
            for (blocknum=0; blocknum<width_in_blocks[compnum]; blocknum++)
            {
                for (i=0; i<DCTSIZE2; i++)
                {
                    coef = (int)coef_buffers[compnum][rownum][blocknum][i];
                    hist[coef - cmin]++;
                }
            }
        }
    }
    upper = (upper == 65535) ? cmax : upper;
    lower = (lower == -65535) ? cmin: lower;
    for (i = cmin; i < cmax + 1; i++)
    {
        if (((int)i >= lower) && ((int)i <= upper))
            printf("%d = %d\n", i, hist[i - cmin]);
    }

    /* Close files */
    fclose(input_file);

    /* All done. */
    exit(jerr.num_warnings ? EXIT_WARNING : EXIT_SUCCESS);
    return 0; /* suppress no-return-value warnings */
}
