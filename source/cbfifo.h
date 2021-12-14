/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: cbfifo.c
 * File description: circular buffer header file
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/
#ifndef CBFIFO_H_
#define CBFIFO_H_

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/*********************************************************************
 * Macro Definition
 * *******************************************************************/
#define SIZE_OF_BUFFER 256
#define RESET 0
// critical section macro functions
#define START_CRITICAL()	__disable_irq()
#define END_CRITICAL(x)	__set_PRIMASK(x)
#define min(x,y) ((x)<(y)?(x):(y))

/*********************************************************************
 * Structure declaration
 * *******************************************************************/
typedef struct {
	int rear;
	int front;
	size_t length;
	bool IS_Buffer_full;
	uint8_t cb_buf[SIZE_OF_BUFFER];
} cbfifo_t;

/**********************************************************************
 * Function Name :cbfifo_init(cbfifo_t * q);
 * Description: Initialize circular buffer
 * param[In]  : cbfifo_t * q
 * param[out] : void
 * ********************************************************************/
extern void cbfifo_init(cbfifo_t * q);

/**********************************************************************
 * Function Name :cbfifo_enqueue(cbfifo_t * q, const void *buf , size_t nbyte);
 * Description: Enqueue element into buffer
 * param[In]  : cbfifo_t * q
 * param[In]  : buffer
 * param[out] : size
 * ********************************************************************/
extern size_t cbfifo_enqueue(cbfifo_t * q, const void *buf , size_t nbyte);

/**********************************************************************
 * Function Name :cbfifo_dequeue(cbfifo_t * q, void *buf , size_t nbyte);
 * Description: Enqueue element into buffer
 * param[In]  : cbfifo_t * q
 * param[In]  : buffer
 * param[out] : size
 * ********************************************************************/
extern size_t cbfifo_dequeue(cbfifo_t * q, void *buf , size_t nbyte);

/**********************************************************************
 * Function Name :cbfifo_length(cbfifo_t * q);
 * Description: current length of buffer
 * param[In]  : cbfifo_t * q
 * param[out] : size
 * ********************************************************************/
extern size_t cbfifo_length(cbfifo_t * q);

/**********************************************************************
 * Function Name :cbfifo_capacity(cbfifo_t * q);
 * Description: capacity of remaining buffer
 * param[In]  : cbfifo_t * q
 * param[out] : size
 * ********************************************************************/
extern size_t cbfifo_capacity(cbfifo_t * q);

/**********************************************************************
 * Function Name :cbfifo_empty(cbfifo_t * q);
 * Description: Check if buffer is empty
 * param[In]  : cbfifo_t * q
 * param[out] : bool
 * ********************************************************************/
extern bool cbfifo_empty(cbfifo_t * q);

/**********************************************************************
 * Function Name :cbfifo_full(cbfifo_t * q);
 * Description: To check if buffer is full
 * param[In]  : cbfifo_t * q
 * param[out] : bool
 * ********************************************************************/
extern bool cbfifo_full(cbfifo_t * q);

/**********************************************************************
 * Function Name :cbfifo_size(cbfifo_t * q);
 * Description: Total size of buffer
 * param[In]  : cbfifo_t * q
 * param[out] : void
 * ********************************************************************/
extern int cbfifo_size(cbfifo_t * q);

#endif /* CBFIFO_H_ */
