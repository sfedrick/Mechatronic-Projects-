/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#define COMPAREVALUE 8000000
int testy(void)
{
	DDRC |= 0x40;
  set(TCCR3A,COM3A0);
  set(TCCR3B,WGM32);
  set(TCCR3B,CS30);
  OCR3A=COMPAREVALUE;
  while(1);
  return 0;
}
