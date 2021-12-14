/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: extra_switch.h
 * File description: external switch source file to terminal angle
 *                   measurement operation.
 * *******************************************************************/
#ifndef EXTRA_SWITCH_H_
#define EXTRA_SWITCH_H_
#include <stdbool.h>

/*********************************************************************
 * Macro Definition
 * *******************************************************************/
#define SWITCH_GPIO_PORT GPIOD
#define SWITCH_PIN (3)
#define SWITCH_PIN_CTRL_REG PORTD->PCR[SWITCH_PIN]
#define SWITCH_SCGC5_MASK SIM_SCGC5_PORTD_MASK
#define PRIORITY (3)
#define FALL_EDGE (10)

/*********************************************************************
 * Global variable
 * *******************************************************************/
extern bool switch_pressed;

/**********************************************************************
 * Function Name :init_switch()
 * Description: External switch initialization for terminating guage
 *              angle operation.
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void init_switch();

#endif /* EXTRA_SWITCH_H_ */
