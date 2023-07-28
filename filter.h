#ifndef FILTER_H
#define FILTER_H

typedef struct {
    double old;
    double older;
    double out;
    double f0;
    double bw;
    double b ;
    double c ;

} tfilter ;


void filter_init(tfilter *filter);

double filter_compute(tfilter *filter, double in);

#endif