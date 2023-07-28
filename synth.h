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

void init_state(tframe *frame, tsynth_state *state, double pitch);

void play_sample(unsigned char *buffer, int start,int length ,  void (*do_play)(double)  );

void play_frame(tframe *frame, tsynth_state *state,double *time, void (*do_play)(double)  );

#endif