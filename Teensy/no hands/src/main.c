/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"
#include "t_usb.h"
int main(void)
{
	// initialize printing
	m_usb_init();
	//set c7 as an input
	DDRC&=~(1<<7);
	//make d1 an output
	DDRD|=(1<<1);
	//stores the input value that goes into c7
	int pinstate;

	while(1){
		//checks if the 7th bit is set
		pinstate=PINC &(1<<7);
		// if the transistor is in darkness turn on the light
		if(pinstate!=0){
			PORTD|=1<<1;
			m_usb_tx_string("I'm on");
			//while the transistor is in darkness keep the light on
			while(pinstate!=0){
				pinstate=PINC &(1<<7);
			}
		}
		else{
			//turns off the light if the transistor senses light
			PORTD &=~(1<<1);
		}
	}
	return 0;

}
