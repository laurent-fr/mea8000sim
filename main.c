/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-28
 * 
 * @copyright Copyright (c) 2023
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

void show_help() {
    printf("mea8000sim <binfile> <numsample>\n");
}

void debug_out(double value) {
    printf("%f\n",value);
}

void add_to_out_buffer(double out) {
    out_buffer[out_buffer_length++]=out;
}

void save_wave(char *filename) {
    
    SF_INFO sfinfo ;
    sfinfo.channels = 1;
    sfinfo.samplerate = SAMPLE_FREQUENCY;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE *outfile = sf_open(filename, SFM_WRITE, &sfinfo);

    sf_write_double(outfile, out_buffer, out_buffer_length) ;
    sf_write_sync(outfile);
    sf_close(outfile);
}


int main(int argc, char *argv[]) {

    if (argc!=3) {
        show_help();
        exit(0);
    }

    char *filename=argv[1];
    sscanf(argv[2],"%d",&num_sample);

    bin_buffer_length = read_bin(filename,bin_buffer,MAX_BIN_BUFFER);

    printf("File %s, read %d bytes.\n",
        filename, bin_buffer_length);

    int sample_start;
    int sample_length;
    find_sample(bin_buffer,num_sample,&sample_start,&sample_length);

    printf("Sample %d : start offset %d, length %d.\n",
        num_sample,sample_start,sample_length);

    play_sample(bin_buffer,sample_start,sample_length,add_to_out_buffer);

    save_wave("output.wav");

    return 0;
}