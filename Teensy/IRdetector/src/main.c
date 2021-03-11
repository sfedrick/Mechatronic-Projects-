/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"
#include "t_usb.h"
#include "subroutines.h"
int main(void)
{
	m_usb_init();
	unsigned int value;
	while(!m_usb_isconnected()); // wait for a connection
	//initialize usb output
		m_usb_init();
		// set the prescaler on the clock to /1024
		//so a clock frequency of 15.625 khz
		TCCR3B|=1<<(CS32);
		TCCR3B&=~(1<<(CS31));
		TCCR3B|=1<<(CS30);


		/* insert your hardware initialization here */
		// Make c7 the input pin by clearing bit 7 on DDRC
		DDRC &= ~(1<<(7));

		// set the pin D0 to set it as an output
		DDRD |= (1<<(0));
		// set the pin D1 to set it as an output
		DDRD |= (1<<(1));
		//set the output to be zero
		PORTD &=~(1<<(1));
		PORTD &=~(1<<(0));
//initialize variables
		int error=5;
		int check=10;
		int i;
		int state=0;
		int change=0;
		int avg_change;
	while(1){
		avg_change=0;
		for(i=0;i<check;i+=1){
			 change=WaitForChange();
			 if(change<0){
				 i-=1;
			 }
			 else{
				 avg_change+=change;
			 }
		}
		avg_change=avg_change/check;
		if(avg_change>5 && avg_change<20){
			PORTD |=(1<<(1));
			PORTD &=~(1<<(0));

		}
		else if(avg_change>70 && avg_change<100){
			PORTD |=(1<<(0));
			PORTD &=~(1<<(1));
		}
		else{
			PORTD &=~(1<<(1));
			PORTD &=~(1<<(0));
		}
		m_usb_tx_string("\n Average change: \n");
		m_usb_tx_int(avg_change);


	}
  return 0;   /* never reached */
}
