/**
 * @file synth.h
 * @author Laurent FRANCOISE (@LaurentFr@piaille.fr)
 * @brief 
 * @version 0.2
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2023 Laurent FRANCOISE . All rights reserved. 
 *            This project is released under the GNU Public License.
 * 
 */

#ifndef SYNTH_H
#define SYNTH_H

#include "filter.h"
#include "bin.h"

typedef struct synth_state {

    double pitch;
    double amp;
    double frame_time;
    tfilter filter[4];
} tsynth_state ;


double gen_sawtooth(double freq);

double gen_noise();

void init_state(unsigned char *buffer, int start, tsynth_state *state);

void set_pitch(unsigned char *buffer, int start, tsynth_state *state);

void play_sample(unsigned char *buffer, int start,int length, tsynth_state *state, void (*do_play)(double) );

void play_frame(tframe *frame, tsynth_state *state, void (*do_play)(double) );

#endif