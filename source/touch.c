/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: touch.C
 * File description: Configure touch sensor for calibration
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/
#include "touch.h"
#include <MKL25Z4.h>

/*********************************************************************
 * Global Declaration
 * *******************************************************************/
volatile uint32_t touch_val=0;

/**********************************************************************
 * Function Name :init_TSI(void)
 * Description: Initialize touch sensor for calibration
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void init_TSI(void)
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;   /* enabling the clock */

	TSI0->GENCS = TSI_GENCS_MODE   (0u) |  /* operating in non-noise mode */
			      TSI_GENCS_REFCHRG(0u) |  /* reference oscillator charge and discharge value 500nA */
			      TSI_GENCS_DVOLT  (0u) |  /* oscillator voltage rails set to default */
			      TSI_GENCS_EXTCHRG(0u) |  /* electrode oscillator charge and discharge value 500nA */
			      TSI_GENCS_PS     (0u) |  /* frequency clock divided by one */
			      TSI_GENCS_NSCN   (31u)|  /* scanning the electrode 32 times */
			      TSI_GENCS_TSIEN_MASK  |  /* enabling the TSI module */
			      TSI_GENCS_EOSF_MASK   |  /* writing one to clear the end of scan flag */
			      TSI_GENCS_ESOR_MASK   |  /* enabling interrupt using end of scan */
			      TSI_GENCS_TSIIEN_MASK;   /* enabling the TSI interrupt */


	TSI0->DATA = 	TSI_DATA_TSICH(10u); /* selecting channel 10 */

	NVIC_SetPriority(TSI0_IRQn, 2);      /* Set Interrupt priority */
	NVIC_ClearPendingIRQ(TSI0_IRQn);     /* Clear pending Interrupt */
	NVIC_EnableIRQ(TSI0_IRQn);           /* Enable Interrupt */

	SMC->PMPROT = SMC_PMPROT_ALLS_MASK; 	/* Allow low leakage stop mode */

	SMC->PMCTRL = SMC_PMCTRL_STOPM(3) | SMC_PMCTRL_RUNM(0);  /* Enable low-leakage mode */

	SMC->STOPCTRL = SMC_STOPCTRL_PSTOPO(0) | SMC_STOPCTRL_VLLSM(3);
}

/**********************************************************************
 * Function Name :TSI0_IRQHandler (void)
 * Description: Touch detect interrupt handler
 * param[In]  : void
 * param[out] : void
 * ********************************************************************/
void TSI0_IRQHandler (void){
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK ; 	/* Clear interrupt flag */
	touch_val = TOUCH_DATA - TOUCH_OFFSET;

}
