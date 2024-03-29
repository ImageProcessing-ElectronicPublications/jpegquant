/*
 * jpegquarel.c
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
    printf("         -c N    count cicle (int, optional, default = 1)\n");
    printf("         -k N.N  coeff average (double, optional, default = 1.0)\n");
    printf("         -l N    lower bound (int, optional, default = -65535)\n");
    printf("         -u N    upper bound (int, optional, default = 65535)\n");
    printf("         -d N.N  rounding quant (double, optional, default = 0.5)\n");
    printf("         -q N.N  quant (double, optional, default = 1.0)\n");
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
    int coeforig, coefres, coefold, coeferr;
    float sumcec, sumcend, numc = 0.0f, quant = 1.0f, iquant = 1.0f, qdelta = 0.5f, qsdelta, iquant2, kavr = 1.0f;
    int opt, fhelp = 0, ccicle = 1, ct, lower = -65535, upper = 65535;

    /* Handle arguments */
    while ((opt = getopt(argc, argv, ":c:k:l:u:d:q:h")) != -1)
    {
        switch(opt)
        {
            case 'c':
                ccicle = atoi(optarg);
                break;
            case 'k':
                kavr = atof(optarg);
                break;
            case 'l':
                lower = atoi(optarg);
                break;
            case 'u':
                upper = atoi(optarg);
                break;
            case 'd':
                qdelta = atof(optarg);
                break;
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
    ccicle = (ccicle < 1) ? 1 : ccicle;
    kavr = (kavr < 0.0f || kavr > 1.0f) ? 1.0f : kavr;
    if (optind + 1 > argc || fhelp) usage(argv[0]);

    inputname = argv[optind];
    if (optind + 2 > argc)
    {
        outputname = "-";
    } else {
        outputname = argv[optind + 1];
        if (strcmp(inputname, outputname) == 0)
        {
            fprintf(stderr, "ERROR: Cannot output to input %s\n", inputname);
            exit(EXIT_FAILURE);
        }
    }
    /* Open the input and output files */
    input_file = (strcmp("-", inputname) == 0) ? stdin : fopen(inputname, "rb");
    if (input_file == NULL)
    {
        fprintf(stderr, "ERROR: Can't open %s\n", inputname);
        exit(EXIT_FAILURE);
    }
    output_file = (strcmp("-", outputname) == 0) ? stdout : fopen(outputname, "wb");
    if (output_file == NULL)
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
    fprintf(stderr, "Read DCT coefficients successfully written to %s\n", inputname);

    /* Allocate memory for reading out DCT coeffs */
    for (compnum = 0; compnum < inputinfo.num_components; compnum++)
        coef_buffers[compnum] = ((&inputinfo)->mem->alloc_barray)
                            ((j_common_ptr) &inputinfo, JPOOL_IMAGE,
                             inputinfo.comp_info[compnum].width_in_blocks,
                             inputinfo.comp_info[compnum].height_in_blocks);

    /* Read input file as DCT coeffs */
    coef_arrays = jpeg_read_coefficients(&inputinfo);

    /* Copy compression parameters from the input file to the output file */
    jpeg_copy_critical_parameters(&inputinfo, &outputinfo);
    outputinfo.optimize_coding = TRUE;

    /* Copy DCT coeffs to a new array */
    int num_components = inputinfo.num_components;
    size_t block_row_size[num_components];
    int width_in_blocks[num_components];
    int height_in_blocks[num_components];
    for (compnum = 0; compnum < num_components; compnum++)
    {
        height_in_blocks[compnum] = inputinfo.comp_info[compnum].height_in_blocks;
        width_in_blocks[compnum] = inputinfo.comp_info[compnum].width_in_blocks;
        block_row_size[compnum] = (size_t) SIZEOF(JCOEF)*DCTSIZE2*width_in_blocks[compnum];
        for (rownum = 0; rownum < height_in_blocks[compnum]; rownum++)
        {
            row_ptrs[compnum] = ((&inputinfo)->mem->access_virt_barray)
                                ((j_common_ptr) &inputinfo, coef_arrays[compnum],
                                    rownum, (JDIMENSION) 1, FALSE);
            for (blocknum = 0; blocknum < width_in_blocks[compnum]; blocknum++)
            {
                for (i = 0; i < DCTSIZE2; i++)
                {
                    coef_buffers[compnum][rownum][blocknum][i] = row_ptrs[compnum][0][blocknum][i];
                }
            }
        }
    }

    fprintf(stderr, "Quant = %f\n", quant);
    quant = (quant < 0.0f || quant > 0.0f) ? 1.0f / quant : 1.0f;
    iquant = 1.0f / quant;
    iquant2 = 0.5f * iquant;
    sumcend = 0.0f;
    coefold = 0;
    for (ct = 0; ct < ccicle; ct++)
    {
        numc = 0;
        sumcec = 0.0;
        for (compnum = 0; compnum < num_components; compnum++)
        {
            for (rownum = 0; rownum < height_in_blocks[compnum]; rownum++)
            {
                for (blocknum = 0; blocknum < width_in_blocks[compnum]; blocknum++)
                {
                    for (i = 0; i < DCTSIZE2; i++)
                    {
                        coeforig = coef_buffers[compnum][rownum][blocknum][i];
                        coeferr = (coeforig < coefold) ? (coefold - coeforig) : (coeforig - coefold);
                        coefres = coeforig;
                        qsdelta = (coeforig < 0) ? -qdelta : qdelta;
                        if ((coeferr > iquant2) && (coeforig >= lower) && (coeforig <= upper))
                        {
                            coefres = (int)((float)coefres * quant + qsdelta);
                            coefres = (int)((float)coefres * iquant + qsdelta);
                            coeferr = (coeforig < coefres) ? (coefres - coeforig) : (coeforig - coefres);
                            coefres = (int)((float)coefres * kavr + (float)coeforig * (1.0 - kavr) + qsdelta);
                            coef_buffers[compnum][rownum][blocknum][i] = coefres;
                            sumcec += coeferr;
                        }
                        else
                        {
                            coeferr = 0;
                        }
                        numc++;
                        coefold = coefres;
                    }
                }
            }
        }
        if (numc > 0) sumcec /= numc;
        sumcend += sumcec;
    }
    sumcend *= kavr;
    fprintf(stderr, "QuantErr = %f\n", sumcend);

    /* Output the new DCT coeffs to a JPEG file */
    for (compnum = 0; compnum < num_components; compnum++)
    {
        for (rownum = 0; rownum < height_in_blocks[compnum]; rownum++)
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
    fprintf(stderr, "New DCT coefficients successfully written to %s\n\n", outputname);
    exit(jerr.num_warnings ? EXIT_WARNING : EXIT_SUCCESS);
    return 0; /* suppress no-return-value warnings */
}
