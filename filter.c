/**
 * @file filter.c
 * @author Laurent FRANCOISE (@LaurentFr@piaille.fr)
 * @brief 
 * @version 0.2
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2023 Laurent FRANCOISE . All rights reserved. 
 *            This project is released under the GNU Public License.
 * 
 */

#include <math.h>

#include "filter.h"
#include "config.h"

#ifndef M_PI
    #define M_PI 3.1415926535
#endif 

/**
 * @brief set initial state of a filter
 * 
 * @param filter a pointer to filter data
 */
void filter_init(tfilter *filter) {

    filter->old = filter->older = filter->out =0;
}

/**
 * @brief compute the 2nd order filter
 * 
 * @param filter a pointer to filter data
 * @param in data in
 * @return double data out
 */
double filter_compute(tfilter *filter, double in) {

    double b = 2*cos(2*M_PI* filter->f0/SAMPLE_FREQUENCY);
    double c = -exp(-1*M_PI* filter->bw/SAMPLE_FREQUENCY);

    filter->out = in ;
    filter->out -= c * (b * filter->old + c * filter->older);

    filter->older = filter->old; 
    filter->old = filter->out ; 

    return filter->out;
}


