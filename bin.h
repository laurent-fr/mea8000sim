#ifndef BIN_H
#define BIN_H

int read_bin(char *filename,char *buffer,int max_size);

void find_sample(char *buffer,int sample_num,int *start,int *length);

#endif