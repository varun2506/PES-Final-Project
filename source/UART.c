/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: UART.C
 * File description: UART source file
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/
#include "UART.h"
#include "sysclock.h"
#include "cbfifo.h"
#include "cli.h"

/*********************************************************************
 * Global Declaration
 * *******************************************************************/
cbfifo_t TxCB, RxCB;

/**********************************************************************
 * Function Name :__sys_write(int handle, char* buffer, int count)
 * Description: Write data
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
int __sys_write(int handle, char* buffer, int count) {
	if(buffer == NULL)
	{
		return -1;
	}
	while(cbfifo_full(&TxCB)); /* Wait for the space to openup */

	if(cbfifo_enqueue(&TxCB, buffer, count) != count)
	{
		return -1;
	}

	if(!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}
	while(cbfifo_length(&TxCB)!=0);  /* Wait for length to become 0 */
	return 0;
}

/**********************************************************************
 * Function Name :__sys_readc(void)
 * Description: Read data
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
int __sys_readc(void) {
	char ch;
	if (cbfifo_dequeue(&RxCB, &ch, 1) != 1)  /* Dequeue element */
	{
		return -1;
	}
	return ch;
}

/**********************************************************************
 * Function Name :Uart_Init(uint32_t baud_rate);
 * Description: Initialize UART for baud rate, data size, parity and Stop bit
 * param[In]  : baud_rate
 * param[out] : void
 * ********************************************************************/
void Uart_Init(uint32_t baud_rate) {
	uint16_t sbr;

	/* Enable clock gating for UART0 and Port A */
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	/* Make sure transmitter and receiver are disabled before init */
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	/* Set UART clock to 24 MHz clock */
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	/* Set pins to UART0 Rx and Tx */
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); /* Rx */
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); /* Tx */

	/* Set baud rate and oversampling ratio */
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * (UART_OVERSAMPLE_RATE+1)));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE);

	/* Disable interrupts for RX active edge and LIN break detect, select one stop bit */
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(UART_STOP_BIT) | UART0_BDH_LBKDIE(0);

	/* Don't enable loopback mode, use 8 data bit mode, don't use parity */
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(UART_DATA_BIT) | UART0_C1_PE(UART_PARITY_BIT) | UART0_C1_PT(0);

	/* Don't invert transmit data, don't enable interrupts for errors */
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	/* Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1); */

	/* Send LSB first, do not invert received data */
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	/* Enable receive interrupts but not transmit interrupts yet */
	UART0->C2 |= UART_C2_RIE(1);

	/* Enable UART receiver and transmitter */
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	/* Enable interrupts. Listing 8.11 on p. 234 */
	cbfifo_init(&TxCB);
	cbfifo_init(&RxCB);

	NVIC_SetPriority(UART0_IRQn, 2);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

}

/**********************************************************************
 * Function Name :Start_TxRx();
 * Description: Uart to command line interface
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void Start_TxRx() {
	char Buffer[640];
	char* Xbuf = &Buffer[0];
	uint8_t ch;
	char Wel_str[] = "Welcome to FreeRTOS based Accelerometer!!\r\n";
	cbfifo_enqueue(&TxCB, Wel_str, sizeof(Wel_str));
	while(true) {
		printf("? ");
		while(ch != 0x0D)
		{
			while (cbfifo_size(&RxCB) == 0);      /* Wait for buffer empty */

			cbfifo_dequeue(&RxCB, &ch, 1);        /* Dequeue element */
			putchar(ch);
			if (ch != 0x0D || ch != 0x0A)
			{
				if(ch != 0x08)                    /* Backspace character */
				{
					*Xbuf = (char)ch;
					Xbuf++;
				}
				else
				{
					printf(" \b");                /* If backspace character detected */
					Xbuf--;
				}
			}

			if (!(UART0->C2 & UART0_C2_TIE_MASK)) /* start transmitter if it isn't already running */
			{
				UART0->C2 |= UART0_C2_TIE(1);
			}
			if(ch == '\r')                       /* new line */
			{
				ch = 0x0A;
				printf("\r\n");
				break;
			}

		}
		*Xbuf = '\0';

		cmd_execute(Buffer);             /* call command line argument */
		Xbuf = &Buffer[0];

	}
}

/**********************************************************************
 * Function Name :UART0_IRQHandler(void)
 * Description: Uart interrupt handler
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void UART0_IRQHandler(void) {

	uint8_t charc;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
		UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);     /* Clear interrupt */
		charc = UART0->D;
	}

	if (UART0->S1 & UART0_S1_RDRF_MASK)                 /* Received a character */
	{
		charc = UART0->D;
		cbfifo_enqueue(&RxCB, &charc, 1);               /* Enqueue element in buffer */
	}

	if ( (UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK) )    /* Enable Tx Interrupt */
	{
		if(cbfifo_dequeue(&TxCB, &charc, 1))      /* Dequeue Element from buffer  */
		{
			UART0->D = charc;
		}
		else
		{
			UART0->C2 &= ~UART0_C2_TIE_MASK;     /* Disable Interrupt */
		}
	}
}
