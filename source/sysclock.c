/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: sysclock.h
 * File description: Configure sysclock source file
 * Reference: KL25Z reference manual
 * *******************************************************************/
#include "MKL25Z4.h"
#include "sysclock.h"

/*********************************************************************
 * Macro Definition
 * *******************************************************************/
#define SYSCLOCK_FREQUENCY (24000000U)

/**********************************************************************
 * Function Name :sysclock_init()
 * Description: Initialize system clock
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void sysclock_init()
{
  // Corresponds to FEI mode as shown in sec 24.4.1 

  MCG->C1 &= ~(MCG_C1_CLKS_MASK); /* Select PLL clock source */
  MCG->C1 |= MCG_C1_CLKS(0);
	
  MCG->C1 |= MCG_C1_IREFS(1); /*Internal reference clock is used */

  MCG->C6 &= ~(MCG_C6_PLLS_MASK);  /* Select FLL */
  MCG->C6 |= MCG_C6_PLLS(0);

  MCG->C4 &= ~(MCG_C4_DRST_DRS_MASK & MCG_C4_DMX32_MASK);  /* Select 24 MHz */
  MCG->C4 |= MCG_C4_DRST_DRS(0);
  MCG->C4 |= MCG_C4_DMX32(1);
}

