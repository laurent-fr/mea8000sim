#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "synth.h"
#include "config.h"


double interpolate(double value0,double value1,double time_now, double time_max);


/**
 * @brief generates a sawtooth waveform
 *       
 * @param freq frequency in hertz
 * @return double generator output in [-1;1] range
 */
double gen_sawtooth(double freq) {

    static double output=0;

    output += 2*freq/SAMPLE_FREQUENCY;

    if (output>1) {
        output = -1;
    }

    return output;
}

/**
 * @brief generate random noise
 * 
 * @return double generator output in [-1;1] range
 */
double gen_noise() {

    return ((double)rand()*2)/RAND_MAX - 1 ;
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

void show_state(tsynth_state *state) {
    printf("> ampl=%f, pi=%f ",
        state->amp, state->pitch);
    for(int i=0;i<=3;i++) {
        printf("f%d/bw%d= %f/%f ",i,i,state->filter[i].f0,state->filter[i].bw);
    }
    printf("\n");
}

/**
 * @brief play sample from data in buffer[start] to buffer [start+length]
 *        call the do_play function each type a new data is generated 
 *        (each 1/SAMPLE_FREQUENCY second)
 * 
 * @param buffer 
 * @param start 
 * @param length 
 * @param do_play 
 */
void play_sample(unsigned char *buffer, int start,int length, void (*do_play)(double)  ) {

    // first byte of sample is initial amplitude (half of the frequency in Hz)
    int initial_pitch = buffer[start]*2;

    tframe frame;
    tsynth_state state;
    double time = 0;

    init_state(&frame,&state,initial_pitch);
    
    for(int pos=start+1;pos<start+length;pos+=4) {
        decode_frame(buffer,pos,&frame) ;
        play_frame(&frame, &state, &time, do_play );

    }

}

void play_frame(tframe *frame, tsynth_state *state,double *time, void (*do_play)(double) ) {

    state->frame_time = 0 ;

    tsynth_state initial_state;

    // copy of the current state
    memcpy(&initial_state,state,sizeof(initial_state));

    show_state(state);
    show_frame(frame);

    for(int i=0;i<=3;i ++) {
        filter_init(&state->filter[i]);
    }

    while(state->frame_time < frame->fd) {

        double out;

         // interpolate parameters
        state->amp = interpolate(initial_state.amp,frame->ampl,state->frame_time,frame->fd);
        state->filter[0].f0 = interpolate(initial_state.filter[0].f0,frame->fm1,state->frame_time,frame->fd);
        state->filter[1].f0 = interpolate(initial_state.filter[1].f0,frame->fm2,state->frame_time,frame->fd);
        state->filter[2].f0 = interpolate(initial_state.filter[2].f0,frame->fm3,state->frame_time,frame->fd);
        state->filter[3].f0 = 3500;
        state->filter[0].bw = interpolate(initial_state.filter[0].bw,frame->bw1,state->frame_time,frame->fd);
        state->filter[1].bw = interpolate(initial_state.filter[1].bw,frame->bw2,state->frame_time,frame->fd);
        state->filter[2].bw = interpolate(initial_state.filter[2].bw,frame->bw3,state->frame_time,frame->fd);
        state->filter[3].bw = interpolate(initial_state.filter[3].bw,frame->bw4,state->frame_time,frame->fd);

        // generator
        if (frame->pi==-16) {
            out = gen_noise();
        } else {
            out = gen_sawtooth(state->pitch);
        }

        //  amplificator
        out *= state->amp;

        // filters
        for(int i=0;i<=3 ;i++) {
            out = filter_compute(&state->filter[i],out);
        }

        // adjust pitch
        if (frame->pi != -16) { // -16 is noise
            state->pitch+= frame->pi / (0.008 * SAMPLE_FREQUENCY); // frame->pi unit is Hz/8ms
        }

        // update time relative to frame
        state->frame_time+= 1.0/SAMPLE_FREQUENCY;

        // update global time
        *time += 1.0/SAMPLE_FREQUENCY;

        // volume adjust
        out*=VOLUME;

        // clip output
        if (out > 1.0) {
            out = 1.0;
        }

        if (out < -1.0) {
            out = -1.0;
        }

        // execute the callback function
        do_play(out);

    }
}


double interpolate(double value0,double value1,double time_now, double time_max) {
    return value0+(value1-value0)*time_now/time_max;
}