/**
 * @file main.c
 * @author Laurent FRANCOISE (@LaurentFr@piaille.fr)
 * @brief 
 * @version 0.1
 * @date 2023-07-28
 * 
 * @copyright Copyright (c) 2023 Laurent FRANCOISE . All rights reserved. 
 *            This project is released under the GNU Public License.
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include <sndfile.h>

#include "bin.h"
#include "synth.h"

#include "config.h"

unsigned char bin_buffer[MAX_BIN_BUFFER];
int bin_buffer_length;
int num_sample;



double out_buffer[MAX_OUT_BUFFER];
int out_buffer_length = 0 ;

/**
 * @brief show standard help message
 * 
 */
void show_help() {
    printf("mea8000sim <binfile> <numsample> <output_wav_file>\n");
}

/**
 * @brief used as a callback function for play_sample, display each value on screen
 * 
 * @param value the input value
 */
void debug_out(double value) {
    printf("%f\n",value);
}

/**
 * @brief used as a callback function for play_sample, store 
 * 
 * @param out the input value
 */
void add_to_out_buffer(double out) {
    if (out_buffer_length>MAX_OUT_BUFFER) {
        fprintf(stderr,"Output buffer size %d exceeded !\n", MAX_OUT_BUFFER);
        exit(-1);
    }
    out_buffer[out_buffer_length++]=out;
}

/**
 * @brief saves the wav data stored in out_buffer into a file
 * 
 * @param filename the file name
 */
void save_wave(char *filename) {
    
    SF_INFO sfinfo ;
    sfinfo.channels = 1;
    sfinfo.samplerate = SAMPLE_FREQUENCY;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE *outfile = sf_open(filename, SFM_WRITE, &sfinfo);

    sf_write_double(outfile, out_buffer, out_buffer_length) ;
    sf_write_sync(outfile);
    sf_close(outfile);

    printf("Output file %s written.\n", filename);
}

/**
 * @brief the main function
 * 
 */
int main(int argc, char *argv[]) {

    // show help ?
    if (argc!=4) {
        show_help();
        exit(0);
    }

    // command line parameters
    char *filename=argv[1];
    sscanf(argv[2],"%d",&num_sample);
    char *output_filename=argv[3];

    bin_buffer_length = read_bin(filename,bin_buffer,MAX_BIN_BUFFER);

    printf("File %s, read %d bytes.\n",
        filename, bin_buffer_length);

    int sample_start;
    int sample_length;
    find_sample(bin_buffer,num_sample,&sample_start,&sample_length);

    printf("Sample %d : start offset %d, length %d.\n",
        num_sample,sample_start,sample_length);

    play_sample(bin_buffer,sample_start,sample_length,add_to_out_buffer);

    save_wave(output_filename);

    return 0;
}