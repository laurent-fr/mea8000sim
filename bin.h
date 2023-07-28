#ifndef BIN_H
#define BIN_H


typedef struct {
    double bw1;
    double bw2;    
    double bw3;
    double bw4;
    double fm3;
    double fm2;
    double fm1;
    double ampl; 
    double  fd; 
    double pi; 

} tframe;

int read_bin(char *filename,unsigned char *buffer,int max_size);

void find_sample(unsigned char *buffer,int sample_num,int *start,int *length);

void decode_frame(unsigned char *buffer,int pos,tframe *frame);

void show_frame(tframe *frame) ;

#endif