/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: cbfifo.c
 * File description: cbfifo source file
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/

#include "MKL25Z4.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <cbfifo.h>

/**********************************************************************
 * Function Name :cbfifo_init(cbfifo_t * q);
 * Description: Initialize circular buffer
 * param[In]  : cbfifo_t * q
 * param[out] : void
 * ********************************************************************/
void cbfifo_init(cbfifo_t * q) {
	q->rear = RESET;
	q->front = RESET;
	q->IS_Buffer_full = false;
	q->length = RESET;
	memset(q->cb_buf, RESET , 256);
}

/**********************************************************************
 * Function Name :cbfifo_capacity(cbfifo_t * q);
 * Description: capacity of remaining buffer
 * param[In]  : cbfifo_t * q
 * param[out] : size
 * ********************************************************************/
size_t cbfifo_capacity(cbfifo_t * q) {
	return SIZE_OF_BUFFER;
}

/**********************************************************************
 * Function Name :cbfifo_empty(cbfifo_t * q);
 * Description: Check if buffer is empty
 * param[In]  : cbfifo_t * q
 * param[out] : bool
 * ********************************************************************/
bool cbfifo_empty(cbfifo_t * q) {
	assert(q);
	return (q->rear == q->front);
}

/**********************************************************************
 * Function Name :cbfifo_size(cbfifo_t * q);
 * Description: Total size of buffer
 * param[In]  : cbfifo_t * q
 * param[out] : void
 * ********************************************************************/
int cbfifo_size(cbfifo_t * q) {
	assert(q);
	return q->length;
}

/**********************************************************************
 * Function Name :cbfifo_full(cbfifo_t * q);
 * Description: To check if buffer is full
 * param[In]  : cbfifo_t * q
 * param[out] : bool
 * ********************************************************************/
bool cbfifo_full(cbfifo_t * q){
	assert(q);
	return (cbfifo_length(q) == SIZE_OF_BUFFER);
}

/**********************************************************************
 * Function Name :cbfifo_length(cbfifo_t * q);
 * Description: current length of buffer
 * param[In]  : cbfifo_t * q
 * param[out] : size
 * ********************************************************************/
size_t cbfifo_length(cbfifo_t * q){
	if(q->IS_Buffer_full){
		return SIZE_OF_BUFFER;
	}
	else if(q->rear >= q->front){
		return (q->rear - q->front);
	}
	else {
		return (SIZE_OF_BUFFER - (q->front - q->rear));
	}
}


/**********************************************************************
 * Function Name :cbfifo_enqueue(cbfifo_t * q, const void *buf , size_t nbyte);
 * Description: Enqueue element into buffer
 * param[In]  : cbfifo_t * q
 * param[In]  : buffer
 * param[out] : size
 * ********************************************************************/
size_t cbfifo_enqueue(cbfifo_t * q, const void *buf , size_t nbyte) {
	size_t len1 = 0;
	size_t len2=0;
	uint32_t masking_state;

	if(q->IS_Buffer_full) {
		return 0;
	}

	masking_state = __get_PRIMASK();
	START_CRITICAL();

	if(q->rear==q->front) {             /* If buffer is empty */
		q->front = 0;
		q->rear = nbyte;
		if(nbyte == SIZE_OF_BUFFER) {
			q->IS_Buffer_full = true;
			q->rear = 0;
		}
		memcpy(q->cb_buf, buf, nbyte);
		q->length += nbyte;
		END_CRITICAL(masking_state);
		return nbyte;
	}

	if(q->front < q->rear){                      /* if buffer has data */
		len1 = min(nbyte, SIZE_OF_BUFFER - q->rear);
		memcpy(&q->cb_buf[q->rear], buf, len1);
		q->rear += len1;

		if (q->rear < SIZE_OF_BUFFER) {
			END_CRITICAL(masking_state);
			return len1 + len2;
		}

		q->rear = 0;
		if(q->front == 0) {            /* check if buffer is full */
			q->IS_Buffer_full = true;
			q->length += (len1 + len2);
			END_CRITICAL(masking_state);
			return len1 + len2;
		}

		nbyte -= len1;
		buf += len1;
	}

	len2 = min(nbyte, q->front - q->rear);
	memcpy(q->cb_buf + q->rear, buf, len2);
	q->rear += len2;

	if(q->rear == q->front) {     /* check if buffer is full */
		q->IS_Buffer_full = true;
	}
	q->length+= (len1 + len2);   /* update the length */

	END_CRITICAL(masking_state);
	return len1 + len2;

}

/**********************************************************************
 * Function Name :cbfifo_dequeue(cbfifo_t * q, void *buf , size_t nbyte);
 * Description: Enqueue element into buffer
 * param[In]  : cbfifo_t * q
 * param[In]  : buffer
 * param[out] : size
 * ********************************************************************/
size_t cbfifo_dequeue(cbfifo_t * q, void *buf , size_t nbyte) {

	size_t len1 = 0, len2=0;
	uint32_t masking_state;

	masking_state = __get_PRIMASK();

	START_CRITICAL();

	if(q->rear==q->front && !q->IS_Buffer_full) {  /* check if buffer is empty */
		q->length-= (len1 + len2);
		END_CRITICAL(masking_state);
		return len1 + len2;
	}

	q->IS_Buffer_full = false;

	len1 = min(nbyte, SIZE_OF_BUFFER - q->front);
	if((q->rear > q->front) && (len1 > q->rear - q->front)) {  /* update the length */
		len1 = q->rear - q->front;
	}
	memcpy(buf, q->cb_buf + q->front, len1);
	q->front += len1;
	if(q->front < SIZE_OF_BUFFER) {
		q->length-= (len1 + len2);
		END_CRITICAL(masking_state);
		return len1 + len2;
	}


	len2 = min(nbyte - len1, q->rear);
	memcpy(buf+len1, q->cb_buf, len2);
	q->front = len2;

	END_CRITICAL(masking_state);
	return len1 + len2;

}




