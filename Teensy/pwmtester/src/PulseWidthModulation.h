/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
// you need to include teensy_general;
 //prescale frequency
 float preFreq=64000;

// hertz sets the desired frequency of the cycles 100 hertz is good
// duty sets the duty circle a number from 0 to 1.
// larger it is the longer the pin stays on

void DutyCall1(float duty,int hertz)
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


}





//use timer 3
void DutyCall2(float duty,int hertz)
{
		//find the number of cycles
		float	cycles=preFreq/(hertz);
		// set when the output gets toggled off recall OCR1A only defines a quater
		// cycle so we need to devide by 2 again
		OCR3A=(int)(cycles*duty/2);
   //set when the clock resets
		ICR3=(int)(OCR3A+((1-duty)*(cycles/2)));

		// The clock toggles the output at OC1A on port c6 when
		//the clock matches the value on OCR1A
		//OC1B is multiplexed onto c6,
		// which means that you must first set bit 6 of DDRC to enable output.
		DDRC |=(1<<(6));

		//sets the wave mode of the clock the clock resets at OCR3A
		// when the clock resets it sets the output to 1
    //(mode 14) UP to ICR3, PWM mode
		(TCCR3A) |= (1<<(WGM31));
		(TCCR3B) |= (1<<(WGM32));
		(TCCR3B) |= (1<<(WGM33));

		//make the clock toggle output OC3A when it reaches ICR3
    //clear at OCR3A, set at rollover
		(TCCR3A) |= (1<<(COM3A1));



		// sets the prescaler for the clock and turns on clock
    //set prescaler to /64
		(TCCR3B) |= (1<<(CS30));
		(TCCR3B) |= (1<<(CS31));


}



//use timer 0
void DutyCall3(float duty)
{
  int hertz=60;
  //find the number of cycles
  float	cycles=(preFreq*4)/(hertz);
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
