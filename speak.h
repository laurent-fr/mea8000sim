#ifndef SPEAK_H
#define SPEAK_H

void show_help_speak();

void speak(unsigned char *buffer, char *sentence, void (*do_play)(double) );

#endif 