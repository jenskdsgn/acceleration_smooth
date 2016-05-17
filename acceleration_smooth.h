/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** acceleration_smooth.h
** Processes acceleration data and aggregates it. 
**
** Author: Jens Klein
** Version: 1.0
** -------------------------------------------------------------------------*/

#include "../Microsmooth/microsmooth.h"
#include <Arduino.h>

#ifndef _ACCELERATION_SMOOTH_H
#define _ACCELERATION_SMOOTH_H

#define AS_DEGREE_OF_AGGREGATION 10 

/* TYPEDEFS */
typedef void (*as_new_value_callback) ();
typedef struct as_avg_acc{
  unsigned long time_stamp;
  float avg_acc;
};

/* FUNCTIONS */
void as_set_new_value_callback(as_new_value_callback);
void as_push_acceleration_data(int8_t,int8_t,int8_t);
const as_avg_acc *as_get_avg_acc();


#endif