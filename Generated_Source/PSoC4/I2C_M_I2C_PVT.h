/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in I2C mode.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_I2C_PVT_I2C_M_H)
#define CY_SCB_I2C_PVT_I2C_M_H

#include "I2C_M_I2C.h"


/***************************************
*     Private Global Vars
***************************************/

extern volatile uint8 I2C_M_state; /* Current state of I2C FSM */

#if(I2C_M_I2C_SLAVE_CONST)
    extern volatile uint8 I2C_M_slStatus;          /* Slave Status */

    /* Receive buffer variables */
    extern volatile uint8 * I2C_M_slWrBufPtr;      /* Pointer to Receive buffer  */
    extern volatile uint32  I2C_M_slWrBufSize;     /* Slave Receive buffer size  */
    extern volatile uint32  I2C_M_slWrBufIndex;    /* Slave Receive buffer Index */

    /* Transmit buffer variables */
    extern volatile uint8 * I2C_M_slRdBufPtr;      /* Pointer to Transmit buffer  */
    extern volatile uint32  I2C_M_slRdBufSize;     /* Slave Transmit buffer size  */
    extern volatile uint32  I2C_M_slRdBufIndex;    /* Slave Transmit buffer Index */
    extern volatile uint32  I2C_M_slRdBufIndexTmp; /* Slave Transmit buffer Index Tmp */
    extern volatile uint8   I2C_M_slOverFlowCount; /* Slave Transmit Overflow counter */
#endif /* (I2C_M_I2C_SLAVE_CONST) */

#if(I2C_M_I2C_MASTER_CONST)
    extern volatile uint16 I2C_M_mstrStatus;      /* Master Status byte  */
    extern volatile uint8  I2C_M_mstrControl;     /* Master Control byte */

    /* Receive buffer variables */
    extern volatile uint8 * I2C_M_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint32  I2C_M_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint32  I2C_M_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Transmit buffer variables */
    extern volatile uint8 * I2C_M_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint32  I2C_M_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint32  I2C_M_mstrWrBufIndex; /* Master Write buffer Index      */
    extern volatile uint32  I2C_M_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */
#endif /* (I2C_M_I2C_MASTER_CONST) */

#if (I2C_M_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
    extern uint32 (*I2C_M_customAddressHandler) (void);
#endif /* (I2C_M_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */

/***************************************
*     Private Function Prototypes
***************************************/

#if(I2C_M_SCB_MODE_I2C_CONST_CFG)
    void I2C_M_I2CInit(void);
#endif /* (I2C_M_SCB_MODE_I2C_CONST_CFG) */

void I2C_M_I2CStop(void);
void I2C_M_I2CFwBlockReset(void);

void I2C_M_I2CSaveConfig(void);
void I2C_M_I2CRestoreConfig(void);

#if(I2C_M_I2C_MASTER_CONST)
    void I2C_M_I2CReStartGeneration(void);
#endif /* (I2C_M_I2C_MASTER_CONST) */

#endif /* (CY_SCB_I2C_PVT_I2C_M_H) */


/* [] END OF FILE */
