#include <stdio.h>
#include <stdlib.h>

#include "bin.h"

/**
 * @brief length of a frame in second
 * 
 */
double fd_table[] = {
    0.008, 0.016, 0.032, 0.064
};

double amplitude_table[] = {
    0, 0.008, 0.011, 0.016, 0.022, 0.032, 0.044, 0.062,
    0.088, 0.125, 0.177, 0.25, 0.354, 0.5, 0.707, 1
};

double fm1_table[] = {
    150, 162, 174, 188, 202, 217, 233, 250, 
    267, 286, 305, 325, 346, 368, 391, 415,
    440, 466, 494, 523, 554, 587, 622, 659,
    698, 740, 784, 830, 880, 932, 988, 1047
};

double fm2_table[] = {
    440, 466, 494, 523, 554, 587, 622, 659,
    698, 740, 784, 830, 880, 932, 988, 1047,
    1110, 1179, 1254, 1337, 1428, 1528, 1639, 1761,
    1897, 2047, 2214, 2400, 2609, 2842, 3105, 3400
};

double fm3_table[] = {
    1179, 1337, 1528, 1761, 2047, 2400, 2842, 3400
};

double bw_table[] = {
    726, 309, 125, 50
};

/**
 * @brief load a MEA8000 data file into memory buffer
 * 
 * @param filename input file name
 * @param buffer output buffer
 * @param max_size size limit
 * @return int number of bytes read
 */
int read_bin(char *filename,unsigned char *buffer,int max_size) {

    FILE *fp;

    fp=fopen(filename,"rb");
    if (fp==NULL) {
        perror("Cannot open file");
        exit(-1);
    }

    // file length
    fseek(fp,0,SEEK_END);
    int length = ftell(fp);
    fseek(fp,0,SEEK_SET);

   if (length>max_size) {
        fprintf(stderr,"file size too big (%d>%d).",length,max_size);
   }

    int n = fread(buffer,1,length,fp);
    fclose(fp);

    return n;
}

/**
 * @brief find a sample in the buffer given its number
 *        the format of the data is the following : 
 *        - a table of 2 bytes data :
 *            2 bytes = offset of sample from start of data
 *            if data = FF FF -> end of table
 *        - the samples, each with
 *            4 bytes header :
 *              2 bytes = sample length
 *              1 byte = 00
 *              1 byte = initial pitch 
 *            a liste of 4 bytes frames 
 * 
 * @param buffer the input buffer
 * @param sample_num the sample number (<128)
 * @param start output the start position of the sample (points to "initial pitch" byte)
 * @param length output the length of the sample (only the frames)
 */
void find_sample(unsigned char *buffer,int sample_num,int *start,int *length) {

    int sample_offset = buffer[sample_num*2]*256 + buffer[sample_num*2+1];

    *length = buffer[sample_offset]*256 + buffer[sample_offset+1] - 3;
    *start =  sample_offset + 3;
}

/**
 * @brief decode the 4 data bytes of a frame
 *        - byte 1 = AABBCCDD , A=bw1, B=bw2, C=bw2, D=bw3
 *        - byte 2 = EEEFFFFF , E=fm3, F=fm2
 *        - byte 3 = GGGGGHHH , G=fm1 
 *        - byte 4 = HIIJJJJJ , H(from bytes 3&4)=ampl, I=fd J=pi
 * 
 * @param buffer 
 * @param pos 
 * @param frame 
 */
void decode_frame(unsigned char *buffer,int pos,tframe *frame) {

    unsigned char bw1 = buffer[pos] >> 6;
    unsigned char bw2 = (buffer[pos] >> 4) & 3 ;
    unsigned char bw3 = (buffer[pos] >> 6) & 3 ;
    unsigned char bw4 = buffer[pos] & 3 ;
    unsigned char fm3 = buffer[pos+1] >> 5;
    unsigned char fm2 = buffer[pos+1] & 31;
    unsigned char fm1 = buffer[pos+2] >> 3;
    unsigned char ampl = ((buffer[pos+2] & 7)<<1) + (buffer[pos+1]>>7) ;
    unsigned char fd = (buffer[pos+3]>>5) & 3;
    unsigned char pi = buffer[pos+3] & 31 ;

    frame->bw1=bw_table[bw1];
    frame->bw2=bw_table[bw2];
    frame->bw3=bw_table[bw3];
    frame->bw4=bw_table[bw4];
    frame->fm3=fm3_table[fm3];
    frame->fm2=fm2_table[fm2];
    frame->fm1=fm1_table[fm1];
    frame->ampl=amplitude_table[ampl];
    frame->fd=fd_table[fd];
    if (pi<16) {
        frame->pi = pi;
    } else {
        frame->pi = pi - 32;
    }


}

/**
 * @brief 
 * 
 * @param frame 
 */
void show_frame(tframe *frame) {

    printf("ampl=%f, pi=%f, fd=%f , f1/bw1=%f/%f f2/bw2=%f/%f f3/bw3=%f/%f bw4=%f\n",
        frame->ampl, frame->pi, frame->fd,
        frame->fm1, frame->bw1,
        frame->fm2, frame->bw2,
        frame->fm3, frame->bw3,
        frame->bw4);

}