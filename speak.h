/**
 * @file speak.h
 * @author Laurent FRANCOISE (@LaurentFr@piaille.fr)
 * @brief 
 * @version 0.2
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2023 Laurent FRANCOISE . All rights reserved. 
 *            This project is released under the GNU Public License.
 * 
 */

#ifndef SPEAK_H
#define SPEAK_H

void show_help_speak();

void speak(unsigned char *buffer, char *sentence, void (*do_play)(double) );

#endif 