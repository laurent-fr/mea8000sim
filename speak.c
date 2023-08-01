/**
 * @file speak.c
 * @author Laurent FRANCOISE (@LaurentFr@piaille.fr)
 * @brief 
 * @version 0.2
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2023 Laurent FRANCOISE . All rights reserved. 
 *            This project is released under the GNU Public License.
 * 
 */

#include <stdio.h>

#include "speak.h"
#include "synth.h"

#define NB_PHONEME 39

const char phonemes_table[] = {
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




void show_help_speak() {

    printf("available keys :\n");
    printf("a e[lE] i o u &[è] *[é] E[eu] w[ou] A[en] I[un] O[on] W[oi] b d g[gu] j k l m\n");
    printf("n p R[Roule] r[bonjouR] s t v z $[ch] N[gn] @[ail] #[eil] %%[euil] <[ien] >[oin] 0[Or]\n");
    printf(";[silence 32ms] .[silence 64ms]\n\n");
}


void speak(unsigned char *buffer, char *sentence, void (*do_play)(double) ) {

    char *phonem = sentence;
    int init_ok=0;

    tsynth_state state;

    while(*phonem) {

        int sample_start;
        int sample_length;

        int num_sample=phonemes_table[*phonem-' '];

        if (num_sample>=0) {

            find_sample(buffer,num_sample,&sample_start,&sample_length);

            if (!init_ok) {
                init_state(buffer,sample_start,&state);
                init_ok=1;
            } else {
                set_pitch(buffer,sample_start,&state);
            }
        
            play_sample(buffer,sample_start,sample_length,&state,do_play);

        }

        phonem ++;
    }

}