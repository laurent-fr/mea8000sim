#include <math.h>

#include "filter.h"

#ifndef M_PI
    #define M_PI 3.1415926535
#endif 

void filter_set(tfilter *filter , double f0, double bw ) {
    
    filter->old = filter->older = filter->out =0;
    filter->f0 = f0;
    filter->bw = bw; 
    filter->b = 2*cos(2*M_PI*f0/F_SAMPLE);
    filter->c = exp(-2*M_PI*bw/F_SAMPLE);

}

double filter_compute(tfilter *filter, double in) {

    filter->out = filter->old * filter->c + filter->older * filter->b;
    filter->out *= -filter->b;

    filter->older = filter->old; 
    filter->old = filter->out ; 

    return filter->out;
}


