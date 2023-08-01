/**
 * @file synth.c
 * @author Laurent FRANCOISE (@LaurentFr@piaille.fr)
 * @brief 
 * @version 0.1
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2023 Laurent FRANCOISE . All rights reserved. 
 *            This project is released under the GNU Public License.
 * 
 */


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


/**
 * @brief init the synth state from initial pitch and data of first frame
 *        starting at buffer[start]
 * 
 * @param buffer a pointer to the data buffer
 * @param start the offset to the start of speach data
 * @param state a pointer to the synth stata
 */
void init_state(unsigned char *buffer, int start, tsynth_state *state) {

    set_pitch(buffer,start,state);
    
    tframe frame;
    decode_frame(buffer,start+1,&frame) ;

    state->amp = 0 ;
    for(int i=0;i<=3; i++) {
        state->filter[i].bw = frame.filter[i].bw;
        state->filter[i].f0 = frame.filter[i].f0;
        filter_init(&state->filter[i]);
    }

}

/**
 * @brief Set the absolute pitch in synth state
 * 
 * @param buffer a pointer to the data buffer
 * @param start the offset to the start of speach data
 * @param state a pointer to the synth stata
 */
void set_pitch(unsigned char *buffer, int start, tsynth_state *state) {
    // first byte of sample is initial amplitude (half of the frequency in Hz)
    int initial_pitch = buffer[start]*2;
    state->pitch = initial_pitch;
}

/**
 * @brief debug function, shows the content of synth state
 * 
 * @param state a pointer to a state
 */
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
 * @param buffer the input buffer
 * @param start start offset of the sample
 * @param length length of the sample
 * @param state a pointer to the synth state
 * @param do_play a pointer to a function executed each time new data is generated
 *                ( SAMPLE_FREQUENCY times per second )
 */
void play_sample(unsigned char *buffer, int start,int length, tsynth_state *state, void (*do_play)(double) ) {

    tframe frame;

    // decode & play each frame
    for(int pos=start+1;pos<start+length;pos+=4) {
        decode_frame(buffer,pos,&frame) ;
        play_frame(&frame, state, do_play );

    }

}

/**
 * @brief generates audio data for a single data frame
 * 
 * @param frame a pointer to a data frame
 * @param state a pointer to the current synth state
 * @param do_play a pointer to a function executed each time new data is generated
 *                ( SAMPLE_FREQUENCY times per second )
 */
void play_frame(tframe *frame, tsynth_state *state, void (*do_play)(double) ) {

    state->frame_time = 0 ;

    tsynth_state initial_state;

    // copy of the current state
    memcpy(&initial_state,state,sizeof(initial_state));

    // uncomment to see the internal states
    //show_state(state);
    //show_frame(frame);

    // process frame
    while(state->frame_time < frame->fd) {

        double out;

         // interpolate parameters
        state->amp = interpolate(initial_state.amp,frame->ampl,state->frame_time,frame->fd);
        for (int i=0;i<=3;i ++) {
            state->filter[i].f0 = interpolate(initial_state.filter[i].f0,
                frame->filter[i].f0,state->frame_time,frame->fd);
             state->filter[i].bw = interpolate(initial_state.filter[i].bw,
                frame->filter[i].bw,state->frame_time,frame->fd);
       
        }
       
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

        // volume adjust
        out*=VOLUME;

        // clip the output value in interval [-1;1]
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

/**
 * @brief internal helper function, interpolates the value between value0 and value1
 *        where value0 corresponds to time=0 and value1 corresponds to time=time_max
 * 
 * @param value0 the start value
 * @param value1 the end value
 * @param time_now the current time
 * @param time_max the maximum time value
 * @return double the interpolated value
 */
double interpolate(double value0,double value1,double time_now, double time_max) {
    return value0+(value1-value0)*time_now/time_max;
}