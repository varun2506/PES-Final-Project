/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: led.h
 * File description: Initialize LED for user interaction
 * *******************************************************************/
#ifndef LEDS_H
#define LEDS_H

/*********************************************************************
 * Macro Definition
 * *******************************************************************/
#define RED_LED_POS   (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS  (1)		// on port D

/**********************************************************************
 * Function Name :Init_RGB_LEDs(void)
 * Description: Initialize RGB led
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void Init_RGB_LEDs(void);

/**********************************************************************
 * Function Name :Control_RGB_LEDs(unsigned int red_on, unsigned int green_on,
 *                                 unsigned int blue_on)
 * Description: Toggle Leds
 * param[In]  : red_on
 * param[In]  : green_on
 * param[In]  : blue_on
 * param[out] : void
 * ********************************************************************/
void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on);
//void Toggle_RGB_LEDs(unsigned int red, unsigned int green, unsigned int blue);

#endif
