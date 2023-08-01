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
#include <unistd.h>

#include <sndfile.h>

#include "config.h"

#include "bin.h"
#include "synth.h"
#include "speak.h"

unsigned char bin_buffer[MAX_BIN_BUFFER];
int bin_buffer_length;

double out_buffer[MAX_OUT_BUFFER];
int out_buffer_length = 0 ;

/**
 * @brief show standard help message
 * 
 */
void show_help() {
    printf("mea8000sim is a speech synthesis program.\n");
    printf("\t -b <binfile>\n");
    printf("\t -n <numsample>\n");
    printf("\t -o <output_wav_file>\n");
    printf("\t -s <string to speech>\n");
    printf("\t -h general help\n");
    printf("\t -H speak module help\n");

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

    if (argc==1) {
        show_help();
        exit(0);
    }

    // command line parameters
    char *bin_filename = NULL;
    char *say = NULL;
    int num_sample = -1;
    char *output_filename = NULL;
    
    int c;
    while (( c = getopt (argc, argv, "b:o:n:s:hH")) != -1) {
        switch(c) {
            case 'b':
                bin_filename = optarg;
                break;
            case 'o':
                output_filename = optarg;
                break;
            case 'n':
                sscanf(optarg,"%d",&num_sample);
                break;
            case 's':
                say = optarg;
                break;
            case 'h':
                show_help();
                exit(0);
            case 'H':
                show_help_speak();
                exit(0);

        }
    }

    // check input parameters
    if (bin_filename==NULL) {
        fprintf(stderr,"Error: -b option is required.\n\n");
        show_help();
        exit(-1);
    }

    if (output_filename==NULL) {
        fprintf(stderr,"Error: -o option is required.\n\n");
        show_help();
        exit(-1);
    }

    if ((num_sample==-1) && (say==NULL)) {
        fprintf(stderr,"Error: one of -n or -s option is required.\n\n");
        show_help();
        exit(-1);
    }


    if ((num_sample!=-1) && (say!=NULL)) {
        fprintf(stderr,"Error: -n and -s options are mutually exclusives.\n\n");
        show_help();
        exit(-1);
    }


    // load data file
    bin_buffer_length = read_bin(bin_filename,bin_buffer,MAX_BIN_BUFFER);

    printf("Data file %s, read %d bytes.\n",
        bin_filename, bin_buffer_length);


    if (num_sample==-1) {
        // speak(bin_buffer,"bOjwr.lE.mOd....jE.sui.I.pRogRam.dE.SIt&z.vokal..av&k.un.vW.dE.Robo.",add_to_out_buffer);
        speak(bin_buffer,say,add_to_out_buffer);
    } else {

        int sample_start;
        int sample_length;
        find_sample(bin_buffer,num_sample,&sample_start,&sample_length);

        printf("Sample %d : start offset %d, length %d.\n",
            num_sample,sample_start,sample_length);

        tsynth_state state;
        init_state(bin_buffer,sample_start,&state);

        play_sample(bin_buffer,sample_start,sample_length,&state,add_to_out_buffer);
    }

    // save the file
    save_wave(output_filename);

    return 0;
}