/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: touch.C
 * File description: Initialize accelerometer and measure angle source file
 * Reference:Alexander Dean, Textbook- Chapter 8 Serial Communication
 * 			Textbook: Embedded Systems Fundamentals with ARM Cortex-M based Microcontrollers
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/
#include <math.h>
#include <stdint.h>
#include "i2c.h"
#include "mma8451.h"
#include "MKL25Z4.h"

int16_t AX;
int16_t AY;
int16_t AZ;
/**********************************************************************
 * Function Name :init_MMA();
 * Description: Initialize acceleromteter for angle measurement
 * param[In]  : void
 * param[out] : int
 * ********************************************************************/
int init_MMA()
{
	/* Initialize the accelerometer in active mode */
	i2c_txByte(MMA_DEV_ADDR, CTRL_REG1, SET_MMA_ACTIVE);

	/* Wait for Acknowledgement and Initialization to be completed */
	if((I2C0->S & I2C_S_RXAK_MASK) ==ACK_RXD) {
		return 1;
	}
	else {
		return 0;
	}
}

/**********************************************************************
 * Function Name :compute_angle();
 * Description: Compute angle from the accelerometer
 * param[In]  : void
 * param[out] : int
 * ********************************************************************/
int compute_angle()
{
	/* variables to read axis value */
	int angle=RESET;
	int ack  =SET;
	uint8_t axis_value[TOTAL_AXIS_BYTES];
	int16_t y_axis_read,z_axis_read;
	float radians_y,radians_z;

	i2c_start_seq();   /* Initiate start sequence */

	i2c_read_addr(MMA_DEV_ADDR, REG_OUT_Y_MSB );  /* Read Address register */

	for( int i=RESET; i<TOTAL_AXIS_BYTES; i++)  /* read bytes with MSB of Y and Z axis */
	{
		axis_value[i] = i2c_read_reg(ack); /* Master transmit ack and register read */

		if(i==LAST_AXIS_BYTE)     /* Read last byte */
			ack=RESET;
	}

	/* Appending MSB to LSB of y and z axis measurements and adjusting values to 14 bit output values */
	y_axis_read=(((int16_t) ((axis_value[OUT_MSB_Y]<<MSB_SHIFT) | axis_value[OUT_LSB_Y]))>>ADJUST_OUT);
	z_axis_read=(((int16_t) ((axis_value[OUT_MSB_Z]<<MSB_SHIFT) | axis_value[OUT_LSB_Z]))>>ADJUST_OUT);

	/* Output values to 4096 counts */
	radians_y = y_axis_read/SENSITIVITY;
	radians_z = z_axis_read/SENSITIVITY;

	/* The resultant value is in radian, hence converted to angle */
	angle = atan2(radians_y, radians_z)*ANGLE_CONV*PI_DEN/PI_NUM;
	return angle;   /* Return calculated angle */
}

