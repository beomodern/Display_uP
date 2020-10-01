/* ========================================
 *
 * Copyright HEMI, 2020
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 * 
 * Functions used by BeoModern main PSoC to display informaiton on BeoModern
 * front display as well as front LEDs
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF HEMI.
 *
 * ========================================
*/

#ifndef DISPLAY_H_   /* Include guard */
#define DISPLAY_H_

#include <project.h>
#include <operate.h>    
//#include <main.h>
    
/*******************************************************************************
* Function Name: SPI_message_reader
********************************************************************************
* Summary:
*  Funciton reponsible for decoding and displayin commands that were sent over 
*  SPI interface to display. It intercepts 24 ASCI characters and pushes them to 
*  display. It also interprets two control words and manipulates LEDs acordingly
*  to control words state.
*  LED control
*  CTRL0.0 - Player
*  CTRL0.1 - iRadio
*  CTRL0.2 - DAB Radio
*  CTRL0.3 - Aux IN
*  CTRL0.5 - RDS/FM Display    
*  CTRL0.6 - BT Tx
*  CTRL0.7 - BT Rx    
*  CTRL1.0 - Online status
*  CTRL1.1 - Clock
*  CTRL1.2 - Clock Atomic
*  CTRL1.3 - NOT USED
*  CTRL1.4 - NOT USED
*  CTRL1.5 - NOT USED
*  CTRL1.6 - NOT USED
*  CTRL1.7 - NOT USED
*
* Parameters:
*  input_table[] - table returned by SPI slave routine
*
* Return:
*  Returns display status
*  0 - power down
*  1 - operational
*  2 - ...
*
*******************************************************************************/
uint8 SPI_message_reader(uint8 input_table[]);
    
    
#endif // DISPLAY_H_

/* [] END OF FILE */
