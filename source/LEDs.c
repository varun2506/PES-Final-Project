/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: led.c
 * File description: Initialize LED for user interaction
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/
#include <MKL25Z4.H>
#include "LEDs.h"

/*********************************************************************
 * Macro Definition
 * *******************************************************************/
#define MASK(x) (1UL << (x))

/**********************************************************************
 * Function Name :Init_RGB_LEDs(void)
 * Description: Initialize RGB led
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void Init_RGB_LEDs(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | /* Enable clock to PORT B */
			      SIM_SCGC5_PORTD_MASK;  /* Enable clock to PORT D */
	
	PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;    	/* Initalize Red Led */
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);

	PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;    /* Initalize Green Led */
	PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1);

	PORTD->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;     /* Initalize Blue Led */
	PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(1);          
	
	PTB->PDDR |= MASK(RED_LED_POS) |    /* Set output ports */
			     MASK(GREEN_LED_POS);

	PTD->PDDR |= MASK(BLUE_LED_POS);    /* Set output ports */
}

/**********************************************************************
 * Function Name :Control_RGB_LEDs(unsigned int red_on, unsigned int green_on,
 *                                 unsigned int blue_on)
 * Description: Toggle Leds
 * param[In]  : red_on
 * param[In]  : green_on
 * param[In]  : blue_on
 * param[out] : void
 * ********************************************************************/
void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on) {
	if (red_on) {
			PTB->PCOR = MASK(RED_LED_POS);
	} else {
			PTB->PSOR = MASK(RED_LED_POS); 
	}
	if (green_on) {
			PTB->PCOR = MASK(GREEN_LED_POS);
	}	else {
			PTB->PSOR = MASK(GREEN_LED_POS); 
	} 
	if (blue_on) {
			PTD->PCOR = MASK(BLUE_LED_POS);
	}	else {
			PTD->PSOR = MASK(BLUE_LED_POS); 
	}
}	
