/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
#include <stdio.h>
#include "teensy_general.h"
#include "t_usb.h"
#include "ADCsubroutines.h"
#include "sorting.h"
#include "PulseWidthModulation.h"

// scale is used to scale up integer values so that I can divide them
// without losing too much information
int scale=100;
int hertz=30;
int wait=0;


//covert's a voltage measurement into a bit wise adc value
int AcDCValue(float value){
  value=(value/5.0)*1024;
  int answer=value;
  return answer;
}

// I use this function to find the angle and the desired pwm based on the input
//if the input is slightly above or below the pwm range then I floor it

int FindDutyAngle(int voltage[],int dutyAngle[],int index, int voltageRead, int size){
  int result;
  if(index<0){

    if(voltageRead<voltage[0]){
      result=dutyAngle[0];
    }
    else{
      result=dutyAngle[size];
    }

  }
  else{
    result=dutyAngle[index];
  }
  return (result/scale);
}


int main(void){
// sets the pwm range for the servors after expirementation this is what worked best
// these whole number represents percent values
int dutymax=7*scale;
int dutymax2=7*scale;
int dutymax3=2*scale;
int anglemax=50*scale;

//create input device voltage array for device 1
int min1=AcDCValue(1.75);
int max1=AcDCValue(2.10);
int size1=max1-min1;
int voltage1[size1+1];
int angle1[size1+1];
int duty1[size1+1];
//fill voltage1 with a linearly interpolated values from min to max adcvalue
linInterp(min1,max1,size1,voltage1);
linInterp(0,anglemax,size1,angle1);
linInterp(0,dutymax,size1,duty1);

//create input device voltage array for device 2
int min2=AcDCValue(1.80);
int max2=AcDCValue(2.15);
int size2=max2-min2;
int voltage2[size2+1];
int angle2[size2+1];
int duty2[size2+1];
linInterp(min2,max2,size2,voltage2);
linInterp(0,anglemax,size2,angle2);
linInterp(0,dutymax2,size2,duty2);


//create input device voltage array for device 3
int min3=AcDCValue(1.6);
int max3=AcDCValue(2);
int size3=max3-min3;
int voltage3[size3+1];
int angle3[size3+1];
int duty3[size3+1];
linInterp(min3,max3,size3,voltage3);
linInterp(0,anglemax,size3,angle3);
linInterp(0,dutymax3,size3,duty3);


//m_usb_init();
// an array that decides what pins are going to enable adc
// each element in activate array corresponds to turning dc input on or off
//activePins[12]={ADC0D,ADC1D,ADC4D,ADC5D,ADC6D,ADC7D,ADC8D,ADC9D,ADC10D,ADC11D,ADC12D,ADC13D};
// so an array of {1,1,1,0,0,0,0,0,0,0,0,0} will enable adc on pins ADC0D ADC1D and ADC4D
int activate[12]={1,1,1,0,0,0,0,0,0,0,0,0};
//sets up adc given the chosen adc pins
ADCsetup(activate);

//step 5 choose ADC channel the chanel is chosen by setting up an array of
// 4 zeros along with encoding the binary of the channel you want in that array
// passing a pointer to that binary array into choosechannel chooses the channel
int array0[4]={0,0,0,0};
int array1[4]={0,0,0,0};
int array4[4]={0,0,0,0};
int *channel0=getbinary(0,array0);
int *channel1=getbinary(1,array1);
int *channel2=getbinary(4,array4);
choosechannel(channel0);

//initialize important variables
int result;
int dutyResult;
int angleResult;
int mode=0;
int channelNum=3;
int index;
while(1)
{

	result=grabADC();
	if(result>=0 && mode%channelNum==0){
		mode++;
    index=binarySearch(voltage1, 0, size1, result);
    dutyResult=FindDutyAngle(voltage1,duty1,index, result, size1);
    angleResult=FindDutyAngle(voltage1,angle1,index, result, size1);
    choosechannel(channel1);

    DutyCall1(dutyResult/100.0,hertz);
    // this was used in part 3.1 to show the angle and desired duty cycle I keep it hear for
    //debugging convenience feel free to uncomment and veryify that the output is as expected
/*
    m_usb_tx_string("\n Trying servos Output pin channel 1 voltage \n");
    m_usb_tx_long(result);
    m_usb_tx_string("\n current angle \n");
    m_usb_tx_long(angleResult);
    m_usb_tx_string("\n desired duty cylce \n");
    m_usb_tx_long(dutyResult);
    teensy_wait(wait);
    */

	}
	else if(result>=0 && mode%channelNum==1){
		mode++;
    index=binarySearch(voltage2, 0, size2, result);
    dutyResult=FindDutyAngle(voltage2,duty2,index, result, size2);
    angleResult=FindDutyAngle(voltage2,angle2,index, result, size2);
    choosechannel(channel2);
    DutyCall2(dutyResult/100.0,hertz);
    // this was used in part 3.1 to show the angle and desired duty cycle I keep it hear for
    //debugging convenience feel free to uncomment and veryify that the output is as expected
    /*
    m_usb_tx_string("\n Output pin channel 2 voltage \n");
    m_usb_tx_long(result);
    m_usb_tx_string("\n current angle \n");
    m_usb_tx_long(angleResult);
    m_usb_tx_string("\n desired duty cylce \n");
    m_usb_tx_long(dutyResult);
    m_usb_tx_string("\n");
    teensy_wait(wait);
    */
	}
  else if(result>=0 && mode%channelNum==2){
		mode++;
    index=binarySearch(voltage3, 0, size3, result);
    dutyResult=FindDutyAngle(voltage3,duty3,index, result, size3);
    angleResult=FindDutyAngle(voltage3,angle3,index, result, size3);
    choosechannel(channel0);
    DutyCall3(dutyResult/100.0,hertz-5);
    // this was used in part 3.1 to show the angle and desired duty cycle I keep it hear for
    //debugging convenience feel free to uncomment and veryify that the output is as expected
    /*
    m_usb_tx_string("\n Output pin channel 3 voltage \n");
    m_usb_tx_long(result);
    m_usb_tx_string("\n current angle \n");
    m_usb_tx_long(angleResult);
    m_usb_tx_string("\n desired duty cylce \n");
    m_usb_tx_long(dutyResult);
    m_usb_tx_string("\n");
    */
	}
	else{

    m_usb_tx_string("\n not ready \n ");

	}

}
  return 0;   /* never reached */
}
