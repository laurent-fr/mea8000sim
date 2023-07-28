#include <math.h>

#include "filter.h"

#ifndef M_PI
    #define M_PI 3.1415926535
#endif 

void filter_init(tfilter *filter) {
    
    filter->old = filter->older = filter->out =0;
    
}

double filter_compute(tfilter *filter, double in) {

    filter->b = 2*cos(2*M_PI* filter->f0/F_SAMPLE);
    filter->c = -exp(-1*M_PI* filter->bw/F_SAMPLE);

    filter->out = in ;
    filter->out -= filter->c * (filter->b * filter->old + filter->c * filter->older);

    filter->older = filter->old; 
    filter->old = filter->out ; 

    return filter->out;
}


