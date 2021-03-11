/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"
#include "t_usb.h"
#include "PulseWidthModulation.h"
int main(void)
{

	m_usb_init();
	while(1)
	{
		float duty=0.5;
		int hertz=60;
	  //find the number of cycles
	  float	cycles=(preFreq*10)/(hertz);
	  // set when the output gets toggled off recall OCR1A only defines a quater
	  // cycle so we need to divide by 2 again
	  OCR0B=(int)(cycles*duty/2);
	 //set when the clock resets
	  OCR0A=(int)(OCR0B+((1-duty)*(cycles/2)));

	  // The clock toggles the output at OC1A on port D0 when
	  //the clock matches the value on OCR0B
	  //OC1B is multiplexed onto D0,
	  // which means that you must first set bit 0 of DDRD to enable output.
	  DDRD |=(1<<(0));

	  //sets the wave mode of the clock the clock resets at OCR3A
	  // when the clock resets it sets the output to 1
	  //	 UP to OCR0A, PWM mode
	  (TCCR0A) |= (1<<(WGM00));
	  (TCCR0A) |= (1<<(WGM01));
	  (TCCR0B) |= (1<<(WGM02));


	  // clear at OCR0B, set at OCR0A
	  (TCCR0A) |= (1<<(COM0B1));



	  // sets the prescaler for the clock and turns on clock
	  //set prescaler to /64
	  (TCCR0B) |= (1<<(CS02));

	  m_usb_tx_string("\n new value after waiting for 0.05 seconds \n");
	  m_usb_tx_string("\n OCRB value \n");
	  m_usb_tx_long(OCR0B);
	  m_usb_tx_string("\n OCRA value \n");
	  m_usb_tx_long(OCR0A);
	  m_usb_tx_string("\n time counter value \n");
	  m_usb_tx_int(TCNT0);



	}
  return 0;   /* never reached */
}
