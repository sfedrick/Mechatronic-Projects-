/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"

int main(void)
{
    /* insert your hardware initialization here */
		// Turn on the pull up resistor in the teensy
		DDRD |= (1<<(0));
		//set pin D0 to be an input by setting it
		PORTD|=1<<(0);
		// clear pin D1 to set it as an output
		PORTD &=~(1<<(1));
		while(1){
			//bit_is_set is used to find out if a bit is set
			//this checks if a PIND is set by doing a binary and operator
			//if the bit is in both the left and right operator it gets copied to the
			//output
			int pinstate=PIND & (1<<0);
			// check if the input is set
			if(pinstate==0){
				// turn on pin D1 if the button is pressed
				PORTD|=1<<(1);
			}
			else{
				// clear d0
				PORTD &= ~(1<<(1));
			}

		}



}
