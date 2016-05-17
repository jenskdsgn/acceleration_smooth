/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** acceleration_smooth.cpp
** Processes acceleration data and aggregates it. 
**
** Author: Jens Klein
** Version: 1.0
** -------------------------------------------------------------------------*/


#include "acceleration_smooth.h"

// current heartrate structure 
//containing timestamp and hearrate
as_avg_acc avg_acc;

// Function that gets called when heartbeat is detected
as_new_value_callback new_as_value_callback;

//buffer
uint16_t *history_x = ms_init(SGA);
uint16_t *history_y = ms_init(SGA);
uint16_t *history_z = ms_init(SGA);
int8_t prev_x = 0;
int8_t prev_y = 0;
int8_t prev_z = 0;

/*
 * Sets the callback function when new avg acc data is available
 */
void as_set_new_value_callback(as_new_value_callback callback){
  new_as_value_callback = callback;
}

/*
 * Calculates the average value of n values and calls the callback, 
 * where n is AS_DEGREE_OF_AGGREGATION
 */
void averagize_processed_data(float new_value) {

  static float sum;
  static uint8_t cnt;

  if(cnt == AS_DEGREE_OF_AGGREGATION){
    new_as_value_callback ();
    avg_acc.time_stamp = millis();
    // convert to g
    // F  = (5V * SensorRead) / ( 2^10 * 0.33V*g^(-1))
    // => F [g] = SensorRead * 0.017964
    avg_acc.avg_acc = (sum/AS_DEGREE_OF_AGGREGATION)*0.0147964; 
    sum = 0;
    cnt = 0;
  } else {
    sum += new_value;
    cnt++;
  }
}

/*
 * given a 3D Vector with x,y,z values, this function calculates
 * the length of the vector as a floating point number
 * to not loose precision
 */
float calc_length_from_prev(int8_t x, int8_t y, int8_t z) {  
  return (
    sqrt(
      pow(x - prev_x,2) + 
      pow(y - prev_y,2) +
      pow(z - prev_z,2)
    )
  );  
}


/*
 * receives a new 3D vector consisting of x,y,z values to be filtered. 
 */
void as_push_acceleration_data(int8_t x, int8_t y, int8_t z) {  

  // ignore first 3 values
  static int8_t values_left_to_ignore = 3;

  int8_t processed_value_x = sga_filter(  x, history_x );
  int8_t processed_value_y = sga_filter(  y, history_y );
  int8_t processed_value_z = sga_filter(  z, history_z );

  if( values_left_to_ignore == 0 ) {
    averagize_processed_data(
      calc_length_from_prev(
        processed_value_x, 
        processed_value_y, 
        processed_value_z
      )
    );    
  } else {
    values_left_to_ignore--;
  }

  prev_x = processed_value_x; 
  prev_y = processed_value_y; 
  prev_z = processed_value_z;  
}

/*
 * Getter function that is callable outside the library
 * Can be called when new_value_callback was called.
 * Returns the pointer to the last detected heartbeat struct
 */
const as_avg_acc *as_get_avg_acc(){
  return &avg_acc;
}
