/*******************************************************************************
* File Name: Display_DATA_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Display_DATA.h"

/* Check for removal by optimization */
#if !defined(Display_DATA_Sync_ctrl_reg__REMOVED)

static Display_DATA_BACKUP_STRUCT  Display_DATA_backup = {0u};

    
/*******************************************************************************
* Function Name: Display_DATA_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Display_DATA_SaveConfig(void) 
{
    Display_DATA_backup.controlState = Display_DATA_Control;
}


/*******************************************************************************
* Function Name: Display_DATA_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void Display_DATA_RestoreConfig(void) 
{
     Display_DATA_Control = Display_DATA_backup.controlState;
}


/*******************************************************************************
* Function Name: Display_DATA_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Display_DATA_Sleep(void) 
{
    Display_DATA_SaveConfig();
}


/*******************************************************************************
* Function Name: Display_DATA_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Display_DATA_Wakeup(void)  
{
    Display_DATA_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
