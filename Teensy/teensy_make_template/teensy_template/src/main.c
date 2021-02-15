/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"
#include "t_usb.h"
int main(void)
{
	m_usb_init();
	unsigned int value;
	m_usb_init();
	while(!m_usb_isconnected()); // wait for a connection
	while(1)
	{
	if(m_usb_rx_available())
		{
		value = m_usb_rx_char();
		m_usb_tx_uint(value);
		}
	}
  return 0;   /* never reached */
}
