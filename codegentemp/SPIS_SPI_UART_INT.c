/***************************************************************************//**
* \file SPIS_SPI_UART_INT.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPIS_PVT.h"
#include "SPIS_SPI_UART_PVT.h"


#if (SPIS_SCB_IRQ_INTERNAL)
/*******************************************************************************
* Function Name: SPIS_SPI_UART_ISR
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
*******************************************************************************/
CY_ISR(SPIS_SPI_UART_ISR)
{
#if (SPIS_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
#endif /* (SPIS_INTERNAL_RX_SW_BUFFER_CONST) */

#if (SPIS_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (SPIS_INTERNAL_TX_SW_BUFFER_CONST) */

#ifdef SPIS_SPI_UART_ISR_ENTRY_CALLBACK
    SPIS_SPI_UART_ISR_EntryCallback();
#endif /* SPIS_SPI_UART_ISR_ENTRY_CALLBACK */

    if (NULL != SPIS_customIntrHandler)
    {
        SPIS_customIntrHandler();
    }

    #if(SPIS_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        SPIS_ClearSpiExtClkInterruptSource(SPIS_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if (SPIS_CHECK_RX_SW_BUFFER)
    {
        if (SPIS_CHECK_INTR_RX_MASKED(SPIS_INTR_RX_NOT_EMPTY))
        {
            do
            {
                /* Move local head index */
                locHead = (SPIS_rxBufferHead + 1u);

                /* Adjust local head index */
                if (SPIS_INTERNAL_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if (locHead == SPIS_rxBufferTail)
                {
                    #if (SPIS_CHECK_UART_RTS_CONTROL_FLOW)
                    {
                        /* There is no space in the software buffer - disable the
                        * RX Not Empty interrupt source. The data elements are
                        * still being received into the RX FIFO until the RTS signal
                        * stops the transmitter. After the data element is read from the
                        * buffer, the RX Not Empty interrupt source is enabled to
                        * move the next data element in the software buffer.
                        */
                        SPIS_INTR_RX_MASK_REG &= ~SPIS_INTR_RX_NOT_EMPTY;
                        break;
                    }
                    #else
                    {
                        /* Overflow: through away received data element */
                        (void) SPIS_RX_FIFO_RD_REG;
                        SPIS_rxBufferOverflow = (uint8) SPIS_INTR_RX_OVERFLOW;
                    }
                    #endif
                }
                else
                {
                    /* Store received data */
                    SPIS_PutWordInRxBuffer(locHead, SPIS_RX_FIFO_RD_REG);

                    /* Move head index */
                    SPIS_rxBufferHead = locHead;
                }
            }
            while(0u != SPIS_GET_RX_FIFO_ENTRIES);

            SPIS_ClearRxInterruptSource(SPIS_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if (SPIS_CHECK_TX_SW_BUFFER)
    {
        if (SPIS_CHECK_INTR_TX_MASKED(SPIS_INTR_TX_NOT_FULL))
        {
            do
            {
                /* Check for room in TX software buffer */
                if (SPIS_txBufferHead != SPIS_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (SPIS_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if (SPIS_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    SPIS_TX_FIFO_WR_REG = SPIS_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    SPIS_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    SPIS_DISABLE_INTR_TX(SPIS_INTR_TX_NOT_FULL);
                    break;
                }
            }
            while (SPIS_SPI_UART_FIFO_SIZE != SPIS_GET_TX_FIFO_ENTRIES);

            SPIS_ClearTxInterruptSource(SPIS_INTR_TX_NOT_FULL);
        }
    }
    #endif

#ifdef SPIS_SPI_UART_ISR_EXIT_CALLBACK
    SPIS_SPI_UART_ISR_ExitCallback();
#endif /* SPIS_SPI_UART_ISR_EXIT_CALLBACK */

}

#endif /* (SPIS_SCB_IRQ_INTERNAL) */


/* [] END OF FILE */
