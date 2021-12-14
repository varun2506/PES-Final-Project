/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: UART.h
 * File description: UART header file
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/
#ifndef UART_H_
#define UART_H_

#include "MKL25Z4.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/*********************************************************************
 * Macro Definition
 * *******************************************************************/
#define UART_BAUDRATE	        (38400)
#define UART_DATA_BIT 			(0)
#define UART_PARITY_BIT			(0)
#define UART_STOP_BIT			(1)

#define UART_OVERSAMPLE_RATE 	(15)
#define BUS_CLOCK 				(24e6)
#define SYS_CLOCK				(24e6)

/**********************************************************************
 * Function Name :Uart_Init(uint32_t baud_rate);
 * Description: Initialize UART for baud rate, data size, parity and Stop bit
 * param[In]  : baud_rate
 * param[out] : void
 * ********************************************************************/
void Uart_Init(uint32_t baud_rate);

/**********************************************************************
 * Function Name :Start_TxRx();
 * Description: Uart to command line interface
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void Start_TxRx();

#endif /* UART_H_ */
