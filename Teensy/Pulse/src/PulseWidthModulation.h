/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
// you need to include teensy_general;
 //prescale frequency
 float preFreq=250000;

// hertz sets the desired frequency of the cycles 100 hertz is good
// duty sets the duty circle a number from 0 to 1.
// larger it is the longer the pin stays on

int DutyCall(float duty,int hertz)
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
    return 0;

}

// make an output pin pulse based on a given pulse array
// first collumn represents time and second collumn represents an intensity
// by changing the duty cycle on the output of a pwm pin
//int size is the number of rows
//int step tells how fine the linear interpolation is
// frequency is the frequency you want the pin to be osscilating at 100 herts
//works well

int pulse(float parray[][2],int size, int step, int frequency, float max){
//iterates through pulse array
  int i;
//defines the time step
  float t0;
// defines how long you wait for a intepolated intensity
  float dutywait;
// defines the start of the intensity ramp
  float intensityStart;
// defines the end of intensity ramp
  float intensityEnd;
// defines the interpolation step from one intensity to the next
  float intensityStep;
// the current intensity
  float intensity;
// counter to step through linear interpolations
  int s;
  //step through rows in array
  for(i=0;i<size-1;i+=1){
    // find how long we wait for element in the pulse array
    t0=parray[i+1][0]-parray[i][0];
    //find how long we wait for each interpolation between elements of
    //pulse array
    dutywait=t0/step;
    //defines the start intensity
    intensityStart=parray[i][1];
    //defines the end instensity
    intensityEnd=parray[i+1][1];
    //defines the intensity step
    intensityStep=(intensityEnd-intensityStart)/step;
    //the first intensity
    intensity=intensityStart;
    for(s=0; s<step;s++){
      intensity=intensity+intensityStep;
      DutyCall(intensity*max,frequency);
      teensy_wait(dutywait*1000);

    }

  }
 return 0;
}
