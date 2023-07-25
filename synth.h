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


double gen_sawtooth(double freq, double amp, double sample_freq, double *time);

double gen_noise(double amp);

void init_state(tframe *frame, tsynth_state *state, double pitch);

void play_frame(tframe *frame, tsynth_state *state, double sample_freq,double *time,
  void (*do_play)(double)  );

#endif