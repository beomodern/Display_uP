/***************************************************************************//**
* \file SPIS_BOOT.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_SPIS_H)
#define CY_SCB_BOOT_SPIS_H

#include "SPIS_PVT.h"

#if (SPIS_SCB_MODE_I2C_INC)
    #include "SPIS_I2C.h"
#endif /* (SPIS_SCB_MODE_I2C_INC) */

#if (SPIS_SCB_MODE_EZI2C_INC)
    #include "SPIS_EZI2C.h"
#endif /* (SPIS_SCB_MODE_EZI2C_INC) */

#if (SPIS_SCB_MODE_SPI_INC || SPIS_SCB_MODE_UART_INC)
    #include "SPIS_SPI_UART.h"
#endif /* (SPIS_SCB_MODE_SPI_INC || SPIS_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define SPIS_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SPIS) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (SPIS_SCB_MODE_I2C_INC)
    #define SPIS_I2C_BTLDR_COMM_ENABLED     (SPIS_BTLDR_COMM_ENABLED && \
                                                            (SPIS_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             SPIS_I2C_SLAVE_CONST))
#else
     #define SPIS_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (SPIS_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (SPIS_SCB_MODE_EZI2C_INC)
    #define SPIS_EZI2C_BTLDR_COMM_ENABLED   (SPIS_BTLDR_COMM_ENABLED && \
                                                         SPIS_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define SPIS_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (SPIS_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (SPIS_SCB_MODE_SPI_INC)
    #define SPIS_SPI_BTLDR_COMM_ENABLED     (SPIS_BTLDR_COMM_ENABLED && \
                                                            (SPIS_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             SPIS_SPI_SLAVE_CONST))
#else
        #define SPIS_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (SPIS_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (SPIS_SCB_MODE_UART_INC)
       #define SPIS_UART_BTLDR_COMM_ENABLED    (SPIS_BTLDR_COMM_ENABLED && \
                                                            (SPIS_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (SPIS_UART_RX_DIRECTION && \
                                                              SPIS_UART_TX_DIRECTION)))
#else
     #define SPIS_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (SPIS_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define SPIS_BTLDR_COMM_MODE_ENABLED    (SPIS_I2C_BTLDR_COMM_ENABLED   || \
                                                     SPIS_SPI_BTLDR_COMM_ENABLED   || \
                                                     SPIS_EZI2C_BTLDR_COMM_ENABLED || \
                                                     SPIS_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SPIS_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void SPIS_I2CCyBtldrCommStart(void);
    void SPIS_I2CCyBtldrCommStop (void);
    void SPIS_I2CCyBtldrCommReset(void);
    cystatus SPIS_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus SPIS_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (SPIS_SCB_MODE_I2C_CONST_CFG)
        #define SPIS_CyBtldrCommStart   SPIS_I2CCyBtldrCommStart
        #define SPIS_CyBtldrCommStop    SPIS_I2CCyBtldrCommStop
        #define SPIS_CyBtldrCommReset   SPIS_I2CCyBtldrCommReset
        #define SPIS_CyBtldrCommRead    SPIS_I2CCyBtldrCommRead
        #define SPIS_CyBtldrCommWrite   SPIS_I2CCyBtldrCommWrite
    #endif /* (SPIS_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SPIS_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SPIS_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void SPIS_EzI2CCyBtldrCommStart(void);
    void SPIS_EzI2CCyBtldrCommStop (void);
    void SPIS_EzI2CCyBtldrCommReset(void);
    cystatus SPIS_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus SPIS_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (SPIS_SCB_MODE_EZI2C_CONST_CFG)
        #define SPIS_CyBtldrCommStart   SPIS_EzI2CCyBtldrCommStart
        #define SPIS_CyBtldrCommStop    SPIS_EzI2CCyBtldrCommStop
        #define SPIS_CyBtldrCommReset   SPIS_EzI2CCyBtldrCommReset
        #define SPIS_CyBtldrCommRead    SPIS_EzI2CCyBtldrCommRead
        #define SPIS_CyBtldrCommWrite   SPIS_EzI2CCyBtldrCommWrite
    #endif /* (SPIS_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SPIS_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SPIS_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void SPIS_SpiCyBtldrCommStart(void);
    void SPIS_SpiCyBtldrCommStop (void);
    void SPIS_SpiCyBtldrCommReset(void);
    cystatus SPIS_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus SPIS_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (SPIS_SCB_MODE_SPI_CONST_CFG)
        #define SPIS_CyBtldrCommStart   SPIS_SpiCyBtldrCommStart
        #define SPIS_CyBtldrCommStop    SPIS_SpiCyBtldrCommStop
        #define SPIS_CyBtldrCommReset   SPIS_SpiCyBtldrCommReset
        #define SPIS_CyBtldrCommRead    SPIS_SpiCyBtldrCommRead
        #define SPIS_CyBtldrCommWrite   SPIS_SpiCyBtldrCommWrite
    #endif /* (SPIS_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SPIS_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SPIS_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void SPIS_UartCyBtldrCommStart(void);
    void SPIS_UartCyBtldrCommStop (void);
    void SPIS_UartCyBtldrCommReset(void);
    cystatus SPIS_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus SPIS_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (SPIS_SCB_MODE_UART_CONST_CFG)
        #define SPIS_CyBtldrCommStart   SPIS_UartCyBtldrCommStart
        #define SPIS_CyBtldrCommStop    SPIS_UartCyBtldrCommStop
        #define SPIS_CyBtldrCommReset   SPIS_UartCyBtldrCommReset
        #define SPIS_CyBtldrCommRead    SPIS_UartCyBtldrCommRead
        #define SPIS_CyBtldrCommWrite   SPIS_UartCyBtldrCommWrite
    #endif /* (SPIS_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SPIS_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SPIS_BTLDR_COMM_ENABLED)
    #if (SPIS_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void SPIS_CyBtldrCommStart(void);
        void SPIS_CyBtldrCommStop (void);
        void SPIS_CyBtldrCommReset(void);
        cystatus SPIS_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus SPIS_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (SPIS_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SPIS)
        #define CyBtldrCommStart    SPIS_CyBtldrCommStart
        #define CyBtldrCommStop     SPIS_CyBtldrCommStop
        #define CyBtldrCommReset    SPIS_CyBtldrCommReset
        #define CyBtldrCommWrite    SPIS_CyBtldrCommWrite
        #define CyBtldrCommRead     SPIS_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SPIS) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SPIS_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define SPIS_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define SPIS_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define SPIS_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define SPIS_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef SPIS_SPI_BYTE_TO_BYTE
    #define SPIS_SPI_BYTE_TO_BYTE   (16u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef SPIS_UART_BYTE_TO_BYTE
    #define SPIS_UART_BYTE_TO_BYTE  (2500u)
#endif /* SPIS_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_SPIS_H) */


/* [] END OF FILE */
