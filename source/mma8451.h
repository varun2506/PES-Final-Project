/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: touch.C
 * File description: Initialize accelerometer and measure angle source file
 * Reference:Alexander Dean, Textbook- Chapter 8 Serial Communication
 * 			Textbook: Embedded Systems Fundamentals with ARM Cortex-M based Microcontrollers
 * *******************************************************************/
#ifndef MMA8451_H
#define MMA8451_H

/*********************************************************************
 * Macro Definition
 * *******************************************************************/
#define MMA_DEV_ADDR     (0x3A)
#define REG_OUT_Y_MSB    (0x03)
#define CTRL_REG1        (0x2A)
#define ANGLE_CONV       (180)
#define PI_NUM           (22)
#define PI_DEN           (7)
#define MSB_SHIFT        (8)
#define ADJUST_OUT       (2)
#define OUT_MSB_Y        (0)
#define OUT_LSB_Y        (1)
#define OUT_MSB_Z        (2)
#define OUT_LSB_Z        (3)
#define SENSITIVITY      (4096.0)
#define SET_MMA_ACTIVE   (0x01)
#define TOTAL_AXIS_BYTES (4)
#define LAST_AXIS_BYTE   (2)
#define SET              (1)
#define RESET            (0)
#define ACK_RXD          (0)
#define INIT_SUCCESS     (1)
#define INIT_FAILURE     (0)

extern int16_t AX;
extern int16_t AY;
extern int16_t AZ;
/**********************************************************************
 * Function Name :init_MMA();
 * Description: Initialize acceleromteter for angle measurement
 * param[In]  : void
 * param[out] : int
 * ********************************************************************/
int init_MMA();

/**********************************************************************
 * Function Name :compute_angle();
 * Description: Compute angle from the accelerometer
 * param[In]  : void
 * param[out] : int
 * ********************************************************************/
int compute_angle();


#endif
