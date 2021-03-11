/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"
#include "t_usb.h"
#include "ADCsubroutines.h"
int main(void)
{
	m_usb_init();
	while(1){

	// an array that decides what pins are going to enable adc
	// each element in activate array corresponds to turning dc input on or off
	//activePins[12]={ADC0D,ADC1D,ADC4D,ADC5D,ADC6D,ADC7D,ADC8D,ADC9D,ADC10D,ADC11D,ADC12D,ADC13D};
	int activate[12]={1,0,0,0,0,0,0,0,0,0,0,0};

	ADCsetup(activate);

	//step 5 choose ADC channel
	m_usb_tx_string("\n channel chosen \n");
	int *channel=getbinary(1);
	choosechannel(channel);



	}
	return 0;
}
