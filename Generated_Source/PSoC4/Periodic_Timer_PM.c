/*******************************************************************************
* File Name: Periodic_Timer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Periodic_Timer.h"

static Periodic_Timer_backupStruct Periodic_Timer_backup;


/*******************************************************************************
* Function Name: Periodic_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Periodic_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Periodic_Timer_SaveConfig(void) 
{
    #if (!Periodic_Timer_UsingFixedFunction)
        Periodic_Timer_backup.TimerUdb = Periodic_Timer_ReadCounter();
        Periodic_Timer_backup.InterruptMaskValue = Periodic_Timer_STATUS_MASK;
        #if (Periodic_Timer_UsingHWCaptureCounter)
            Periodic_Timer_backup.TimerCaptureCounter = Periodic_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Periodic_Timer_UDB_CONTROL_REG_REMOVED)
            Periodic_Timer_backup.TimerControlRegister = Periodic_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Periodic_Timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Periodic_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Periodic_Timer_RestoreConfig(void) 
{   
    #if (!Periodic_Timer_UsingFixedFunction)

        Periodic_Timer_WriteCounter(Periodic_Timer_backup.TimerUdb);
        Periodic_Timer_STATUS_MASK =Periodic_Timer_backup.InterruptMaskValue;
        #if (Periodic_Timer_UsingHWCaptureCounter)
            Periodic_Timer_SetCaptureCount(Periodic_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Periodic_Timer_UDB_CONTROL_REG_REMOVED)
            Periodic_Timer_WriteControlRegister(Periodic_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Periodic_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Periodic_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Periodic_Timer_Sleep(void) 
{
    #if(!Periodic_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Periodic_Timer_CTRL_ENABLE == (Periodic_Timer_CONTROL & Periodic_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Periodic_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Periodic_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Periodic_Timer_Stop();
    Periodic_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Periodic_Timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Periodic_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Periodic_Timer_Wakeup(void) 
{
    Periodic_Timer_RestoreConfig();
    #if(!Periodic_Timer_UDB_CONTROL_REG_REMOVED)
        if(Periodic_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Periodic_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */