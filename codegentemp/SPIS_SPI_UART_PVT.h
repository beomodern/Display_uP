/***************************************************************************//**
* \file SPIS_SPI_UART_PVT.h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
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

#if !defined(CY_SCB_SPI_UART_PVT_SPIS_H)
#define CY_SCB_SPI_UART_PVT_SPIS_H

#include "SPIS_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (SPIS_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  SPIS_rxBufferHead;
    extern volatile uint32  SPIS_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   SPIS_rxBufferOverflow;
    /** @} globals */
#endif /* (SPIS_INTERNAL_RX_SW_BUFFER_CONST) */

#if (SPIS_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  SPIS_txBufferHead;
    extern volatile uint32  SPIS_txBufferTail;
#endif /* (SPIS_INTERNAL_TX_SW_BUFFER_CONST) */

#if (SPIS_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 SPIS_rxBufferInternal[SPIS_INTERNAL_RX_BUFFER_SIZE];
#endif /* (SPIS_INTERNAL_RX_SW_BUFFER) */

#if (SPIS_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 SPIS_txBufferInternal[SPIS_TX_BUFFER_SIZE];
#endif /* (SPIS_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void SPIS_SpiPostEnable(void);
void SPIS_SpiStop(void);

#if (SPIS_SCB_MODE_SPI_CONST_CFG)
    void SPIS_SpiInit(void);
#endif /* (SPIS_SCB_MODE_SPI_CONST_CFG) */

#if (SPIS_SPI_WAKE_ENABLE_CONST)
    void SPIS_SpiSaveConfig(void);
    void SPIS_SpiRestoreConfig(void);
#endif /* (SPIS_SPI_WAKE_ENABLE_CONST) */

void SPIS_UartPostEnable(void);
void SPIS_UartStop(void);

#if (SPIS_SCB_MODE_UART_CONST_CFG)
    void SPIS_UartInit(void);
#endif /* (SPIS_SCB_MODE_UART_CONST_CFG) */

#if (SPIS_UART_WAKE_ENABLE_CONST)
    void SPIS_UartSaveConfig(void);
    void SPIS_UartRestoreConfig(void);
#endif /* (SPIS_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in SPIS_SetPins() */
#define SPIS_UART_RX_PIN_ENABLE    (SPIS_UART_RX)
#define SPIS_UART_TX_PIN_ENABLE    (SPIS_UART_TX)

/* UART RTS and CTS position to be used in  SPIS_SetPins() */
#define SPIS_UART_RTS_PIN_ENABLE    (0x10u)
#define SPIS_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define SPIS_SpiUartEnableIntRx(intSourceMask)  SPIS_SetRxInterruptMode(intSourceMask)
#define SPIS_SpiUartEnableIntTx(intSourceMask)  SPIS_SetTxInterruptMode(intSourceMask)
uint32  SPIS_SpiUartDisableIntRx(void);
uint32  SPIS_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_SPIS_H) */


/* [] END OF FILE */
