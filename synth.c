#include "filter.h"


tfilter filter[4];



double synth_compute() {

    double out;

    for(int i=0;i<3 ;i++) {
        out = filter_compute(&filter[i],out);
    }

    return out;
}