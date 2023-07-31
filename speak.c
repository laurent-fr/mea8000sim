/**
 * @file phonems.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>

#include "speak.h"
#include "synth.h"

#define NB_PHONEM 39

const char phonems_table[] = {
    /* space ! " # $ % & ' { } * + , - . / */
    -1,-1,-1,32,29,33,5,-1,-1,-1,6,-1,-1,-1,38,-1,
    /* 0 1 2 3 4 5 6 7 8 9 : ; < = > ? */
    36,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,37,34,-1,35,-1,
    /* @ A B C D E F G H I J K L M N O */
    31,9,-1,-1,-1,7,-1,-1,-1,10,-1,-1,-1,-1,30,11,
    /* P Q R S T U V W X Y Z [ \ ] ^ _ */
    -1,-1,23,15,-1,-1,-1,12,-1,-1,-1,-1,-1,-1,-1,-1,
    /* ` a b c d e f g h i j k l m n o */
    -1,0,13,-1,14,1,-1,16,-1,2,17,18,19,20,21,3,
    /* p q r s t u v w x y z { | } ~ del */
    22,-1,24,25,26,4,27,8,-1,-1,28,-1,-1,-1,-1,-1,
};

const char *phonem_help[] = {
    "A\tpApA,rAt",
    "E\tlE",
    "I\tnavire,limace",
    "O,AU\tplatEAU,O,OH",
    "U\tlUne,prUne",
    "AI,EI,E,ET\tgAI,orEILLE",
    "é\tEcole,EclatER",
    "OU\tlOUp",
    "AN,EN\tnathAN",
    "IN,UN,AIN,EIN\tlapIN,sAINt",
    "ON\tlONg,sON",
    "OI",
    "B",
    "D",
    "G",
    "J",
    "K",
    "L",
    "M",
    "N",
    "P",
    "R1",
    "R2",
    "S",
    "T",
    "V",
    "Z",
    "CH",
    "GN",
    "AIL",
    "EIL",
    "EUIL",
    "IEN",
    "OIN",
    "O",
    "SILENCE 32ms",
    "SILENCE 64ms"
};


void show_help_speak() {

    // build phonems reverse table
    char key[NB_PHONEM];
    for(int i=0;i<(6*16);i++) {
        int num = phonems_table[i];
        if (num>=0) {
            key[num]=i+' '; 
        }
    }

    // display help
    for(int i=0;i<NB_PHONEM;i ++) {
        printf("[%c] %s\n",key[i],phonem_help[i]);
    }
  
}


void speak(unsigned char *buffer, char *sentence, void (*do_play)(double) ) {

    char *phonem = sentence;
    int init_ok=0;

    tsynth_state state;

    while(*phonem) {

        printf("%c",*phonem);

        int sample_start;
        int sample_length;

        int num_sample=phonems_table[*phonem-' '];
        printf(" %d ",num_sample);
        if (num_sample>=0) {

            find_sample(buffer,num_sample,&sample_start,&sample_length);

            if (!init_ok) {
                init_state(buffer,sample_start,&state);
            }
        
            play_sample(buffer,sample_start,sample_length,&state,do_play);

        }

        phonem ++;
    }

}