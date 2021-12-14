/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: extra_switch.c
 * File description: external switch source file to terminal angle
 *                   measurement operation.
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/
#include <stdbool.h>
#include "MKL25Z4.h"
#include "extra_switch.h"

/*********************************************************************
 * Global Variable
 * *******************************************************************/
bool switch_pressed;

/**********************************************************************
 * Function Name :init_switch()
 * Description: External switch initialization for terminating guage
 *              angle operation.
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void init_switch()
{

	SWITCH_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK; /* Clear and set PORT D PIN 3 */

	SWITCH_PIN_CTRL_REG |= PORT_PCR_MUX(1);
	SWITCH_PIN_CTRL_REG |= PORT_PCR_PE(1) |
			               PORT_PCR_PS(1) |            /* Enable pull up registor  */
						   PORT_PCR_IRQC(FALL_EDGE) |  /* Interrupt on falling edge */
						   PORT_PCR_ISF(0);

	SWITCH_GPIO_PORT->PDDR &= ~(1 << SWITCH_PIN); /* Set port direction */

	NVIC_SetPriority(PORTD_IRQn, PRIORITY);       /* Set Interrupt priority */

	NVIC_EnableIRQ(PORTD_IRQn);      /* Enable Interrupt */
}

/**********************************************************************
 * Function Name :PORTD_IRQHandler(void)
 * Description: Switch pressed interrupt handler
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void PORTD_IRQHandler(void)
{
	NVIC_DisableIRQ(PORTD_IRQn);  /* Disable Interrupt */

	switch_pressed=PORTD->ISFR & (1 << SWITCH_PIN); /* Set the Interrupt flag and clear the interrupt */

	SWITCH_PIN_CTRL_REG |=PORT_PCR_ISF(0); /* Clear Interrupt flag */

	NVIC_EnableIRQ(PORTD_IRQn);            /* Enable Interrupt */
}
