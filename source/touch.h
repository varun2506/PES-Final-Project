/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: touch.C
 * File description: Configure touch sensor for calibration
 * *******************************************************************/
#ifndef TOUCH_H_
#define TOUCH_H_

/*********************************************************************
 * Header File
 * *******************************************************************/
#include <stdint.h>

/*********************************************************************
 * Global Variable
 * *******************************************************************/
extern volatile uint32_t touch_val;

/*********************************************************************
 * Macro Definition
 * *******************************************************************/
#define TOUCH_OFFSET 	(570)	  	//Offset for scan range
#define TOUCH_DATA 		(TSI0->DATA & 0xFFFF) // Accessing TSI0_DATA_TSICNT of TSI0_DATA
#define MIN_INPUT		(60)		//Minimum THreshold for TSI touch

/**********************************************************************
 * Function Name :init_TSI(void)
 * Description: Initialize touch sensor for calibration
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void init_TSI(void);


#endif /* TOUCH_H_ */
