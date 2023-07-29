/**
 * @file bin.h
 * @author Laurent FRANCOISE (@LaurentFr@piaille.fr)
 * @brief 
 * @version 0.1
 * @date 2023-07-29
 * 
  * @copyright Copyright (c) 2023 Laurent FRANCOISE . All rights reserved. 
 *            This project is released under the GNU Public License.* 
 */

#ifndef BIN_H
#define BIN_H

typedef struct {
    double bw;
    double f0;
} tfilter_def;

typedef struct {
    double ampl; 
    double  fd; 
    double pi; 
    tfilter_def filter[4];

} tframe;

int read_bin(char *filename,unsigned char *buffer,int max_size);

void find_sample(unsigned char *buffer,int sample_num,int *start,int *length);

void decode_frame(unsigned char *buffer,int pos,tframe *frame);

void show_frame(tframe *frame) ;

#endif