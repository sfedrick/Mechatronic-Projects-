/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"
#include "t_usb.h"

int main(void)
{ //initialize usb output
	m_usb_init();
	// set the prescaler on the clock to /1024
	//so a clock frequency of 15.625 khz
	TCCR3B|=1<<(CS32);
	TCCR3B&=~(1<<(CS31));
	TCCR3B|=1<<(CS30);


	/* insert your hardware initialization here */
	// Make c7 the input pin by clearing bit 7 on DDRC
	DDRC &= ~(1<<(7));
	// set the pin D1 to set it as an output
	DDRD |= (1<<(1));
	PORTD &=~(1<<(1));


	int pinstate;
	int press=0;
	int mode;
	unsigned int oldtime=0;
	unsigned int newtime=0;
	float dt=0;
	float currentdt=0;
	float averagepress;
	while(1){
		//bit_is_set is used to find out if a bit is set
		//this checks if a PIND is set by doing a binary and operator
		//if the bit is in both the left and right operator it gets copied to the
		//output
		pinstate=PINC & (1<<7);

		// check if the input is set
		if(pinstate==0){
			//incriment the press counter
			press+=1;
			//cycle through the button presses using mode
			mode=press%5;
			// turn on pin D1 if the button is pressed
			PORTD|=1<<(1);
			//while the button is pressed remain on
			while(pinstate==0){
			pinstate=PINC & (1<<7);
			};
			//mode 0 is the initial mode

			if(mode==0){
					oldtime=newtime;
					newtime=(unsigned int)ICR3;

				//check for a rollover if it didn't do a normal subtraction to find else
				//do a subtraction for a rollover
				if(newtime>oldtime){
					dt+=(newtime-oldtime);
				}
				else{
					dt+=(65536.0-oldtime)+newtime;

				}
				//print out the average presses and prompt the user to try again

					averagepress=(((float)dt)/4.0)*(1.0/15625.0)*(1000.0);
					m_usb_tx_string("\n press number:");
					m_usb_tx_uint(5);
					m_usb_tx_string("\n Cycles:");
					m_usb_tx_uint(ICR3);
					m_usb_tx_string("\n Previous average press \n");
					m_usb_tx_int(averagepress);
						m_usb_tx_string("\t milliseconds\n");
					m_usb_tx_string("\n Press the button to begin \n");
					m_usb_tx_string("Press the button 5 times as fast as you can \n");
					press=0;
					dt=0;
					currentdt=0;
			}
			//you can't find dt for the first button press just print out where the button stats
			else if(mode==1){

				oldtime=0;
				newtime=(unsigned int)ICR3;
				dt=0;
				currentdt=0;
				m_usb_tx_string("\n press number:");
				m_usb_tx_uint(mode);
				m_usb_tx_string("\n Cycles:");
				m_usb_tx_uint(ICR3);
			}
			else{
				//set the oldtime and newtime

					oldtime=newtime;
					newtime=(unsigned int)ICR3;

				//check for a rollover if it didn't do a normal subtraction to find dt
				if(newtime>oldtime){
					dt+=(newtime-oldtime);
				}
				else{
					dt+=(65536.0-oldtime)+newtime;

				}
				//check for a rollover if it didn't do a normal subtraction to find else
				//do a subtraction for a rollover
				m_usb_tx_string("\n press number:");
				m_usb_tx_uint(mode);
				m_usb_tx_string("\n Cycles:");
				m_usb_tx_uint(ICR3);
			}

		}
		else {
			// clear d1
			PORTD &= ~(1<<(1));

		}

	}





return 0;
}
