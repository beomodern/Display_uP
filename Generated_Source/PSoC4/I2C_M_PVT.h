/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
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

#if !defined(CY_SCB_PVT_I2C_M_H)
#define CY_SCB_PVT_I2C_M_H

#include "I2C_M.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define I2C_M_SetI2CExtClkInterruptMode(interruptMask) I2C_M_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define I2C_M_ClearI2CExtClkInterruptSource(interruptMask) I2C_M_CLEAR_INTR_I2C_EC(interruptMask)
#define I2C_M_GetI2CExtClkInterruptSource()                (I2C_M_INTR_I2C_EC_REG)
#define I2C_M_GetI2CExtClkInterruptMode()                  (I2C_M_INTR_I2C_EC_MASK_REG)
#define I2C_M_GetI2CExtClkInterruptSourceMasked()          (I2C_M_INTR_I2C_EC_MASKED_REG)

#if (!I2C_M_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define I2C_M_SetSpiExtClkInterruptMode(interruptMask) \
                                                                I2C_M_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define I2C_M_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                I2C_M_CLEAR_INTR_SPI_EC(interruptMask)
    #define I2C_M_GetExtSpiClkInterruptSource()                 (I2C_M_INTR_SPI_EC_REG)
    #define I2C_M_GetExtSpiClkInterruptMode()                   (I2C_M_INTR_SPI_EC_MASK_REG)
    #define I2C_M_GetExtSpiClkInterruptSourceMasked()           (I2C_M_INTR_SPI_EC_MASKED_REG)
#endif /* (!I2C_M_CY_SCBIP_V1) */

#if(I2C_M_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void I2C_M_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (I2C_M_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (I2C_M_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_I2C_M_CUSTOM_INTR_HANDLER)
    extern cyisraddress I2C_M_customIntrHandler;
#endif /* !defined (CY_REMOVE_I2C_M_CUSTOM_INTR_HANDLER) */
#endif /* (I2C_M_SCB_IRQ_INTERNAL) */

extern I2C_M_BACKUP_STRUCT I2C_M_backup;

#if(I2C_M_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 I2C_M_scbMode;
    extern uint8 I2C_M_scbEnableWake;
    extern uint8 I2C_M_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 I2C_M_mode;
    extern uint8 I2C_M_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * I2C_M_rxBuffer;
    extern uint8   I2C_M_rxDataBits;
    extern uint32  I2C_M_rxBufferSize;

    extern volatile uint8 * I2C_M_txBuffer;
    extern uint8   I2C_M_txDataBits;
    extern uint32  I2C_M_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 I2C_M_numberOfAddr;
    extern uint8 I2C_M_subAddrSize;
#endif /* (I2C_M_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (I2C_M_SCB_MODE_I2C_CONST_CFG || \
        I2C_M_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 I2C_M_IntrTxMask;
#endif /* (! (I2C_M_SCB_MODE_I2C_CONST_CFG || \
              I2C_M_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(I2C_M_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define I2C_M_SCB_MODE_I2C_RUNTM_CFG     (I2C_M_SCB_MODE_I2C      == I2C_M_scbMode)
    #define I2C_M_SCB_MODE_SPI_RUNTM_CFG     (I2C_M_SCB_MODE_SPI      == I2C_M_scbMode)
    #define I2C_M_SCB_MODE_UART_RUNTM_CFG    (I2C_M_SCB_MODE_UART     == I2C_M_scbMode)
    #define I2C_M_SCB_MODE_EZI2C_RUNTM_CFG   (I2C_M_SCB_MODE_EZI2C    == I2C_M_scbMode)
    #define I2C_M_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (I2C_M_SCB_MODE_UNCONFIG == I2C_M_scbMode)

    /* Defines wakeup enable */
    #define I2C_M_SCB_WAKE_ENABLE_CHECK       (0u != I2C_M_scbEnableWake)
#endif /* (I2C_M_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!I2C_M_CY_SCBIP_V1)
    #define I2C_M_SCB_PINS_NUMBER    (7u)
#else
    #define I2C_M_SCB_PINS_NUMBER    (2u)
#endif /* (!I2C_M_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_I2C_M_H) */


/* [] END OF FILE */
