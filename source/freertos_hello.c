/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: Freertos_hello.h
 * File description: external switch source file to terminal angle
 *                   measurement operation.
 * *******************************************************************/
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "sysclock.h"
#include "UART.h"
#include "cli.h"
#include "LEDs.h"
#include "i2c.h"
#include "extra_switch.h"
#include "touch.h"
#include "mma8451.h"
#include "MKL25Z4.h"
#include "test_cbfifo.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
#define touch_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void hello_task(void *pvParameters);
//static void touch_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 250, NULL, /*hello_task_PRIORITY*/3, NULL);
	//  xTaskCreate(touch_task, "Touch_task", configMINIMAL_STACK_SIZE + 10, NULL, /*touch_task_PRIORITY*/3, NULL);
	vTaskStartScheduler();
	for (;;)
		;
}


/**********************************************************************
 * Function Name :hello_task(void *pvParameters)
 * Description: Command Proessing thread
 * param[In]  : pvParameters
 * param[out] : void
 * ********************************************************************/
static void hello_task(void *pvParameters)
{

	sysclock_init(); /* Init sysclock */
	Uart_Init(UART_BAUDRATE);    /* Init UART */
	init_I2C();                  /* IIC Init */
	Init_RGB_LEDs();             /* Init RGB led */
	if (!init_MMA()) {           /* Init MMA */
		Control_RGB_LEDs(1, 0, 0);
		while (1)
			;
	}

	init_TSI();        /* Init TSI */
	init_switch();     /* Init ext switch for operation termination */
	Control_RGB_LEDs(0, 0, 0); /* Control LED */
	test_cbfifo();     /* test cbfifo */
	Start_TxRx();      /* Start command processing */
}