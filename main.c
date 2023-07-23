#include <stdio.h>
#include <stdlib.h>

#include "bin.h"

#define MAX_BIN_BUFFER 16000

unsigned char bin_buffer[MAX_BIN_BUFFER];
int bin_buffer_length;
int num_sample;

void show_help() {
    printf("mea8000sim <binfile> <numsample>\n");
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

    return 0;
}