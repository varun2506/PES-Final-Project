/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: iic.c
 * File description: Initialize IIC for interfacing with accelerometer
 * Reference: Alexander Dean, Textbook- Chapter 8 Serial Communication
 * 			Textbook: Embedded Systems Fundamentals with ARM Cortex-M based Microcontrollers
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/
#include <MKL25Z4.H>
#include <stdint.h>
#include "i2c.h"

/*********************************************************************
 * Macro Definition
 * *******************************************************************/
#define RESET       (0)
#define PRESCALE_64 (0x10)
#define I2C_SCL_PIN (24)
#define I2C_SDA_PIN (25)
#define I2C_PORT    (5)

/**********************************************************************
 * Function Name :i2c_start_bit()
 * Description: Initialize IIC start condition
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
static void i2c_start_bit()
{
	I2C0->C1 |= I2C_C1_MST_MASK;  /* Set master bit for start condition */
}

/**********************************************************************
 * Function Name :i2c_stop_bit()
 * Description: Initialize IIC stop condition
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
static void i2c_stop_bit()
{
	I2C0->C1 &= ~(I2C_C1_MST_MASK); /* Clear master bit for Stop condition */
}

/**********************************************************************
 * Function Name :i2c_repeated_start()
 * Description: Initialize RGB led
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
static void i2c_repeated_start()
{
	I2C0->C1 |= I2C_C1_RSTA_MASK; /* Set RSTA bit for repeated start condition */
}

/**********************************************************************
 * Function Name :begin_transmit()
 * Description: Start IIC Transmission
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
static void begin_transmit()
{
	I2C0->C1 |= I2C_C1_TX_MASK;  /* Start tx bit */
}

/**********************************************************************
 * Function Name :begin_recieve()
 * Description: Start Reception
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
static void begin_recieve()
{
	I2C0->C1 &= ~(I2C_C1_TX_MASK);  /* Enable Reception */
}

/**********************************************************************
 * Function Name :i2c_send_ack_bit()
 * Description: Send IIC Acknowledgement bit
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
static void i2c_send_ack_bit()
{
	I2C0->C1 &= ~(I2C_C1_TXAK_MASK); /* IIC Acknowledgement bit */
}

/**********************************************************************
 * Function Name :i2c_send_nack_bit()
 * Description: Send IIC Negative Acknowledgement bit
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
static void i2c_send_nack_bit()
{
	I2C0->C1 |= I2C_C1_TXAK_MASK;  /* IIC Negative Acknowledgement bit */
}

/**********************************************************************
 * Function Name :tx_addr_read(uint8_t dev_addr)
 * Description: Master read address
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
static void tx_addr_read(uint8_t dev_addr)
{
	I2C0->D = (dev_addr|0x1);  /* Master read data address */
}

/**********************************************************************
 * Function Name :tx_addr_write(uint8_t dev_addr)
 * Description: Transmit write address
 * param[In]  : (uint8_t dev_addr)
 * param[out] : void
 * ********************************************************************/
static void tx_addr_write(uint8_t dev_addr)
{
	I2C0->D = (dev_addr); /* Address to be written */
}

/**********************************************************************
 * Function Name :i2c_rx_slave_data()
 * Description: Read data from slave register
 * param[In]  : void
 * param[out] : uint8_t
 * ********************************************************************/
static uint8_t i2c_rx_slave_data()
{
	return I2C0->D;  /* Read slave data */
}

/**********************************************************************
 * Function Name :init_I2C()
 * Description: Initialize IIC
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void init_I2C()
{
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;    /* Enable IIC Clock */
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK); /* Enable clock to PORT E */

	PORTE->PCR[I2C_SCL_PIN] |= PORT_PCR_MUX(I2C_PORT); /* Init IIC data pins */
	PORTE->PCR[I2C_SDA_PIN] |= PORT_PCR_MUX(I2C_PORT);

	I2C0->F = RESET;                     /* Reset frequency divider */
 	I2C0->F = (I2C_F_ICR(PRESCALE_64));  /* Standard IIC mode */

	I2C0->C1 |= (I2C_C1_IICEN_MASK | I2C_C1_MST_MASK );  /* Enable IIC master mode */

	I2C0->C2 |= I2C_C2_HDRS_MASK;  /* High drive mode */
}

/**********************************************************************
 * Function Name :i2c_ack_wait()
 * Description: Wait for acknowledgement
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void i2c_ack_wait()
{
	while(((I2C0->S & I2C_S_IICIF_MASK)==0));  /* wait for completion of slave transfer */

	I2C0->S |= I2C_S_IICIF_MASK;  /* Clear the bit */
}

/**********************************************************************
 * Function Name :i2c_start_seq()
 * Description: Start IIC sequence
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void i2c_start_seq()
{
	begin_transmit();  /* Begin trasnmission */
	i2c_start_bit();   /* Generate start condition */
}

/**********************************************************************
 * Function Name :i2c_read_addr(uint8_t dev_addr, uint8_t location)
 * Description: Initialize RGB led
 * param[In]  : uint8_t dev_addr
 * param[In]  : uint8_t location
 * param[OUT] : VOID
 * ********************************************************************/
void i2c_read_addr(uint8_t dev_addr, uint8_t location)
{

	tx_addr_write(dev_addr);  /* Transmit device address */
	i2c_ack_wait();           /* Wait for Acknowledgement */

	tx_addr_write(location);  /* Location to read data */
	i2c_ack_wait();           /* Wait for Acknowledgement */

	i2c_repeated_start();     /* Generate repeated start bit */
	tx_addr_read(dev_addr);   /*  Master reads address */
	i2c_ack_wait();           /* Wait for Acknowledgement */

	begin_recieve();  /* start reaading */
}


/**********************************************************************
 * Function Name :i2c_read_reg(uint8_t tx_ack)
 * Description: Read IIC register
 * param[In]  : uint8_t tx_ack
 * param[out] : void
 * ********************************************************************/
uint8_t i2c_read_reg(uint8_t tx_ack)
{
	uint8_t rx_byte;

	if(!tx_ack)   /* Acknowledgement when operation has ended */
	{
		i2c_send_nack_bit();  /* Send Nack */
	}
	else
	{
		i2c_send_ack_bit();  /* Send Ack */
	}
	
	rx_byte = i2c_rx_slave_data();  /* Read slave data */
	i2c_ack_wait();                 /* Wait for acknowledgement */

	if(!tx_ack)
	{
		i2c_stop_bit(); /* Write Nak for last byte */
	}

	rx_byte = i2c_rx_slave_data();  /* Read slave data */
	return  rx_byte;
}

/**********************************************************************
 * Function Name :i2c_rxByte(uint8_t dev_addr, uint8_t location)
 * Description: IIC recieve bytes
 * param[In]  : uint8_t dev_addr
 * param[In]  : uint8_t location
 * param[out] : uint8_t
 * ********************************************************************/
uint8_t i2c_rxByte(uint8_t dev_addr, uint8_t location)
{
	uint8_t rx_byte;

	i2c_start_seq();  /* Start transmission sequence */

	tx_addr_write(dev_addr);  /* Transmit device address */
	i2c_ack_wait();           /* Wait for acknowledgement */

	tx_addr_write(location);  /* Transmit device address */
	i2c_ack_wait();           /* Wait for acknowledgement */

	i2c_repeated_start();     /* IIC repeated sequence */
	tx_addr_read(dev_addr);
	i2c_ack_wait();           /* Wait for acknowledgement */


	begin_recieve();      /* Begin Recieve */
	i2c_send_nack_bit();  /* Wait for negative acknowledgement */
	
	rx_byte = i2c_rx_slave_data(); /* Recieve data from slave */
	i2c_ack_wait();                /* Wait for acknowledgement */

	i2c_stop_bit();                 /* IIC stop bit sequence */
	rx_byte = i2c_rx_slave_data();  /* Recieve data from slave */

	return rx_byte;
}

/**********************************************************************
 * Function Name :i2c_txByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t txbyte)
 * Description: IIC transfer byte
 * param[In]  : dev_addr
 * param[In]  : reg_addr
 * param[In]  : txbyte
 * param[out] : void
 * ********************************************************************/
void i2c_txByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t txbyte)
{
	i2c_start_seq();  /* IIC start sequence */
	
	tx_addr_write(dev_addr);  /* Transmit device address */
	i2c_ack_wait();           /* Wait for acknowledgement */

	tx_addr_write(reg_addr);  /* Transmit device address */
	i2c_ack_wait();           /* Wait for acknowledgement */

	tx_addr_write(txbyte);    /* Transmit device address */
	i2c_ack_wait();           /* Wait for acknowledgement */

	i2c_stop_bit();   /* IIC stop condition */
	
}
