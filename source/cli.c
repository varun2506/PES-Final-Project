/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: cli.c
 * File description: command line interface source file
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "fsl_debug_console.h"
#include "sysclock.h"
#include "UART.h"
#include "cli.h"
#include "LEDs.h"
#include "i2c.h"
#include "extra_switch.h"
#include "touch.h"
#include "mma8451.h"
#include "MKL25Z4.h"

typedef void (*cmdline)(const char* cmd); /* Operating Using Function Pointers */

/*********************************************************************
 * Structure declaration
 * *******************************************************************/
typedef struct {
	const char* head;
	cmdline func_name;
} cmd_lookup_t;


/**********************************************************************
 * Function Name :read_accelerometer(const char* cmd)
 * Description: Measure and display the current angle on the accelerometer.
 *              Hold the board in any position and calibrate the angle to 0 degree.
 * param[In]  : const char* cmd
 * param[out] : bool
 * ********************************************************************/
static void read_accelerometer(const char* cmd)
{
	switch_pressed=false; /* Reset switch status */

	int degree=0;    /* Initialize angle to 0 degree */
	int set_value=0; /* Initialize calibration to 0 degree */

	while(!switch_pressed)      /* Check switch status and execute the loop */
	{

		int roll=compute_angle();     /* Compute Angle */
		TSI0->DATA |= TSI_DATA_SWTS_MASK;
		if(touch_val>100)             /* Check if touch is detected for calbration */
		{
			touch_val=0;
			set_value=fabs(roll);     /* Get the current degree for calibration */
		}

		degree=fabs(roll)-set_value;  /* Calculate output degree*/
		printf("Accelerometer Angle: %3d\r",degree);
	}
	printf("\n\r");
}

/**********************************************************************
 * Function Name :angle(const char* cmd)
 * Description: Measure and initialize the LED when the angle in the
 *              argument is detected.
 * param[In]  : const char* cmd
 * param[out] : bool
 * ********************************************************************/
static void angle(const char* cmd)
{
	switch_pressed=false; /* Reset switch status */

	int degree=0;    /* Initialize angle to 0 degree */
	int set_value=0; /* Initialize calibration to 0 degree */
	uint16_t user_angle=0;  /* Intially the angle s set to 0 degree if not set by user */
	char* ptr;
	user_angle = strtol(&cmd[6],&ptr,10);

	printf("User angle %d\n\r",user_angle);

	if(user_angle)  {
		if(user_angle>180)
		{
			printf("Invalid angle input\n\r");
			return;
		}
		printf("Blue LED glows at %d degrees\n\r",user_angle);
		while(!switch_pressed)     /* Check switch status and execute the loop */
		{
			int roll=compute_angle();   /* Compute Angle */
			TSI0->DATA |= TSI_DATA_SWTS_MASK;
			if(touch_val>100)          /* Check if touch is detected for calbration */
			{
				touch_val=0;
				set_value=fabs(roll);
			}
			degree=fabs(roll)-set_value;
			Control_RGB_LEDs(0,0,0);
			if(degree==user_angle)       /* When measured angle is equal angle in argument */
				Control_RGB_LEDs(0,0,1);
		}
		Control_RGB_LEDs(0,0,0);
	}
	else
	{
		printf("Invalid angle input\n\r");
	}
}

/**********************************************************************
 * Function Name :lapse_30(const char* cmd)
 * Description: Change the color of the led for every 30 degree angle change
 *              (30, 60, 90)
 * param[In]  : const char* cmd
 * param[out] : bool
 * ********************************************************************/
static void lapse_30(const char* cmd){
	switch_pressed=false; /* Reset switch status */

	int degree=0;    /* Initialize angle to 0 degree */
	int set_value=0; /* Initialize calibration to 0 degree */
	printf("Change LED color at 30 60 90 \n\r");
	while(!switch_pressed)       /* Check switch status and execute the loop */
	{
		int roll=compute_angle();     /* Compute Angle */
		TSI0->DATA |= TSI_DATA_SWTS_MASK;
		if(touch_val>100)            /* Check if touch is detected for calbration */
		{
			touch_val=0;
			set_value=fabs(roll);
		}
		degree=fabs(roll)-set_value;
		if(degree==30)                 /* Change led color when angle is measured to 30 degree */
			Control_RGB_LEDs(0,1,1);
		else if(degree==60)            /* Change led color when angle is measured to 60 degree */
			Control_RGB_LEDs(1,0,1);
		else if(degree==90)            /* Change led color when angle is measured to 90 degree */
			Control_RGB_LEDs(1,1,0);
		else
			Control_RGB_LEDs(0,0,0);
	}
	Control_RGB_LEDs(0,0,0);
}

/**********************************************************************
 * Function Name :surface(const char* cmd)
 * Description: Blink LED when angle is measured with respect to surface
 *              (0, 180)
 * param[In]  : const char* cmd
 * param[out] : bool
 * ********************************************************************/
static void surface(const char* cmd)
{
	printf("Glow LED when surface occurs\n\r");
	switch_pressed=false;   /* Reset switch status */

	int degree=0;               /* Initialize angle to 0 degree */
	int set_value=0;            /* Initialize calibration to 0 degree */
	while(!switch_pressed)      /* Check switch status and execute the loop */
	{
		int roll=compute_angle();         /* Compute Angle */
		TSI0->DATA |= TSI_DATA_SWTS_MASK;
		if(touch_val>100)                 /* Check if touch is detected for calbration */
		{
			touch_val=0;
			set_value=fabs(roll);
		}
		degree=fabs(roll)-set_value;
		if(degree==0 || degree==90)   /* Blink LED when angle is measured with respect to surface (0, 180) */
			Control_RGB_LEDs(0,1,0);
		else
			Control_RGB_LEDs(0,0,0);
	}
	Control_RGB_LEDs(0,0,0);
}

/**********************************************************************
 * Function Name :help( )
 * Description: Helper function for user
 * param[In]  : cmd
 * param[out] : void
 * ********************************************************************/
static void help(const char* cmd)
{
	printf("read_acc          :Measure accelerometer angle\r\n");
	printf("angle <ANGLE>     :Glow LED when requested angle is measured \r\n");
	printf("lapse_30          :Change the color of the led for every 30 degree angle change(30, 60, 90)\r\n");
	printf("surface           :Blink LED when angle is measured with respect to surface(0, 180).\r\n");
}


/**********************************************************************
 * Function Name :invalid(const char* cmd)
 * Description: Unknon/invalid commands
 * param[In]  : cmd string
 * param[out] : void
 * ********************************************************************/
static void invalid(const char* cmd){
	printf("Unknown Command: %s\r\n", cmd);
}

static const cmd_lookup_t cmds[] = {
		{"read_acc", read_accelerometer},
		{"angle", angle},
		{"lapse_30",lapse_30},
		{"surface",surface},
		{"help",help},
};

/**********************************************************************
 * Function Name :cmd_execute(char *input_str);
 * Description: Execute command line interface
 * param[In]  : *input_str
 * param[out] : void
 * ********************************************************************/
void cmd_execute(char *input_str) {

	//char temp_str[CLI_SIZE] = {0};
	char temp_str[40] = {0};

	char* temp1 = &temp_str[0];
	char* temp2 = &input_str[0];
	int status = true;
	char head[20];

	while(*temp2 != '\0')           /* parse the string */
	{
		if(isspace(*temp2))         /* Eliminate the white spaces */
		{
			if(status)
			{
				temp2++;
			} else
			{
				*temp1++ = ' ';
				status = 1;
			}
		}
		else
		{
			*temp1++ = tolower(*temp2++);   /* handled lower case */
			status = 0;
		}
	}
	*temp2 = '\0';

	sscanf(temp_str, "%s", head);

	int num = sizeof(cmds) / sizeof(cmd_lookup_t); /* call approrpriate funtion as per command intruction */
	for(int i=0; i<num; i++)
		//for(int i=0; i<num; i++)
	{
		if(strcmp(head, cmds[i].head) == 0)
		{
			cmds[i].func_name(temp_str);
			temp1 = &temp_str[0];
			temp2 = &input_str[0];
			return;
		}
	}

	invalid(temp_str);   /* Command does'nt match */
	temp1 = &temp_str[0];
	temp2 = &input_str[0];
	return;

}
