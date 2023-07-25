#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "synth.h"



double interpolate(double value0,double value1,double time_now, double time_max);


/**
 * @brief generates a sawtooth waveform
 *       
 * @param freq frequency in hertz
 * @param amp amplitude
 * @param sample_freq sample frequency in hertz
 * @param time time in second
 * @return double generator output in [-1;1] range
 */
double gen_sawtooth(double freq, double amp, double sample_freq, double *time) {

    double period=1/freq;
    double num_period = floor(*time/period);
    double start_of_period = period * num_period;
    double position_in_period = (*time-start_of_period)/period; // 0<=position_in_period<=1
    
    return position_in_period * amp * 2 - 1; // -1<=out<=1
}

double gen_noise(double amp) {

    // should be replaced by a white noise generator
    return amp*(((double)rand()*2)/RAND_MAX - 1) ;
}



void init_state(tframe *frame, tsynth_state *state, double pitch) {
    state->pitch = pitch;
    state->amp = 0 ;
    state->filter[0].bw = frame->bw1;
    state->filter[1].bw = frame->bw2;
    state->filter[2].bw = frame->bw3;
    state->filter[3].bw = frame->bw4;
    state->filter[0].f0 = frame->fm1; 
    state->filter[1].f0 = frame->fm2; 
    state->filter[2].f0 = frame->fm3;
    state->filter[3].f0 = 3500;
}

void play_frame(tframe *frame, tsynth_state *state, double sample_freq,double *time,
  void (*do_play)(double) ) {


    state->frame_time = 0 ;

    tsynth_state initial_state;

    // copy the current state
    memcpy(&initial_state,state,sizeof(initial_state));

    while(state->frame_time < frame->fd) {

        double out;

        // generator
        if (frame->pi==-16) {
            out = gen_noise(state->amp);
        } else {
            out = gen_sawtooth(state->pitch,state->amp,sample_freq,time);
        }

        // filters
        //for(int i=0;i<3 ;i++) {
        //    out = filter_compute(state->filter[i],out);
        // }

        if (frame->pi != -16) {
            state->pitch+= frame->pi;
        }

        // update time relative to frame
        state->frame_time+= 1/sample_freq;

        // interpolate parameters
        state->amp = interpolate(initial_state.amp,frame->ampl,state->frame_time,frame->fd);
        // TODO : filters

        // update global time
        *time += 1/sample_freq;

        // execute the callback function
        do_play(out);

    }
}


double interpolate(double value0,double value1,double time_now, double time_max) {
    return value0+(value1-value0)*time_now/time_max;
}