/* ========================================
 *
 * Copyright HEMI, 2021
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 * 
 * This project is intended to be used as code that runs on PSoC KIT
 * inside BeoModern system in display module. 
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
uint8 SPI_flag;
    
// Changes in the interrupt to signal that the transfer was complete 
uint8 mS200_flag;

// flag used when clock is enabled to update display more frequent than SPI
uint8 clock_flag;    

// counter used by clock adjustments function. When its value is higher than zero,
// periodic timer will decrease it by 1 every 200mS    
uint8 clock_message_timer;
        
// Variable to store DCF strength signal 
uint8 DCF_strength;    

// global variable to store DCF timer count value    
uint32 DCF_timer_count;     

// table to store last received full DCF message (60 bits)
uint8 DCF_data[60];
    
/*******************************************************************************
* Function Name: CY_ISR(Periodic_Timer_INTHandler)
********************************************************************************
* Summary:
*  The Interrupt Service Routine. Triggered by software when timer expires after 
*  Periodic_Timer_Start() is called. It uses timer to count 200 mseconds wait.
*  This time is used by the system to perform periodic checks (like brightness, 
*  off timer expired, message timer).
*   
* Parameters:
*  Timer interrupt handler 
*
* Return:
*  updates Disp_Brightness variable when TC is called.
*
********************************************************************************/
CY_ISR(Periodic_Timer_INTHandler);



/*******************************************************************************
* Function Name: Slave_ISR
********************************************************************************
* Summary:
*  Function is called by SPI block when new SPI message is received. It sets flag 
*  that is served later on by main loop. 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Slave_ISR();



/*******************************************************************************
* Function Name: SysTickIsrHandler
********************************************************************************
* Summary:
*  The interrupt handler for SysTick interrupts. Interrupt used by internal RTC
*  function to produce time and date info. 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SysTickIsrHandler(void);



/*******************************************************************************
* Function Name: DCF_PIN_INTR_ISR
********************************************************************************
* Summary:
*  The interrupt handler for DCF pin state change. It is used to check and capture 
*  DCF timer state
*  NEEDS TO BE DEBUGGED WHEN DCF SIGNAL IS AVAILABLE
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DCF_PIN_INTR_ISR(void);



/*******************************************************************************
* Function Name: DCF_TIMER_INTR_ISR
********************************************************************************
* Summary:
*  The interrupt handler for DCF timer overload indicaton to the overall system.
*  NEEDS TO BE DEBUGGED WHEN DCF SIGNAL IS AVAILABLE
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DCF_TIMER_INTR_ISR(void);


#endif // INPUTS_H_

/* [] END OF FILE */
