/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
 // set the desired frequency of the cycles
 int	hertz=20;
 // set the duty how much of the cycle is spent on
 float duty=1;
 //prescale frequency
 float preFreq=250000;
#include "teensy_general.h"
int main(void)
{
		//find the number of cycles
		float	cycles=preFreq/(hertz);
		// set when the output gets toggled off recall OCR1A only defines a quater
		// cycle so we need to devide by 2 again
		OCR1A=(int)(cycles*duty/2);
   //set when the clock resets
		ICR1=(int)(OCR1A+((1-duty)*(cycles/2)));

		// The clock toggles the output at OC1A on port B5 when
		//the clock matches the value on OCR1A
		//OC1B is multiplexed onto B5,
		// which means that you must first set bit 5 of DDRB to enable output.
		DDRB |=(1<<(5));

		//sets the wave mode of the clock the clock resets at OCR1A
		// when the clock resets it sets the output to 1
		(TCCR1A) |= (1<<(WGM11));
		(TCCR1B) |= (1<<(WGM12));
		(TCCR1B) |= (1<<(WGM13));

		//make the clock toggle output OC1A when it reaches ICR1
		(TCCR1A) |= (1<<(COM1A1));



		// sets the prescaler for the clock and turns on clock
		(TCCR1B) |= (1<<(CS10));
		(TCCR1B) |= (1<<(CS11));
//run for ever
		while(1);
		return 0;


}
