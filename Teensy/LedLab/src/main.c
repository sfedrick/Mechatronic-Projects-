/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
/* Sets the total time of the blinking period*/
int BlinkTime=5000;
/*Sets the duty cycle from zero to 1 percent
higher percentage means it is on for longer
*/
float duty=0.50;

int main(void)
{
    teensy_clockdivide(0); //set the clock speed
    teensy_wait(1000);		// wait 1000 ms when at 16 MHz
    /* insert your hardware initialization here */
    for(;;){
      float ontime=BlinkTime*duty;
      float offtime=BlinkTime*(1-duty);
      //turns on pin d0
      PORTD=0x01;
      teensy_wait(ontime);
      turns off all pins
      PORTD=0x00;
      teensy_wait(offtime);
    }

    return 0;   /* never reached */
}
