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

#if !defined(CY_SCB_PVT_SPIS_H)
#define CY_SCB_PVT_SPIS_H

#include "SPIS.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SPIS_SetI2CExtClkInterruptMode(interruptMask) SPIS_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SPIS_ClearI2CExtClkInterruptSource(interruptMask) SPIS_CLEAR_INTR_I2C_EC(interruptMask)
#define SPIS_GetI2CExtClkInterruptSource()                (SPIS_INTR_I2C_EC_REG)
#define SPIS_GetI2CExtClkInterruptMode()                  (SPIS_INTR_I2C_EC_MASK_REG)
#define SPIS_GetI2CExtClkInterruptSourceMasked()          (SPIS_INTR_I2C_EC_MASKED_REG)

#if (!SPIS_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define SPIS_SetSpiExtClkInterruptMode(interruptMask) \
                                                                SPIS_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define SPIS_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                SPIS_CLEAR_INTR_SPI_EC(interruptMask)
    #define SPIS_GetExtSpiClkInterruptSource()                 (SPIS_INTR_SPI_EC_REG)
    #define SPIS_GetExtSpiClkInterruptMode()                   (SPIS_INTR_SPI_EC_MASK_REG)
    #define SPIS_GetExtSpiClkInterruptSourceMasked()           (SPIS_INTR_SPI_EC_MASKED_REG)
#endif /* (!SPIS_CY_SCBIP_V1) */

#if(SPIS_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SPIS_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (SPIS_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (SPIS_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SPIS_CUSTOM_INTR_HANDLER)
    extern cyisraddress SPIS_customIntrHandler;
#endif /* !defined (CY_REMOVE_SPIS_CUSTOM_INTR_HANDLER) */
#endif /* (SPIS_SCB_IRQ_INTERNAL) */

extern SPIS_BACKUP_STRUCT SPIS_backup;

#if(SPIS_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 SPIS_scbMode;
    extern uint8 SPIS_scbEnableWake;
    extern uint8 SPIS_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 SPIS_mode;
    extern uint8 SPIS_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * SPIS_rxBuffer;
    extern uint8   SPIS_rxDataBits;
    extern uint32  SPIS_rxBufferSize;

    extern volatile uint8 * SPIS_txBuffer;
    extern uint8   SPIS_txDataBits;
    extern uint32  SPIS_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 SPIS_numberOfAddr;
    extern uint8 SPIS_subAddrSize;
#endif /* (SPIS_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (SPIS_SCB_MODE_I2C_CONST_CFG || \
        SPIS_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 SPIS_IntrTxMask;
#endif /* (! (SPIS_SCB_MODE_I2C_CONST_CFG || \
              SPIS_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(SPIS_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define SPIS_SCB_MODE_I2C_RUNTM_CFG     (SPIS_SCB_MODE_I2C      == SPIS_scbMode)
    #define SPIS_SCB_MODE_SPI_RUNTM_CFG     (SPIS_SCB_MODE_SPI      == SPIS_scbMode)
    #define SPIS_SCB_MODE_UART_RUNTM_CFG    (SPIS_SCB_MODE_UART     == SPIS_scbMode)
    #define SPIS_SCB_MODE_EZI2C_RUNTM_CFG   (SPIS_SCB_MODE_EZI2C    == SPIS_scbMode)
    #define SPIS_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (SPIS_SCB_MODE_UNCONFIG == SPIS_scbMode)

    /* Defines wakeup enable */
    #define SPIS_SCB_WAKE_ENABLE_CHECK       (0u != SPIS_scbEnableWake)
#endif /* (SPIS_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!SPIS_CY_SCBIP_V1)
    #define SPIS_SCB_PINS_NUMBER    (7u)
#else
    #define SPIS_SCB_PINS_NUMBER    (2u)
#endif /* (!SPIS_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_SPIS_H) */


/* [] END OF FILE */
