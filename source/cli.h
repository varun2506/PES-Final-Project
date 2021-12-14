/*********************************************************************
 * Author: Varun Mehta
 * PES Final Project Fall 21
 * File name: cli.c
 * File description: command line interface header file
 * *******************************************************************/
/*********************************************************************
 * Header File
 * *******************************************************************/
#ifndef CLI_H_
#define CLI_H_

/*********************************************************************
 * Macro Definition
 * *******************************************************************/
#define CLI_SIZE 2048

/**********************************************************************
 * Function Name :cmd_execute(char *input_str);
 * Description: Execute command line interface
 * param[In]  : *input_str
 * param[out] : void
 * ********************************************************************/
void cmd_execute(char *input_str);

#endif /* CLI_H_ */
