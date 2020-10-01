/* ========================================
 *
 * Copyright HEMI, 2020
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 * 
 * Functions used by BeoModern main PSoC to monitor input informaiton coming from
 * push buttons and over DataLink interface
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF HEMI.
 *
 * ========================================
*/

#ifndef INPUTS_H_   /* Include guard */
#define INPUTS_H_

#include <project.h>

// Changes in the interrupt to signal that the transfer was complete 
uint8 flag;

/*******************************************************************************
* Function Name: CY_ISR(DISPLAY_Timer_INTHandler)
********************************************************************************
* Summary:
*  The Interrupt Service Routine. Triggered by software when timer expires after 
*  DISPLAY_Timer_Start() is called. It uses timer to count 2 seconds wait.
*  This time is used to display information that are temporary (like button press
*  or BT status).
*   
* Parameters:
*  Timer interrupt handler 
*
* Return:
*  updates DISPLAY_UPDATE variable.when TC is called.
*
********************************************************************************/
CY_ISR(DISPLAY_Timer_INTHandler);



/*******************************************************************************
* Function Name: Slave_ISR
********************************************************************************
* Summary:
*  The main function performs the following actions:
*   1. Sets the flag to 1
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Slave_ISR();

#endif // INPUTS_H_

/* [] END OF FILE */
