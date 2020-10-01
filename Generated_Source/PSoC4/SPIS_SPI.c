/***************************************************************************//**
* \file SPIS_SPI.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  SPI mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPIS_PVT.h"
#include "SPIS_SPI_UART_PVT.h"

#if(SPIS_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const SPIS_SPI_INIT_STRUCT SPIS_configSpi =
    {
        SPIS_SPI_MODE,
        SPIS_SPI_SUB_MODE,
        SPIS_SPI_CLOCK_MODE,
        SPIS_SPI_OVS_FACTOR,
        SPIS_SPI_MEDIAN_FILTER_ENABLE,
        SPIS_SPI_LATE_MISO_SAMPLE_ENABLE,
        SPIS_SPI_WAKE_ENABLE,
        SPIS_SPI_RX_DATA_BITS_NUM,
        SPIS_SPI_TX_DATA_BITS_NUM,
        SPIS_SPI_BITS_ORDER,
        SPIS_SPI_TRANSFER_SEPARATION,
        0u,
        NULL,
        0u,
        NULL,
        (uint32) SPIS_SCB_IRQ_INTERNAL,
        SPIS_SPI_INTR_RX_MASK,
        SPIS_SPI_RX_TRIGGER_LEVEL,
        SPIS_SPI_INTR_TX_MASK,
        SPIS_SPI_TX_TRIGGER_LEVEL,
        (uint8) SPIS_SPI_BYTE_MODE_ENABLE,
        (uint8) SPIS_SPI_FREE_RUN_SCLK_ENABLE,
        (uint8) SPIS_SPI_SS_POLARITY
    };


    /*******************************************************************************
    * Function Name: SPIS_SpiInit
    ****************************************************************************//**
    *
    *  Configures the SPIS for SPI operation.
    *
    *  This function is intended specifically to be used when the SPIS 
    *  configuration is set to “Unconfigured SPIS” in the customizer. 
    *  After initializing the SPIS in SPI mode using this function, 
    *  the component can be enabled using the SPIS_Start() or 
    * SPIS_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration 
    *  settings. This structure contains the same information that would otherwise 
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of 
    *   fields. These fields match the selections available in the customizer. 
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void SPIS_SpiInit(const SPIS_SPI_INIT_STRUCT *config)
    {
        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            SPIS_SetPins(SPIS_SCB_MODE_SPI, config->mode, SPIS_DUMMY_PARAM);

            /* Store internal configuration */
            SPIS_scbMode       = (uint8) SPIS_SCB_MODE_SPI;
            SPIS_scbEnableWake = (uint8) config->enableWake;
            SPIS_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            SPIS_rxBuffer      =         config->rxBuffer;
            SPIS_rxDataBits    = (uint8) config->rxDataBits;
            SPIS_rxBufferSize  =         config->rxBufferSize;

            /* Set TX direction internal variables */
            SPIS_txBuffer      =         config->txBuffer;
            SPIS_txDataBits    = (uint8) config->txDataBits;
            SPIS_txBufferSize  =         config->txBufferSize;

            /* Configure SPI interface */
            SPIS_CTRL_REG     = SPIS_GET_CTRL_OVS(config->oversample)           |
                                            SPIS_GET_CTRL_BYTE_MODE(config->enableByteMode) |
                                            SPIS_GET_CTRL_EC_AM_MODE(config->enableWake)    |
                                            SPIS_CTRL_SPI;

            SPIS_SPI_CTRL_REG = SPIS_GET_SPI_CTRL_CONTINUOUS    (config->transferSeperation)  |
                                            SPIS_GET_SPI_CTRL_SELECT_PRECEDE(config->submode &
                                                                          SPIS_SPI_MODE_TI_PRECEDES_MASK) |
                                            SPIS_GET_SPI_CTRL_SCLK_MODE     (config->sclkMode)            |
                                            SPIS_GET_SPI_CTRL_LATE_MISO_SAMPLE(config->enableLateSampling)|
                                            SPIS_GET_SPI_CTRL_SCLK_CONTINUOUS(config->enableFreeRunSclk)  |
                                            SPIS_GET_SPI_CTRL_SSEL_POLARITY (config->polaritySs)          |
                                            SPIS_GET_SPI_CTRL_SUB_MODE      (config->submode)             |
                                            SPIS_GET_SPI_CTRL_MASTER_MODE   (config->mode);

            /* Configure RX direction */
            SPIS_RX_CTRL_REG     =  SPIS_GET_RX_CTRL_DATA_WIDTH(config->rxDataBits)         |
                                                SPIS_GET_RX_CTRL_BIT_ORDER (config->bitOrder)           |
                                                SPIS_GET_RX_CTRL_MEDIAN    (config->enableMedianFilter) |
                                                SPIS_SPI_RX_CTRL;

            SPIS_RX_FIFO_CTRL_REG = SPIS_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure TX direction */
            SPIS_TX_CTRL_REG      = SPIS_GET_TX_CTRL_DATA_WIDTH(config->txDataBits) |
                                                SPIS_GET_TX_CTRL_BIT_ORDER (config->bitOrder)   |
                                                SPIS_SPI_TX_CTRL;

            SPIS_TX_FIFO_CTRL_REG = SPIS_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

            /* Configure interrupt with SPI handler but do not enable it */
            CyIntDisable    (SPIS_ISR_NUMBER);
            CyIntSetPriority(SPIS_ISR_NUMBER, SPIS_ISR_PRIORITY);
            (void) CyIntSetVector(SPIS_ISR_NUMBER, &SPIS_SPI_UART_ISR);

            /* Configure interrupt sources */
            SPIS_INTR_I2C_EC_MASK_REG = SPIS_NO_INTR_SOURCES;
            SPIS_INTR_SPI_EC_MASK_REG = SPIS_NO_INTR_SOURCES;
            SPIS_INTR_SLAVE_MASK_REG  = SPIS_GET_SPI_INTR_SLAVE_MASK(config->rxInterruptMask);
            SPIS_INTR_MASTER_MASK_REG = SPIS_GET_SPI_INTR_MASTER_MASK(config->txInterruptMask);
            SPIS_INTR_RX_MASK_REG     = SPIS_GET_SPI_INTR_RX_MASK(config->rxInterruptMask);
            SPIS_INTR_TX_MASK_REG     = SPIS_GET_SPI_INTR_TX_MASK(config->txInterruptMask);
            
            /* Configure TX interrupt sources to restore. */
            SPIS_IntrTxMask = LO16(SPIS_INTR_TX_MASK_REG);

            /* Set active SS0 */
            SPIS_SpiSetActiveSlaveSelect(SPIS_SPI_SLAVE_SELECT0);

            /* Clear RX buffer indexes */
            SPIS_rxBufferHead     = 0u;
            SPIS_rxBufferTail     = 0u;
            SPIS_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            SPIS_txBufferHead = 0u;
            SPIS_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: SPIS_SpiInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the SPI operation.
    *
    *******************************************************************************/
    void SPIS_SpiInit(void)
    {
        /* Configure SPI interface */
        SPIS_CTRL_REG     = SPIS_SPI_DEFAULT_CTRL;
        SPIS_SPI_CTRL_REG = SPIS_SPI_DEFAULT_SPI_CTRL;

        /* Configure TX and RX direction */
        SPIS_RX_CTRL_REG      = SPIS_SPI_DEFAULT_RX_CTRL;
        SPIS_RX_FIFO_CTRL_REG = SPIS_SPI_DEFAULT_RX_FIFO_CTRL;

        /* Configure TX and RX direction */
        SPIS_TX_CTRL_REG      = SPIS_SPI_DEFAULT_TX_CTRL;
        SPIS_TX_FIFO_CTRL_REG = SPIS_SPI_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with SPI handler but do not enable it */
    #if(SPIS_SCB_IRQ_INTERNAL)
            CyIntDisable    (SPIS_ISR_NUMBER);
            CyIntSetPriority(SPIS_ISR_NUMBER, SPIS_ISR_PRIORITY);
            (void) CyIntSetVector(SPIS_ISR_NUMBER, &SPIS_SPI_UART_ISR);
    #endif /* (SPIS_SCB_IRQ_INTERNAL) */

        /* Configure interrupt sources */
        SPIS_INTR_I2C_EC_MASK_REG = SPIS_SPI_DEFAULT_INTR_I2C_EC_MASK;
        SPIS_INTR_SPI_EC_MASK_REG = SPIS_SPI_DEFAULT_INTR_SPI_EC_MASK;
        SPIS_INTR_SLAVE_MASK_REG  = SPIS_SPI_DEFAULT_INTR_SLAVE_MASK;
        SPIS_INTR_MASTER_MASK_REG = SPIS_SPI_DEFAULT_INTR_MASTER_MASK;
        SPIS_INTR_RX_MASK_REG     = SPIS_SPI_DEFAULT_INTR_RX_MASK;
        SPIS_INTR_TX_MASK_REG     = SPIS_SPI_DEFAULT_INTR_TX_MASK;

        /* Configure TX interrupt sources to restore. */
        SPIS_IntrTxMask = LO16(SPIS_INTR_TX_MASK_REG);
            
        /* Set active SS0 for master */
    #if (SPIS_SPI_MASTER_CONST)
        SPIS_SpiSetActiveSlaveSelect(SPIS_SPI_SLAVE_SELECT0);
    #endif /* (SPIS_SPI_MASTER_CONST) */

    #if(SPIS_INTERNAL_RX_SW_BUFFER_CONST)
        SPIS_rxBufferHead     = 0u;
        SPIS_rxBufferTail     = 0u;
        SPIS_rxBufferOverflow = 0u;
    #endif /* (SPIS_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(SPIS_INTERNAL_TX_SW_BUFFER_CONST)
        SPIS_txBufferHead = 0u;
        SPIS_txBufferTail = 0u;
    #endif /* (SPIS_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (SPIS_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: SPIS_SpiPostEnable
****************************************************************************//**
*
*  Restores HSIOM settings for the SPI master output pins (SCLK and/or SS0-SS3) 
*  to be controlled by the SCB SPI.
*
*******************************************************************************/
void SPIS_SpiPostEnable(void)
{
#if(SPIS_SCB_MODE_UNCONFIG_CONST_CFG)

    if (SPIS_CHECK_SPI_MASTER)
    {
    #if (SPIS_SCLK_PIN)
        /* Set SCB SPI to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SCLK_HSIOM_REG, SPIS_SCLK_HSIOM_MASK,
                                       SPIS_SCLK_HSIOM_POS, SPIS_SCLK_HSIOM_SEL_SPI);
    #endif /* (SPIS_SCLK_PIN) */

    #if (SPIS_SS0_PIN)
        /* Set SCB SPI to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS0_HSIOM_REG, SPIS_SS0_HSIOM_MASK,
                                       SPIS_SS0_HSIOM_POS, SPIS_SS0_HSIOM_SEL_SPI);
    #endif /* (SPIS_SS0_PIN) */

    #if (SPIS_SS1_PIN)
        /* Set SCB SPI to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS1_HSIOM_REG, SPIS_SS1_HSIOM_MASK,
                                       SPIS_SS1_HSIOM_POS, SPIS_SS1_HSIOM_SEL_SPI);
    #endif /* (SPIS_SS1_PIN) */

    #if (SPIS_SS2_PIN)
        /* Set SCB SPI to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS2_HSIOM_REG, SPIS_SS2_HSIOM_MASK,
                                       SPIS_SS2_HSIOM_POS, SPIS_SS2_HSIOM_SEL_SPI);
    #endif /* (SPIS_SS2_PIN) */

    #if (SPIS_SS3_PIN)
        /* Set SCB SPI to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS3_HSIOM_REG, SPIS_SS3_HSIOM_MASK,
                                       SPIS_SS3_HSIOM_POS, SPIS_SS3_HSIOM_SEL_SPI);
    #endif /* (SPIS_SS3_PIN) */
    }

#else

    #if (SPIS_SPI_MASTER_SCLK_PIN)
        /* Set SCB SPI to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SCLK_M_HSIOM_REG, SPIS_SCLK_M_HSIOM_MASK,
                                       SPIS_SCLK_M_HSIOM_POS, SPIS_SCLK_M_HSIOM_SEL_SPI);
    #endif /* (SPIS_MISO_SDA_TX_PIN_PIN) */

    #if (SPIS_SPI_MASTER_SS0_PIN)
        /* Set SCB SPI to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS0_M_HSIOM_REG, SPIS_SS0_M_HSIOM_MASK,
                                       SPIS_SS0_M_HSIOM_POS, SPIS_SS0_M_HSIOM_SEL_SPI);
    #endif /* (SPIS_SPI_MASTER_SS0_PIN) */

    #if (SPIS_SPI_MASTER_SS1_PIN)
        /* Set SCB SPI to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS1_M_HSIOM_REG, SPIS_SS1_M_HSIOM_MASK,
                                       SPIS_SS1_M_HSIOM_POS, SPIS_SS1_M_HSIOM_SEL_SPI);
    #endif /* (SPIS_SPI_MASTER_SS1_PIN) */

    #if (SPIS_SPI_MASTER_SS2_PIN)
        /* Set SCB SPI to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS2_M_HSIOM_REG, SPIS_SS2_M_HSIOM_MASK,
                                       SPIS_SS2_M_HSIOM_POS, SPIS_SS2_M_HSIOM_SEL_SPI);
    #endif /* (SPIS_SPI_MASTER_SS2_PIN) */

    #if (SPIS_SPI_MASTER_SS3_PIN)
        /* Set SCB SPI to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS3_M_HSIOM_REG, SPIS_SS3_M_HSIOM_MASK,
                                       SPIS_SS3_M_HSIOM_POS, SPIS_SS3_M_HSIOM_SEL_SPI);
    #endif /* (SPIS_SPI_MASTER_SS3_PIN) */

#endif /* (SPIS_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Restore TX interrupt sources. */
    SPIS_SetTxInterruptMode(SPIS_IntrTxMask);
}


/*******************************************************************************
* Function Name: SPIS_SpiStop
****************************************************************************//**
*
*  Changes the HSIOM settings for the SPI master output pins 
*  (SCLK and/or SS0-SS3) to keep them inactive after the block is disabled. 
*  The output pins are controlled by the GPIO data register.
*
*******************************************************************************/
void SPIS_SpiStop(void)
{
#if(SPIS_SCB_MODE_UNCONFIG_CONST_CFG)

    if (SPIS_CHECK_SPI_MASTER)
    {
    #if (SPIS_SCLK_PIN)
        /* Set output pin state after block is disabled */
        SPIS_spi_sclk_Write(SPIS_GET_SPI_SCLK_INACTIVE);

        /* Set GPIO to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SCLK_HSIOM_REG, SPIS_SCLK_HSIOM_MASK,
                                       SPIS_SCLK_HSIOM_POS, SPIS_SCLK_HSIOM_SEL_GPIO);
    #endif /* (SPIS_spi_sclk_PIN) */

    #if (SPIS_SS0_PIN)
        /* Set output pin state after block is disabled */
        SPIS_spi_ss0_Write(SPIS_GET_SPI_SS0_INACTIVE);

        /* Set GPIO to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS0_HSIOM_REG, SPIS_SS0_HSIOM_MASK,
                                       SPIS_SS0_HSIOM_POS, SPIS_SS0_HSIOM_SEL_GPIO);
    #endif /* (SPIS_spi_ss0_PIN) */

    #if (SPIS_SS1_PIN)
        /* Set output pin state after block is disabled */
        SPIS_spi_ss1_Write(SPIS_GET_SPI_SS1_INACTIVE);

        /* Set GPIO to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS1_HSIOM_REG, SPIS_SS1_HSIOM_MASK,
                                       SPIS_SS1_HSIOM_POS, SPIS_SS1_HSIOM_SEL_GPIO);
    #endif /* (SPIS_SS1_PIN) */

    #if (SPIS_SS2_PIN)
        /* Set output pin state after block is disabled */
        SPIS_spi_ss2_Write(SPIS_GET_SPI_SS2_INACTIVE);

        /* Set GPIO to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS2_HSIOM_REG, SPIS_SS2_HSIOM_MASK,
                                       SPIS_SS2_HSIOM_POS, SPIS_SS2_HSIOM_SEL_GPIO);
    #endif /* (SPIS_SS2_PIN) */

    #if (SPIS_SS3_PIN)
        /* Set output pin state after block is disabled */
        SPIS_spi_ss3_Write(SPIS_GET_SPI_SS3_INACTIVE);

        /* Set GPIO to drive output pin */
        SPIS_SET_HSIOM_SEL(SPIS_SS3_HSIOM_REG, SPIS_SS3_HSIOM_MASK,
                                       SPIS_SS3_HSIOM_POS, SPIS_SS3_HSIOM_SEL_GPIO);
    #endif /* (SPIS_SS3_PIN) */
    
        /* Store TX interrupt sources (exclude level triggered) for master. */
        SPIS_IntrTxMask = LO16(SPIS_GetTxInterruptMode() & SPIS_INTR_SPIM_TX_RESTORE);
    }
    else
    {
        /* Store TX interrupt sources (exclude level triggered) for slave. */
        SPIS_IntrTxMask = LO16(SPIS_GetTxInterruptMode() & SPIS_INTR_SPIS_TX_RESTORE);
    }

#else

#if (SPIS_SPI_MASTER_SCLK_PIN)
    /* Set output pin state after block is disabled */
    SPIS_sclk_m_Write(SPIS_GET_SPI_SCLK_INACTIVE);

    /* Set GPIO to drive output pin */
    SPIS_SET_HSIOM_SEL(SPIS_SCLK_M_HSIOM_REG, SPIS_SCLK_M_HSIOM_MASK,
                                   SPIS_SCLK_M_HSIOM_POS, SPIS_SCLK_M_HSIOM_SEL_GPIO);
#endif /* (SPIS_MISO_SDA_TX_PIN_PIN) */

#if (SPIS_SPI_MASTER_SS0_PIN)
    /* Set output pin state after block is disabled */
    SPIS_ss0_m_Write(SPIS_GET_SPI_SS0_INACTIVE);

    /* Set GPIO to drive output pin */
    SPIS_SET_HSIOM_SEL(SPIS_SS0_M_HSIOM_REG, SPIS_SS0_M_HSIOM_MASK,
                                   SPIS_SS0_M_HSIOM_POS, SPIS_SS0_M_HSIOM_SEL_GPIO);
#endif /* (SPIS_SPI_MASTER_SS0_PIN) */

#if (SPIS_SPI_MASTER_SS1_PIN)
    /* Set output pin state after block is disabled */
    SPIS_ss1_m_Write(SPIS_GET_SPI_SS1_INACTIVE);

    /* Set GPIO to drive output pin */
    SPIS_SET_HSIOM_SEL(SPIS_SS1_M_HSIOM_REG, SPIS_SS1_M_HSIOM_MASK,
                                   SPIS_SS1_M_HSIOM_POS, SPIS_SS1_M_HSIOM_SEL_GPIO);
#endif /* (SPIS_SPI_MASTER_SS1_PIN) */

#if (SPIS_SPI_MASTER_SS2_PIN)
    /* Set output pin state after block is disabled */
    SPIS_ss2_m_Write(SPIS_GET_SPI_SS2_INACTIVE);

    /* Set GPIO to drive output pin */
    SPIS_SET_HSIOM_SEL(SPIS_SS2_M_HSIOM_REG, SPIS_SS2_M_HSIOM_MASK,
                                   SPIS_SS2_M_HSIOM_POS, SPIS_SS2_M_HSIOM_SEL_GPIO);
#endif /* (SPIS_SPI_MASTER_SS2_PIN) */

#if (SPIS_SPI_MASTER_SS3_PIN)
    /* Set output pin state after block is disabled */
    SPIS_ss3_m_Write(SPIS_GET_SPI_SS3_INACTIVE);

    /* Set GPIO to drive output pin */
    SPIS_SET_HSIOM_SEL(SPIS_SS3_M_HSIOM_REG, SPIS_SS3_M_HSIOM_MASK,
                                   SPIS_SS3_M_HSIOM_POS, SPIS_SS3_M_HSIOM_SEL_GPIO);
#endif /* (SPIS_SPI_MASTER_SS3_PIN) */

    #if (SPIS_SPI_MASTER_CONST)
        /* Store TX interrupt sources (exclude level triggered). */
        SPIS_IntrTxMask = LO16(SPIS_GetTxInterruptMode() & SPIS_INTR_SPIM_TX_RESTORE);
    #else
        /* Store TX interrupt sources (exclude level triggered). */
        SPIS_IntrTxMask = LO16(SPIS_GetTxInterruptMode() & SPIS_INTR_SPIS_TX_RESTORE);
    #endif /* (SPIS_SPI_MASTER_CONST) */

#endif /* (SPIS_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (SPIS_SPI_MASTER_CONST)
    /*******************************************************************************
    * Function Name: SPIS_SetActiveSlaveSelect
    ****************************************************************************//**
    *
    *  Selects one of the four slave select lines to be active during the transfer.
    *  After initialization the active slave select line is 0.
    *  The component should be in one of the following states to change the active
    *  slave select signal source correctly:
    *   - The component is disabled.
    *   - The component has completed transfer (TX FIFO is empty and the
    *     SCB_INTR_MASTER_SPI_DONE status is set).
    *
    *  This function does not check that these conditions are met.
    *  This function is only applicable to SPI Master mode of operation.
    *
    *  \param slaveSelect: slave select line which will be active while the following
    *   transfer.
    *   - SPIS_SPI_SLAVE_SELECT0 - Slave select 0.
    *   - SPIS_SPI_SLAVE_SELECT1 - Slave select 1.
    *   - SPIS_SPI_SLAVE_SELECT2 - Slave select 2.
    *   - SPIS_SPI_SLAVE_SELECT3 - Slave select 3.
    *
    *******************************************************************************/
    void SPIS_SpiSetActiveSlaveSelect(uint32 slaveSelect)
    {
        uint32 spiCtrl;

        spiCtrl = SPIS_SPI_CTRL_REG;

        spiCtrl &= (uint32) ~SPIS_SPI_CTRL_SLAVE_SELECT_MASK;
        spiCtrl |= (uint32)  SPIS_GET_SPI_CTRL_SS(slaveSelect);

        SPIS_SPI_CTRL_REG = spiCtrl;
    }
#endif /* (SPIS_SPI_MASTER_CONST) */


#if !(SPIS_CY_SCBIP_V0 || SPIS_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: SPIS_SpiSetSlaveSelectPolarity
    ****************************************************************************//**
    *
    *  Sets active polarity for slave select line.
    *  The component should be in one of the following states to change the active
    *  slave select signal source correctly:
    *   - The component is disabled.
    *   - The component has completed transfer.
    *  
    *  This function does not check that these conditions are met.
    *
    *  \param slaveSelect: slave select line to change active polarity.
    *   - SPIS_SPI_SLAVE_SELECT0 - Slave select 0.
    *   - SPIS_SPI_SLAVE_SELECT1 - Slave select 1.
    *   - SPIS_SPI_SLAVE_SELECT2 - Slave select 2.
    *   - SPIS_SPI_SLAVE_SELECT3 - Slave select 3.
    *
    *  \param polarity: active polarity of slave select line.
    *   - SPIS_SPI_SS_ACTIVE_LOW  - Slave select is active low.
    *   - SPIS_SPI_SS_ACTIVE_HIGH - Slave select is active high.
    *
    *******************************************************************************/
    void SPIS_SpiSetSlaveSelectPolarity(uint32 slaveSelect, uint32 polarity)
    {
        uint32 ssPolarity;

        /* Get position of the polarity bit associated with slave select line */
        ssPolarity = SPIS_GET_SPI_CTRL_SSEL_POLARITY((uint32) 1u << slaveSelect);

        if (0u != polarity)
        {
            SPIS_SPI_CTRL_REG |= (uint32)  ssPolarity;
        }
        else
        {
            SPIS_SPI_CTRL_REG &= (uint32) ~ssPolarity;
        }
    }
#endif /* !(SPIS_CY_SCBIP_V0 || SPIS_CY_SCBIP_V1) */


#if(SPIS_SPI_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: SPIS_SpiSaveConfig
    ****************************************************************************//**
    *
    *  Clears INTR_SPI_EC.WAKE_UP and enables it. This interrupt
    *  source triggers when the master assigns the SS line and wakes up the device.
    *
    *******************************************************************************/
    void SPIS_SpiSaveConfig(void)
    {
        /* Clear and enable SPI wakeup interrupt source */
        SPIS_ClearSpiExtClkInterruptSource(SPIS_INTR_SPI_EC_WAKE_UP);
        SPIS_SetSpiExtClkInterruptMode(SPIS_INTR_SPI_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: SPIS_SpiRestoreConfig
    ****************************************************************************//**
    *
    *  Disables the INTR_SPI_EC.WAKE_UP interrupt source. After wakeup
    *  slave does not drive the MISO line and the master receives 0xFF.
    *
    *******************************************************************************/
    void SPIS_SpiRestoreConfig(void)
    {
        /* Disable SPI wakeup interrupt source */
        SPIS_SetSpiExtClkInterruptMode(SPIS_NO_INTR_SOURCES);
    }
#endif /* (SPIS_SPI_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
