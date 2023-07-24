#include <math.h>

#include "filter.h"
#include "bin.h"

tfilter filter[4];


/**
 * @brief generates a sawtooth waveform
 *        each call makes time advance 1/sample_frequency second
 * 
 * @param freq frequency in hertz
 * @param amp amplitude
 * @param sample_freq sample frequency in hertz
 * @param time time in second
 * @return double generator output
 */
double gen_sawtooth(double freq, double amp, double sample_freq, double *time) {

    double period=1/freq;
    double num_period = floor(*time/period);
    double start_of_period = period * num_period;
    double position_in_period = (*time-start_of_period)/period; // 0<=position_in_period<=1
    
    *time += 1/sample_freq;

    return position_in_period * amp ;
}

//double gen_noise()


void play_frame(tframe *frame, double sample_freq,double *time) {

    double time_end = *time + frame->fd;

    while(*time<time_end) {

    }

}

double synth_compute() {

    double out;

    for(int i=0;i<3 ;i++) {
        out = filter_compute(&filter[i],out);
    }

    return out;
}