/*
 * jpegquant.c
 * based dct.c
 *
 * Copyright (C) 2012, Owen Campbell-Moore.
 * Modify 2019, zvezdochiot
 *
 * This program allows you to take JPEG, modify it's DCT coefficients and
 * then output the new coefficients to a new JPEG.
 *
 * gcc -Wall -I/usr/include/mozjpeg -o mozjpegquant jpegquant.c -lmozjpeg -s
 * gcc -Wall -o jpegquant jpegquant.c -ljpeg -s
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
    printf("\nUsage:\n %s [options] input.jpg output.jpg\n\n", cmd);
    printf(" options:\n");
    printf("         -q N.N  quant (double, optional, default = 100.0)\n");
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
    struct jpeg_compress_struct outputinfo;
    struct jpeg_error_mgr jerr;
    jvirt_barray_ptr *coef_arrays;
    JDIMENSION i, compnum, rownum, blocknum;
    JBLOCKARRAY coef_buffers[MAX_COMPONENTS];
    JBLOCKARRAY row_ptrs[MAX_COMPONENTS];
    FILE * input_file;
    FILE * output_file;
    char *inputname, *outputname;
    double recoef, coeferr, sumce, numc, iquant, quant = 100.0;
    int opt, fhelp = 0;

    /* Handle arguments */
    while ((opt = getopt(argc, argv, ":q:h")) != -1)
    {
        switch(opt)
        {
            case 'q':
                quant = atof(optarg);
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
    if (optind + 2 > argc || fhelp) usage(argv[0]);

    inputname = argv[optind];
    outputname = argv[optind + 1];
    if (strcmp(inputname, outputname) == 0)
    {
        fprintf(stderr, "ERROR: Cannot output to input %s\n", inputname);
        exit(EXIT_FAILURE);
    }
    /* Open the input and output files */
    if ((input_file = fopen(inputname, "rb")) == NULL)
    {
        fprintf(stderr, "ERROR: Can't open %s\n", inputname);
        exit(EXIT_FAILURE);
    }
    if ((output_file = fopen(outputname, "wb")) == NULL)
    {
        fprintf(stderr, "ERROR: Can't open %s\n", outputname);
        exit(EXIT_FAILURE);
    }

    /* Initialize the JPEG compression and decompression objects with default error handling. */
    inputinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&inputinfo);
    outputinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&outputinfo);

    /* Specify data source for decompression and recompression */
    jpeg_stdio_src(&inputinfo, input_file);
    jpeg_stdio_dest(&outputinfo, output_file);

    /* Read file header */
    (void) jpeg_read_header(&inputinfo, TRUE);
    printf("Read DCT coefficients successfully written to %s\n", inputname);

    /* Allocate memory for reading out DCT coeffs */
    for (compnum=0; compnum<inputinfo.num_components; compnum++)
        coef_buffers[compnum] = ((&inputinfo)->mem->alloc_barray) 
                            ((j_common_ptr) &inputinfo, JPOOL_IMAGE,
                             inputinfo.comp_info[compnum].width_in_blocks,
                             inputinfo.comp_info[compnum].height_in_blocks);

    /* Read input file as DCT coeffs */
    coef_arrays = jpeg_read_coefficients(&inputinfo);

    /* Copy compression parameters from the input file to the output file */
    jpeg_copy_critical_parameters(&inputinfo, &outputinfo);

    /* Copy DCT coeffs to a new array */
    int num_components = inputinfo.num_components;
    size_t block_row_size[num_components];
    int width_in_blocks[num_components];
    int height_in_blocks[num_components];
    for (compnum=0; compnum<num_components; compnum++)
    {
        height_in_blocks[compnum] = inputinfo.comp_info[compnum].height_in_blocks;
        width_in_blocks[compnum] = inputinfo.comp_info[compnum].width_in_blocks;
        block_row_size[compnum] = (size_t) SIZEOF(JCOEF)*DCTSIZE2*width_in_blocks[compnum];
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

    printf("Quant = %f%%\n", quant);
    quant /= 100.0;
    iquant = (quant == 0) ? 1.0 : 1.0 / quant;
    sumce = 0.0;
    /* Print out or modify DCT coefficients */
    for (compnum=0; compnum<num_components; compnum++)
    {
        for (rownum=0; rownum<height_in_blocks[compnum]; rownum++)
        {
            for (blocknum=0; blocknum<width_in_blocks[compnum]; blocknum++)
            {
                //printf("\n\nComponent: %i, Row:%i, Column: %i\n", compnum, rownum, blocknum);
                for (i=0; i<DCTSIZE2; i++)
                {
                    recoef = coef_buffers[compnum][rownum][blocknum][i];
                    coeferr = recoef;
                    recoef = (int)(recoef * quant);
                    recoef = (int)(recoef * iquant);
                    coeferr -= recoef;
                    if (coeferr < 0) coeferr = -coeferr;
                    sumce += coeferr;
                    numc++;
                    coef_buffers[compnum][rownum][blocknum][i] = recoef;
                    //printf("%i,", coef_buffers[compnum][rownum][blocknum][i]);
                }
            }
        }
    }
    if (numc > 0) sumce /= numc;
    printf("QuantErr = %f\n", sumce);
    //printf("\n\n");

    /* Output the new DCT coeffs to a JPEG file */
    for (compnum=0; compnum<num_components; compnum++)
    {
        for (rownum=0; rownum<height_in_blocks[compnum]; rownum++)
        {
            row_ptrs[compnum] = ((&outputinfo)->mem->access_virt_barray) 
                                ((j_common_ptr) &outputinfo, coef_arrays[compnum], 
                                 rownum, (JDIMENSION) 1, TRUE);
            memcpy(row_ptrs[compnum][0][0], 
                 coef_buffers[compnum][rownum][0],
                 block_row_size[compnum]);
        }
    }

    /* Write to the output file */
    jpeg_write_coefficients(&outputinfo, coef_arrays);

    /* Finish compression and release memory */
    jpeg_finish_compress(&outputinfo);
    jpeg_destroy_compress(&outputinfo);
    jpeg_finish_decompress(&inputinfo);
    jpeg_destroy_decompress(&inputinfo);

    /* Close files */
    fclose(input_file);
    fclose(output_file);

    /* All done. */
    printf("New DCT coefficients successfully written to %s\n\n", outputname);
    exit(jerr.num_warnings ? EXIT_WARNING : EXIT_SUCCESS);
    return 0; /* suppress no-return-value warnings */
}
