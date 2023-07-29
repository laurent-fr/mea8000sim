/**
 * @file filter.h
 * @author Laurent FRANCOISE (@LaurentFr@piaille.fr)
 * @brief 
 * @version 0.1
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2023 Laurent FRANCOISE . All rights reserved. 
 *            This project is released under the GNU Public License.
 * 
 */

#ifndef FILTER_H
#define FILTER_H

typedef struct {
    double old;
    double older;
    double out;
    double f0;
    double bw;

} tfilter ;


void filter_init(tfilter *filter);

double filter_compute(tfilter *filter, double in);

#endif