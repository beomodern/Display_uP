;
; File Name: cyfitteriar.inc
; 
; PSoC Creator  4.3
;
; Description:
; 
;
;-------------------------------------------------------------------------------
; Copyright (c) 2007-2019 Cypress Semiconductor.  All rights reserved.
; You may use this file only in accordance with the license, terms, conditions, 
; disclaimers, and limitations in the end user license agreement accompanying 
; the software package with which this file was provided.
;-------------------------------------------------------------------------------

#ifndef INCLUDED_CYFITTERIAR_INC
#define INCLUDED_CYFITTERIAR_INC
    INCLUDE cydeviceiar_trm.inc

/* SS */
SS__0__DM__MASK EQU 0xE00
SS__0__DM__SHIFT EQU 9
SS__0__DR EQU CYREG_PRT3_DR
SS__0__HSIOM EQU CYREG_HSIOM_PORT_SEL3
SS__0__HSIOM_GPIO EQU 0
SS__0__HSIOM_MASK EQU 0x0000F000
SS__0__HSIOM_SHIFT EQU 12
SS__0__HSIOM_SPI EQU 15
SS__0__HSIOM_SPI_SSEL0 EQU 15
SS__0__INTCFG EQU CYREG_PRT3_INTCFG
SS__0__INTSTAT EQU CYREG_PRT3_INTSTAT
SS__0__MASK EQU 0x08
SS__0__PA__CFG0 EQU CYREG_UDB_PA3_CFG0
SS__0__PA__CFG1 EQU CYREG_UDB_PA3_CFG1
SS__0__PA__CFG10 EQU CYREG_UDB_PA3_CFG10
SS__0__PA__CFG11 EQU CYREG_UDB_PA3_CFG11
SS__0__PA__CFG12 EQU CYREG_UDB_PA3_CFG12
SS__0__PA__CFG13 EQU CYREG_UDB_PA3_CFG13
SS__0__PA__CFG14 EQU CYREG_UDB_PA3_CFG14
SS__0__PA__CFG2 EQU CYREG_UDB_PA3_CFG2
SS__0__PA__CFG3 EQU CYREG_UDB_PA3_CFG3
SS__0__PA__CFG4 EQU CYREG_UDB_PA3_CFG4
SS__0__PA__CFG5 EQU CYREG_UDB_PA3_CFG5
SS__0__PA__CFG6 EQU CYREG_UDB_PA3_CFG6
SS__0__PA__CFG7 EQU CYREG_UDB_PA3_CFG7
SS__0__PA__CFG8 EQU CYREG_UDB_PA3_CFG8
SS__0__PA__CFG9 EQU CYREG_UDB_PA3_CFG9
SS__0__PC EQU CYREG_PRT3_PC
SS__0__PC2 EQU CYREG_PRT3_PC2
SS__0__PORT EQU 3
SS__0__PS EQU CYREG_PRT3_PS
SS__0__SHIFT EQU 3
SS__DR EQU CYREG_PRT3_DR
SS__INTCFG EQU CYREG_PRT3_INTCFG
SS__INTSTAT EQU CYREG_PRT3_INTSTAT
SS__MASK EQU 0x08
SS__PA__CFG0 EQU CYREG_UDB_PA3_CFG0
SS__PA__CFG1 EQU CYREG_UDB_PA3_CFG1
SS__PA__CFG10 EQU CYREG_UDB_PA3_CFG10
SS__PA__CFG11 EQU CYREG_UDB_PA3_CFG11
SS__PA__CFG12 EQU CYREG_UDB_PA3_CFG12
SS__PA__CFG13 EQU CYREG_UDB_PA3_CFG13
SS__PA__CFG14 EQU CYREG_UDB_PA3_CFG14
SS__PA__CFG2 EQU CYREG_UDB_PA3_CFG2
SS__PA__CFG3 EQU CYREG_UDB_PA3_CFG3
SS__PA__CFG4 EQU CYREG_UDB_PA3_CFG4
SS__PA__CFG5 EQU CYREG_UDB_PA3_CFG5
SS__PA__CFG6 EQU CYREG_UDB_PA3_CFG6
SS__PA__CFG7 EQU CYREG_UDB_PA3_CFG7
SS__PA__CFG8 EQU CYREG_UDB_PA3_CFG8
SS__PA__CFG9 EQU CYREG_UDB_PA3_CFG9
SS__PC EQU CYREG_PRT3_PC
SS__PC2 EQU CYREG_PRT3_PC2
SS__PORT EQU 3
SS__PS EQU CYREG_PRT3_PS
SS__SHIFT EQU 3

/* MISO */
MISO__0__DM__MASK EQU 0x38
MISO__0__DM__SHIFT EQU 3
MISO__0__DR EQU CYREG_PRT3_DR
MISO__0__HSIOM EQU CYREG_HSIOM_PORT_SEL3
MISO__0__HSIOM_GPIO EQU 0
MISO__0__HSIOM_I2C EQU 14
MISO__0__HSIOM_I2C_SDA EQU 14
MISO__0__HSIOM_MASK EQU 0x000000F0
MISO__0__HSIOM_SHIFT EQU 4
MISO__0__HSIOM_SPI EQU 15
MISO__0__HSIOM_SPI_MISO EQU 15
MISO__0__HSIOM_UART EQU 9
MISO__0__HSIOM_UART_TX EQU 9
MISO__0__INTCFG EQU CYREG_PRT3_INTCFG
MISO__0__INTSTAT EQU CYREG_PRT3_INTSTAT
MISO__0__MASK EQU 0x02
MISO__0__OUT_SEL EQU CYREG_UDB_PA3_CFG10
MISO__0__OUT_SEL_SHIFT EQU 2
MISO__0__OUT_SEL_VAL EQU -1
MISO__0__PA__CFG0 EQU CYREG_UDB_PA3_CFG0
MISO__0__PA__CFG1 EQU CYREG_UDB_PA3_CFG1
MISO__0__PA__CFG10 EQU CYREG_UDB_PA3_CFG10
MISO__0__PA__CFG11 EQU CYREG_UDB_PA3_CFG11
MISO__0__PA__CFG12 EQU CYREG_UDB_PA3_CFG12
MISO__0__PA__CFG13 EQU CYREG_UDB_PA3_CFG13
MISO__0__PA__CFG14 EQU CYREG_UDB_PA3_CFG14
MISO__0__PA__CFG2 EQU CYREG_UDB_PA3_CFG2
MISO__0__PA__CFG3 EQU CYREG_UDB_PA3_CFG3
MISO__0__PA__CFG4 EQU CYREG_UDB_PA3_CFG4
MISO__0__PA__CFG5 EQU CYREG_UDB_PA3_CFG5
MISO__0__PA__CFG6 EQU CYREG_UDB_PA3_CFG6
MISO__0__PA__CFG7 EQU CYREG_UDB_PA3_CFG7
MISO__0__PA__CFG8 EQU CYREG_UDB_PA3_CFG8
MISO__0__PA__CFG9 EQU CYREG_UDB_PA3_CFG9
MISO__0__PC EQU CYREG_PRT3_PC
MISO__0__PC2 EQU CYREG_PRT3_PC2
MISO__0__PORT EQU 3
MISO__0__PS EQU CYREG_PRT3_PS
MISO__0__SHIFT EQU 1
MISO__DR EQU CYREG_PRT3_DR
MISO__INTCFG EQU CYREG_PRT3_INTCFG
MISO__INTSTAT EQU CYREG_PRT3_INTSTAT
MISO__MASK EQU 0x02
MISO__PA__CFG0 EQU CYREG_UDB_PA3_CFG0
MISO__PA__CFG1 EQU CYREG_UDB_PA3_CFG1
MISO__PA__CFG10 EQU CYREG_UDB_PA3_CFG10
MISO__PA__CFG11 EQU CYREG_UDB_PA3_CFG11
MISO__PA__CFG12 EQU CYREG_UDB_PA3_CFG12
MISO__PA__CFG13 EQU CYREG_UDB_PA3_CFG13
MISO__PA__CFG14 EQU CYREG_UDB_PA3_CFG14
MISO__PA__CFG2 EQU CYREG_UDB_PA3_CFG2
MISO__PA__CFG3 EQU CYREG_UDB_PA3_CFG3
MISO__PA__CFG4 EQU CYREG_UDB_PA3_CFG4
MISO__PA__CFG5 EQU CYREG_UDB_PA3_CFG5
MISO__PA__CFG6 EQU CYREG_UDB_PA3_CFG6
MISO__PA__CFG7 EQU CYREG_UDB_PA3_CFG7
MISO__PA__CFG8 EQU CYREG_UDB_PA3_CFG8
MISO__PA__CFG9 EQU CYREG_UDB_PA3_CFG9
MISO__PC EQU CYREG_PRT3_PC
MISO__PC2 EQU CYREG_PRT3_PC2
MISO__PORT EQU 3
MISO__PS EQU CYREG_PRT3_PS
MISO__SHIFT EQU 1

/* MOSI */
MOSI__0__DM__MASK EQU 0x07
MOSI__0__DM__SHIFT EQU 0
MOSI__0__DR EQU CYREG_PRT3_DR
MOSI__0__HSIOM EQU CYREG_HSIOM_PORT_SEL3
MOSI__0__HSIOM_GPIO EQU 0
MOSI__0__HSIOM_I2C EQU 14
MOSI__0__HSIOM_I2C_SCL EQU 14
MOSI__0__HSIOM_MASK EQU 0x0000000F
MOSI__0__HSIOM_SHIFT EQU 0
MOSI__0__HSIOM_SPI EQU 15
MOSI__0__HSIOM_SPI_MOSI EQU 15
MOSI__0__HSIOM_UART EQU 9
MOSI__0__HSIOM_UART_RX EQU 9
MOSI__0__INTCFG EQU CYREG_PRT3_INTCFG
MOSI__0__INTSTAT EQU CYREG_PRT3_INTSTAT
MOSI__0__MASK EQU 0x01
MOSI__0__PA__CFG0 EQU CYREG_UDB_PA3_CFG0
MOSI__0__PA__CFG1 EQU CYREG_UDB_PA3_CFG1
MOSI__0__PA__CFG10 EQU CYREG_UDB_PA3_CFG10
MOSI__0__PA__CFG11 EQU CYREG_UDB_PA3_CFG11
MOSI__0__PA__CFG12 EQU CYREG_UDB_PA3_CFG12
MOSI__0__PA__CFG13 EQU CYREG_UDB_PA3_CFG13
MOSI__0__PA__CFG14 EQU CYREG_UDB_PA3_CFG14
MOSI__0__PA__CFG2 EQU CYREG_UDB_PA3_CFG2
MOSI__0__PA__CFG3 EQU CYREG_UDB_PA3_CFG3
MOSI__0__PA__CFG4 EQU CYREG_UDB_PA3_CFG4
MOSI__0__PA__CFG5 EQU CYREG_UDB_PA3_CFG5
MOSI__0__PA__CFG6 EQU CYREG_UDB_PA3_CFG6
MOSI__0__PA__CFG7 EQU CYREG_UDB_PA3_CFG7
MOSI__0__PA__CFG8 EQU CYREG_UDB_PA3_CFG8
MOSI__0__PA__CFG9 EQU CYREG_UDB_PA3_CFG9
MOSI__0__PC EQU CYREG_PRT3_PC
MOSI__0__PC2 EQU CYREG_PRT3_PC2
MOSI__0__PORT EQU 3
MOSI__0__PS EQU CYREG_PRT3_PS
MOSI__0__SHIFT EQU 0
MOSI__DR EQU CYREG_PRT3_DR
MOSI__INTCFG EQU CYREG_PRT3_INTCFG
MOSI__INTSTAT EQU CYREG_PRT3_INTSTAT
MOSI__MASK EQU 0x01
MOSI__PA__CFG0 EQU CYREG_UDB_PA3_CFG0
MOSI__PA__CFG1 EQU CYREG_UDB_PA3_CFG1
MOSI__PA__CFG10 EQU CYREG_UDB_PA3_CFG10
MOSI__PA__CFG11 EQU CYREG_UDB_PA3_CFG11
MOSI__PA__CFG12 EQU CYREG_UDB_PA3_CFG12
MOSI__PA__CFG13 EQU CYREG_UDB_PA3_CFG13
MOSI__PA__CFG14 EQU CYREG_UDB_PA3_CFG14
MOSI__PA__CFG2 EQU CYREG_UDB_PA3_CFG2
MOSI__PA__CFG3 EQU CYREG_UDB_PA3_CFG3
MOSI__PA__CFG4 EQU CYREG_UDB_PA3_CFG4
MOSI__PA__CFG5 EQU CYREG_UDB_PA3_CFG5
MOSI__PA__CFG6 EQU CYREG_UDB_PA3_CFG6
MOSI__PA__CFG7 EQU CYREG_UDB_PA3_CFG7
MOSI__PA__CFG8 EQU CYREG_UDB_PA3_CFG8
MOSI__PA__CFG9 EQU CYREG_UDB_PA3_CFG9
MOSI__PC EQU CYREG_PRT3_PC
MOSI__PC2 EQU CYREG_PRT3_PC2
MOSI__PORT EQU 3
MOSI__PS EQU CYREG_PRT3_PS
MOSI__SHIFT EQU 0

/* SCLK */
SCLK__0__DM__MASK EQU 0x1C0
SCLK__0__DM__SHIFT EQU 6
SCLK__0__DR EQU CYREG_PRT3_DR
SCLK__0__HSIOM EQU CYREG_HSIOM_PORT_SEL3
SCLK__0__HSIOM_GPIO EQU 0
SCLK__0__HSIOM_MASK EQU 0x00000F00
SCLK__0__HSIOM_SHIFT EQU 8
SCLK__0__HSIOM_SPI EQU 15
SCLK__0__HSIOM_SPI_CLK EQU 15
SCLK__0__INTCFG EQU CYREG_PRT3_INTCFG
SCLK__0__INTSTAT EQU CYREG_PRT3_INTSTAT
SCLK__0__MASK EQU 0x04
SCLK__0__PA__CFG0 EQU CYREG_UDB_PA3_CFG0
SCLK__0__PA__CFG1 EQU CYREG_UDB_PA3_CFG1
SCLK__0__PA__CFG10 EQU CYREG_UDB_PA3_CFG10
SCLK__0__PA__CFG11 EQU CYREG_UDB_PA3_CFG11
SCLK__0__PA__CFG12 EQU CYREG_UDB_PA3_CFG12
SCLK__0__PA__CFG13 EQU CYREG_UDB_PA3_CFG13
SCLK__0__PA__CFG14 EQU CYREG_UDB_PA3_CFG14
SCLK__0__PA__CFG2 EQU CYREG_UDB_PA3_CFG2
SCLK__0__PA__CFG3 EQU CYREG_UDB_PA3_CFG3
SCLK__0__PA__CFG4 EQU CYREG_UDB_PA3_CFG4
SCLK__0__PA__CFG5 EQU CYREG_UDB_PA3_CFG5
SCLK__0__PA__CFG6 EQU CYREG_UDB_PA3_CFG6
SCLK__0__PA__CFG7 EQU CYREG_UDB_PA3_CFG7
SCLK__0__PA__CFG8 EQU CYREG_UDB_PA3_CFG8
SCLK__0__PA__CFG9 EQU CYREG_UDB_PA3_CFG9
SCLK__0__PC EQU CYREG_PRT3_PC
SCLK__0__PC2 EQU CYREG_PRT3_PC2
SCLK__0__PORT EQU 3
SCLK__0__PS EQU CYREG_PRT3_PS
SCLK__0__SHIFT EQU 2
SCLK__DR EQU CYREG_PRT3_DR
SCLK__INTCFG EQU CYREG_PRT3_INTCFG
SCLK__INTSTAT EQU CYREG_PRT3_INTSTAT
SCLK__MASK EQU 0x04
SCLK__PA__CFG0 EQU CYREG_UDB_PA3_CFG0
SCLK__PA__CFG1 EQU CYREG_UDB_PA3_CFG1
SCLK__PA__CFG10 EQU CYREG_UDB_PA3_CFG10
SCLK__PA__CFG11 EQU CYREG_UDB_PA3_CFG11
SCLK__PA__CFG12 EQU CYREG_UDB_PA3_CFG12
SCLK__PA__CFG13 EQU CYREG_UDB_PA3_CFG13
SCLK__PA__CFG14 EQU CYREG_UDB_PA3_CFG14
SCLK__PA__CFG2 EQU CYREG_UDB_PA3_CFG2
SCLK__PA__CFG3 EQU CYREG_UDB_PA3_CFG3
SCLK__PA__CFG4 EQU CYREG_UDB_PA3_CFG4
SCLK__PA__CFG5 EQU CYREG_UDB_PA3_CFG5
SCLK__PA__CFG6 EQU CYREG_UDB_PA3_CFG6
SCLK__PA__CFG7 EQU CYREG_UDB_PA3_CFG7
SCLK__PA__CFG8 EQU CYREG_UDB_PA3_CFG8
SCLK__PA__CFG9 EQU CYREG_UDB_PA3_CFG9
SCLK__PC EQU CYREG_PRT3_PC
SCLK__PC2 EQU CYREG_PRT3_PC2
SCLK__PORT EQU 3
SCLK__PS EQU CYREG_PRT3_PS
SCLK__SHIFT EQU 2

/* SPIS */
SPIS_SCB__BIST_CONTROL EQU CYREG_SCB1_BIST_CONTROL
SPIS_SCB__BIST_DATA EQU CYREG_SCB1_BIST_DATA
SPIS_SCB__CTRL EQU CYREG_SCB1_CTRL
SPIS_SCB__EZ_DATA00 EQU CYREG_SCB1_EZ_DATA00
SPIS_SCB__EZ_DATA01 EQU CYREG_SCB1_EZ_DATA01
SPIS_SCB__EZ_DATA02 EQU CYREG_SCB1_EZ_DATA02
SPIS_SCB__EZ_DATA03 EQU CYREG_SCB1_EZ_DATA03
SPIS_SCB__EZ_DATA04 EQU CYREG_SCB1_EZ_DATA04
SPIS_SCB__EZ_DATA05 EQU CYREG_SCB1_EZ_DATA05
SPIS_SCB__EZ_DATA06 EQU CYREG_SCB1_EZ_DATA06
SPIS_SCB__EZ_DATA07 EQU CYREG_SCB1_EZ_DATA07
SPIS_SCB__EZ_DATA08 EQU CYREG_SCB1_EZ_DATA08
SPIS_SCB__EZ_DATA09 EQU CYREG_SCB1_EZ_DATA09
SPIS_SCB__EZ_DATA10 EQU CYREG_SCB1_EZ_DATA10
SPIS_SCB__EZ_DATA11 EQU CYREG_SCB1_EZ_DATA11
SPIS_SCB__EZ_DATA12 EQU CYREG_SCB1_EZ_DATA12
SPIS_SCB__EZ_DATA13 EQU CYREG_SCB1_EZ_DATA13
SPIS_SCB__EZ_DATA14 EQU CYREG_SCB1_EZ_DATA14
SPIS_SCB__EZ_DATA15 EQU CYREG_SCB1_EZ_DATA15
SPIS_SCB__EZ_DATA16 EQU CYREG_SCB1_EZ_DATA16
SPIS_SCB__EZ_DATA17 EQU CYREG_SCB1_EZ_DATA17
SPIS_SCB__EZ_DATA18 EQU CYREG_SCB1_EZ_DATA18
SPIS_SCB__EZ_DATA19 EQU CYREG_SCB1_EZ_DATA19
SPIS_SCB__EZ_DATA20 EQU CYREG_SCB1_EZ_DATA20
SPIS_SCB__EZ_DATA21 EQU CYREG_SCB1_EZ_DATA21
SPIS_SCB__EZ_DATA22 EQU CYREG_SCB1_EZ_DATA22
SPIS_SCB__EZ_DATA23 EQU CYREG_SCB1_EZ_DATA23
SPIS_SCB__EZ_DATA24 EQU CYREG_SCB1_EZ_DATA24
SPIS_SCB__EZ_DATA25 EQU CYREG_SCB1_EZ_DATA25
SPIS_SCB__EZ_DATA26 EQU CYREG_SCB1_EZ_DATA26
SPIS_SCB__EZ_DATA27 EQU CYREG_SCB1_EZ_DATA27
SPIS_SCB__EZ_DATA28 EQU CYREG_SCB1_EZ_DATA28
SPIS_SCB__EZ_DATA29 EQU CYREG_SCB1_EZ_DATA29
SPIS_SCB__EZ_DATA30 EQU CYREG_SCB1_EZ_DATA30
SPIS_SCB__EZ_DATA31 EQU CYREG_SCB1_EZ_DATA31
SPIS_SCB__I2C_CFG EQU CYREG_SCB1_I2C_CFG
SPIS_SCB__I2C_CTRL EQU CYREG_SCB1_I2C_CTRL
SPIS_SCB__I2C_M_CMD EQU CYREG_SCB1_I2C_M_CMD
SPIS_SCB__I2C_S_CMD EQU CYREG_SCB1_I2C_S_CMD
SPIS_SCB__I2C_STATUS EQU CYREG_SCB1_I2C_STATUS
SPIS_SCB__INTR_CAUSE EQU CYREG_SCB1_INTR_CAUSE
SPIS_SCB__INTR_I2C_EC EQU CYREG_SCB1_INTR_I2C_EC
SPIS_SCB__INTR_I2C_EC_MASK EQU CYREG_SCB1_INTR_I2C_EC_MASK
SPIS_SCB__INTR_I2C_EC_MASKED EQU CYREG_SCB1_INTR_I2C_EC_MASKED
SPIS_SCB__INTR_M EQU CYREG_SCB1_INTR_M
SPIS_SCB__INTR_M_MASK EQU CYREG_SCB1_INTR_M_MASK
SPIS_SCB__INTR_M_MASKED EQU CYREG_SCB1_INTR_M_MASKED
SPIS_SCB__INTR_M_SET EQU CYREG_SCB1_INTR_M_SET
SPIS_SCB__INTR_RX EQU CYREG_SCB1_INTR_RX
SPIS_SCB__INTR_RX_MASK EQU CYREG_SCB1_INTR_RX_MASK
SPIS_SCB__INTR_RX_MASKED EQU CYREG_SCB1_INTR_RX_MASKED
SPIS_SCB__INTR_RX_SET EQU CYREG_SCB1_INTR_RX_SET
SPIS_SCB__INTR_S EQU CYREG_SCB1_INTR_S
SPIS_SCB__INTR_S_MASK EQU CYREG_SCB1_INTR_S_MASK
SPIS_SCB__INTR_S_MASKED EQU CYREG_SCB1_INTR_S_MASKED
SPIS_SCB__INTR_S_SET EQU CYREG_SCB1_INTR_S_SET
SPIS_SCB__INTR_SPI_EC EQU CYREG_SCB1_INTR_SPI_EC
SPIS_SCB__INTR_SPI_EC_MASK EQU CYREG_SCB1_INTR_SPI_EC_MASK
SPIS_SCB__INTR_SPI_EC_MASKED EQU CYREG_SCB1_INTR_SPI_EC_MASKED
SPIS_SCB__INTR_TX EQU CYREG_SCB1_INTR_TX
SPIS_SCB__INTR_TX_MASK EQU CYREG_SCB1_INTR_TX_MASK
SPIS_SCB__INTR_TX_MASKED EQU CYREG_SCB1_INTR_TX_MASKED
SPIS_SCB__INTR_TX_SET EQU CYREG_SCB1_INTR_TX_SET
SPIS_SCB__RX_CTRL EQU CYREG_SCB1_RX_CTRL
SPIS_SCB__RX_FIFO_CTRL EQU CYREG_SCB1_RX_FIFO_CTRL
SPIS_SCB__RX_FIFO_RD EQU CYREG_SCB1_RX_FIFO_RD
SPIS_SCB__RX_FIFO_RD_SILENT EQU CYREG_SCB1_RX_FIFO_RD_SILENT
SPIS_SCB__RX_FIFO_STATUS EQU CYREG_SCB1_RX_FIFO_STATUS
SPIS_SCB__RX_MATCH EQU CYREG_SCB1_RX_MATCH
SPIS_SCB__SPI_CTRL EQU CYREG_SCB1_SPI_CTRL
SPIS_SCB__SPI_STATUS EQU CYREG_SCB1_SPI_STATUS
SPIS_SCB__SS0_POSISTION EQU 0
SPIS_SCB__SS1_POSISTION EQU 1
SPIS_SCB__SS2_POSISTION EQU 2
SPIS_SCB__SS3_POSISTION EQU 3
SPIS_SCB__STATUS EQU CYREG_SCB1_STATUS
SPIS_SCB__TX_CTRL EQU CYREG_SCB1_TX_CTRL
SPIS_SCB__TX_FIFO_CTRL EQU CYREG_SCB1_TX_FIFO_CTRL
SPIS_SCB__TX_FIFO_STATUS EQU CYREG_SCB1_TX_FIFO_STATUS
SPIS_SCB__TX_FIFO_WR EQU CYREG_SCB1_TX_FIFO_WR
SPIS_SCB__UART_CTRL EQU CYREG_SCB1_UART_CTRL
SPIS_SCB__UART_RX_CTRL EQU CYREG_SCB1_UART_RX_CTRL
SPIS_SCB__UART_RX_STATUS EQU CYREG_SCB1_UART_RX_STATUS
SPIS_SCB__UART_TX_CTRL EQU CYREG_SCB1_UART_TX_CTRL
SPIS_SCB_IRQ__INTC_CLR_EN_REG EQU CYREG_CM0_ICER
SPIS_SCB_IRQ__INTC_CLR_PD_REG EQU CYREG_CM0_ICPR
SPIS_SCB_IRQ__INTC_MASK EQU 0x800
SPIS_SCB_IRQ__INTC_NUMBER EQU 11
SPIS_SCB_IRQ__INTC_PRIOR_MASK EQU 0xC0000000
SPIS_SCB_IRQ__INTC_PRIOR_NUM EQU 3
SPIS_SCB_IRQ__INTC_PRIOR_REG EQU CYREG_CM0_IPR2
SPIS_SCB_IRQ__INTC_SET_EN_REG EQU CYREG_CM0_ISER
SPIS_SCB_IRQ__INTC_SET_PD_REG EQU CYREG_CM0_ISPR
SPIS_SCBCLK__DIVIDER_MASK EQU 0x0000FFFF
SPIS_SCBCLK__ENABLE EQU CYREG_CLK_DIVIDER_A00
SPIS_SCBCLK__ENABLE_MASK EQU 0x80000000
SPIS_SCBCLK__MASK EQU 0x80000000
SPIS_SCBCLK__REGISTER EQU CYREG_CLK_DIVIDER_A00

/* UART */
UART_rx__0__DM__MASK EQU 0x07
UART_rx__0__DM__SHIFT EQU 0
UART_rx__0__DR EQU CYREG_PRT4_DR
UART_rx__0__HSIOM EQU CYREG_HSIOM_PORT_SEL4
UART_rx__0__HSIOM_GPIO EQU 0
UART_rx__0__HSIOM_I2C EQU 14
UART_rx__0__HSIOM_I2C_SCL EQU 14
UART_rx__0__HSIOM_MASK EQU 0x0000000F
UART_rx__0__HSIOM_SHIFT EQU 0
UART_rx__0__HSIOM_SPI EQU 15
UART_rx__0__HSIOM_SPI_MOSI EQU 15
UART_rx__0__HSIOM_UART EQU 9
UART_rx__0__HSIOM_UART_RX EQU 9
UART_rx__0__INTCFG EQU CYREG_PRT4_INTCFG
UART_rx__0__INTSTAT EQU CYREG_PRT4_INTSTAT
UART_rx__0__MASK EQU 0x01
UART_rx__0__PC EQU CYREG_PRT4_PC
UART_rx__0__PC2 EQU CYREG_PRT4_PC2
UART_rx__0__PORT EQU 4
UART_rx__0__PS EQU CYREG_PRT4_PS
UART_rx__0__SHIFT EQU 0
UART_rx__DR EQU CYREG_PRT4_DR
UART_rx__INTCFG EQU CYREG_PRT4_INTCFG
UART_rx__INTSTAT EQU CYREG_PRT4_INTSTAT
UART_rx__MASK EQU 0x01
UART_rx__PC EQU CYREG_PRT4_PC
UART_rx__PC2 EQU CYREG_PRT4_PC2
UART_rx__PORT EQU 4
UART_rx__PS EQU CYREG_PRT4_PS
UART_rx__SHIFT EQU 0
UART_SCB__BIST_CONTROL EQU CYREG_SCB0_BIST_CONTROL
UART_SCB__BIST_DATA EQU CYREG_SCB0_BIST_DATA
UART_SCB__CTRL EQU CYREG_SCB0_CTRL
UART_SCB__EZ_DATA00 EQU CYREG_SCB0_EZ_DATA00
UART_SCB__EZ_DATA01 EQU CYREG_SCB0_EZ_DATA01
UART_SCB__EZ_DATA02 EQU CYREG_SCB0_EZ_DATA02
UART_SCB__EZ_DATA03 EQU CYREG_SCB0_EZ_DATA03
UART_SCB__EZ_DATA04 EQU CYREG_SCB0_EZ_DATA04
UART_SCB__EZ_DATA05 EQU CYREG_SCB0_EZ_DATA05
UART_SCB__EZ_DATA06 EQU CYREG_SCB0_EZ_DATA06
UART_SCB__EZ_DATA07 EQU CYREG_SCB0_EZ_DATA07
UART_SCB__EZ_DATA08 EQU CYREG_SCB0_EZ_DATA08
UART_SCB__EZ_DATA09 EQU CYREG_SCB0_EZ_DATA09
UART_SCB__EZ_DATA10 EQU CYREG_SCB0_EZ_DATA10
UART_SCB__EZ_DATA11 EQU CYREG_SCB0_EZ_DATA11
UART_SCB__EZ_DATA12 EQU CYREG_SCB0_EZ_DATA12
UART_SCB__EZ_DATA13 EQU CYREG_SCB0_EZ_DATA13
UART_SCB__EZ_DATA14 EQU CYREG_SCB0_EZ_DATA14
UART_SCB__EZ_DATA15 EQU CYREG_SCB0_EZ_DATA15
UART_SCB__EZ_DATA16 EQU CYREG_SCB0_EZ_DATA16
UART_SCB__EZ_DATA17 EQU CYREG_SCB0_EZ_DATA17
UART_SCB__EZ_DATA18 EQU CYREG_SCB0_EZ_DATA18
UART_SCB__EZ_DATA19 EQU CYREG_SCB0_EZ_DATA19
UART_SCB__EZ_DATA20 EQU CYREG_SCB0_EZ_DATA20
UART_SCB__EZ_DATA21 EQU CYREG_SCB0_EZ_DATA21
UART_SCB__EZ_DATA22 EQU CYREG_SCB0_EZ_DATA22
UART_SCB__EZ_DATA23 EQU CYREG_SCB0_EZ_DATA23
UART_SCB__EZ_DATA24 EQU CYREG_SCB0_EZ_DATA24
UART_SCB__EZ_DATA25 EQU CYREG_SCB0_EZ_DATA25
UART_SCB__EZ_DATA26 EQU CYREG_SCB0_EZ_DATA26
UART_SCB__EZ_DATA27 EQU CYREG_SCB0_EZ_DATA27
UART_SCB__EZ_DATA28 EQU CYREG_SCB0_EZ_DATA28
UART_SCB__EZ_DATA29 EQU CYREG_SCB0_EZ_DATA29
UART_SCB__EZ_DATA30 EQU CYREG_SCB0_EZ_DATA30
UART_SCB__EZ_DATA31 EQU CYREG_SCB0_EZ_DATA31
UART_SCB__I2C_CFG EQU CYREG_SCB0_I2C_CFG
UART_SCB__I2C_CTRL EQU CYREG_SCB0_I2C_CTRL
UART_SCB__I2C_M_CMD EQU CYREG_SCB0_I2C_M_CMD
UART_SCB__I2C_S_CMD EQU CYREG_SCB0_I2C_S_CMD
UART_SCB__I2C_STATUS EQU CYREG_SCB0_I2C_STATUS
UART_SCB__INTR_CAUSE EQU CYREG_SCB0_INTR_CAUSE
UART_SCB__INTR_I2C_EC EQU CYREG_SCB0_INTR_I2C_EC
UART_SCB__INTR_I2C_EC_MASK EQU CYREG_SCB0_INTR_I2C_EC_MASK
UART_SCB__INTR_I2C_EC_MASKED EQU CYREG_SCB0_INTR_I2C_EC_MASKED
UART_SCB__INTR_M EQU CYREG_SCB0_INTR_M
UART_SCB__INTR_M_MASK EQU CYREG_SCB0_INTR_M_MASK
UART_SCB__INTR_M_MASKED EQU CYREG_SCB0_INTR_M_MASKED
UART_SCB__INTR_M_SET EQU CYREG_SCB0_INTR_M_SET
UART_SCB__INTR_RX EQU CYREG_SCB0_INTR_RX
UART_SCB__INTR_RX_MASK EQU CYREG_SCB0_INTR_RX_MASK
UART_SCB__INTR_RX_MASKED EQU CYREG_SCB0_INTR_RX_MASKED
UART_SCB__INTR_RX_SET EQU CYREG_SCB0_INTR_RX_SET
UART_SCB__INTR_S EQU CYREG_SCB0_INTR_S
UART_SCB__INTR_S_MASK EQU CYREG_SCB0_INTR_S_MASK
UART_SCB__INTR_S_MASKED EQU CYREG_SCB0_INTR_S_MASKED
UART_SCB__INTR_S_SET EQU CYREG_SCB0_INTR_S_SET
UART_SCB__INTR_SPI_EC EQU CYREG_SCB0_INTR_SPI_EC
UART_SCB__INTR_SPI_EC_MASK EQU CYREG_SCB0_INTR_SPI_EC_MASK
UART_SCB__INTR_SPI_EC_MASKED EQU CYREG_SCB0_INTR_SPI_EC_MASKED
UART_SCB__INTR_TX EQU CYREG_SCB0_INTR_TX
UART_SCB__INTR_TX_MASK EQU CYREG_SCB0_INTR_TX_MASK
UART_SCB__INTR_TX_MASKED EQU CYREG_SCB0_INTR_TX_MASKED
UART_SCB__INTR_TX_SET EQU CYREG_SCB0_INTR_TX_SET
UART_SCB__RX_CTRL EQU CYREG_SCB0_RX_CTRL
UART_SCB__RX_FIFO_CTRL EQU CYREG_SCB0_RX_FIFO_CTRL
UART_SCB__RX_FIFO_RD EQU CYREG_SCB0_RX_FIFO_RD
UART_SCB__RX_FIFO_RD_SILENT EQU CYREG_SCB0_RX_FIFO_RD_SILENT
UART_SCB__RX_FIFO_STATUS EQU CYREG_SCB0_RX_FIFO_STATUS
UART_SCB__RX_MATCH EQU CYREG_SCB0_RX_MATCH
UART_SCB__SPI_CTRL EQU CYREG_SCB0_SPI_CTRL
UART_SCB__SPI_STATUS EQU CYREG_SCB0_SPI_STATUS
UART_SCB__SS0_POSISTION EQU 0
UART_SCB__SS1_POSISTION EQU 1
UART_SCB__SS2_POSISTION EQU 2
UART_SCB__SS3_POSISTION EQU 3
UART_SCB__STATUS EQU CYREG_SCB0_STATUS
UART_SCB__TX_CTRL EQU CYREG_SCB0_TX_CTRL
UART_SCB__TX_FIFO_CTRL EQU CYREG_SCB0_TX_FIFO_CTRL
UART_SCB__TX_FIFO_STATUS EQU CYREG_SCB0_TX_FIFO_STATUS
UART_SCB__TX_FIFO_WR EQU CYREG_SCB0_TX_FIFO_WR
UART_SCB__UART_CTRL EQU CYREG_SCB0_UART_CTRL
UART_SCB__UART_RX_CTRL EQU CYREG_SCB0_UART_RX_CTRL
UART_SCB__UART_RX_STATUS EQU CYREG_SCB0_UART_RX_STATUS
UART_SCB__UART_TX_CTRL EQU CYREG_SCB0_UART_TX_CTRL
UART_SCBCLK__DIVIDER_MASK EQU 0x0000FFFF
UART_SCBCLK__ENABLE EQU CYREG_CLK_DIVIDER_B00
UART_SCBCLK__ENABLE_MASK EQU 0x80000000
UART_SCBCLK__MASK EQU 0x80000000
UART_SCBCLK__REGISTER EQU CYREG_CLK_DIVIDER_B00
UART_tx__0__DM__MASK EQU 0x38
UART_tx__0__DM__SHIFT EQU 3
UART_tx__0__DR EQU CYREG_PRT4_DR
UART_tx__0__HSIOM EQU CYREG_HSIOM_PORT_SEL4
UART_tx__0__HSIOM_GPIO EQU 0
UART_tx__0__HSIOM_I2C EQU 14
UART_tx__0__HSIOM_I2C_SDA EQU 14
UART_tx__0__HSIOM_MASK EQU 0x000000F0
UART_tx__0__HSIOM_SHIFT EQU 4
UART_tx__0__HSIOM_SPI EQU 15
UART_tx__0__HSIOM_SPI_MISO EQU 15
UART_tx__0__HSIOM_UART EQU 9
UART_tx__0__HSIOM_UART_TX EQU 9
UART_tx__0__INTCFG EQU CYREG_PRT4_INTCFG
UART_tx__0__INTSTAT EQU CYREG_PRT4_INTSTAT
UART_tx__0__MASK EQU 0x02
UART_tx__0__PC EQU CYREG_PRT4_PC
UART_tx__0__PC2 EQU CYREG_PRT4_PC2
UART_tx__0__PORT EQU 4
UART_tx__0__PS EQU CYREG_PRT4_PS
UART_tx__0__SHIFT EQU 1
UART_tx__DR EQU CYREG_PRT4_DR
UART_tx__INTCFG EQU CYREG_PRT4_INTCFG
UART_tx__INTSTAT EQU CYREG_PRT4_INTSTAT
UART_tx__MASK EQU 0x02
UART_tx__PC EQU CYREG_PRT4_PC
UART_tx__PC2 EQU CYREG_PRT4_PC2
UART_tx__PORT EQU 4
UART_tx__PS EQU CYREG_PRT4_PS
UART_tx__SHIFT EQU 1

/* SwClock */
SwClock__DIVIDER_MASK EQU 0x0000FFFF
SwClock__ENABLE EQU CYREG_CLK_DIVIDER_B01
SwClock__ENABLE_MASK EQU 0x80000000
SwClock__MASK EQU 0x80000000
SwClock__REGISTER EQU CYREG_CLK_DIVIDER_B01

/* PSoC_LED */
PSoC_LED__0__DM__MASK EQU 0x1C0000
PSoC_LED__0__DM__SHIFT EQU 18
PSoC_LED__0__DR EQU CYREG_PRT1_DR
PSoC_LED__0__HSIOM EQU CYREG_HSIOM_PORT_SEL1
PSoC_LED__0__HSIOM_MASK EQU 0x0F000000
PSoC_LED__0__HSIOM_SHIFT EQU 24
PSoC_LED__0__INTCFG EQU CYREG_PRT1_INTCFG
PSoC_LED__0__INTSTAT EQU CYREG_PRT1_INTSTAT
PSoC_LED__0__MASK EQU 0x40
PSoC_LED__0__PA__CFG0 EQU CYREG_UDB_PA1_CFG0
PSoC_LED__0__PA__CFG1 EQU CYREG_UDB_PA1_CFG1
PSoC_LED__0__PA__CFG10 EQU CYREG_UDB_PA1_CFG10
PSoC_LED__0__PA__CFG11 EQU CYREG_UDB_PA1_CFG11
PSoC_LED__0__PA__CFG12 EQU CYREG_UDB_PA1_CFG12
PSoC_LED__0__PA__CFG13 EQU CYREG_UDB_PA1_CFG13
PSoC_LED__0__PA__CFG14 EQU CYREG_UDB_PA1_CFG14
PSoC_LED__0__PA__CFG2 EQU CYREG_UDB_PA1_CFG2
PSoC_LED__0__PA__CFG3 EQU CYREG_UDB_PA1_CFG3
PSoC_LED__0__PA__CFG4 EQU CYREG_UDB_PA1_CFG4
PSoC_LED__0__PA__CFG5 EQU CYREG_UDB_PA1_CFG5
PSoC_LED__0__PA__CFG6 EQU CYREG_UDB_PA1_CFG6
PSoC_LED__0__PA__CFG7 EQU CYREG_UDB_PA1_CFG7
PSoC_LED__0__PA__CFG8 EQU CYREG_UDB_PA1_CFG8
PSoC_LED__0__PA__CFG9 EQU CYREG_UDB_PA1_CFG9
PSoC_LED__0__PC EQU CYREG_PRT1_PC
PSoC_LED__0__PC2 EQU CYREG_PRT1_PC2
PSoC_LED__0__PORT EQU 1
PSoC_LED__0__PS EQU CYREG_PRT1_PS
PSoC_LED__0__SHIFT EQU 6
PSoC_LED__DR EQU CYREG_PRT1_DR
PSoC_LED__INTCFG EQU CYREG_PRT1_INTCFG
PSoC_LED__INTSTAT EQU CYREG_PRT1_INTSTAT
PSoC_LED__MASK EQU 0x40
PSoC_LED__PA__CFG0 EQU CYREG_UDB_PA1_CFG0
PSoC_LED__PA__CFG1 EQU CYREG_UDB_PA1_CFG1
PSoC_LED__PA__CFG10 EQU CYREG_UDB_PA1_CFG10
PSoC_LED__PA__CFG11 EQU CYREG_UDB_PA1_CFG11
PSoC_LED__PA__CFG12 EQU CYREG_UDB_PA1_CFG12
PSoC_LED__PA__CFG13 EQU CYREG_UDB_PA1_CFG13
PSoC_LED__PA__CFG14 EQU CYREG_UDB_PA1_CFG14
PSoC_LED__PA__CFG2 EQU CYREG_UDB_PA1_CFG2
PSoC_LED__PA__CFG3 EQU CYREG_UDB_PA1_CFG3
PSoC_LED__PA__CFG4 EQU CYREG_UDB_PA1_CFG4
PSoC_LED__PA__CFG5 EQU CYREG_UDB_PA1_CFG5
PSoC_LED__PA__CFG6 EQU CYREG_UDB_PA1_CFG6
PSoC_LED__PA__CFG7 EQU CYREG_UDB_PA1_CFG7
PSoC_LED__PA__CFG8 EQU CYREG_UDB_PA1_CFG8
PSoC_LED__PA__CFG9 EQU CYREG_UDB_PA1_CFG9
PSoC_LED__PC EQU CYREG_PRT1_PC
PSoC_LED__PC2 EQU CYREG_PRT1_PC2
PSoC_LED__PORT EQU 1
PSoC_LED__PS EQU CYREG_PRT1_PS
PSoC_LED__SHIFT EQU 6

/* Push_button */
Push_button__0__DM__MASK EQU 0xE00000
Push_button__0__DM__SHIFT EQU 21
Push_button__0__DR EQU CYREG_PRT0_DR
Push_button__0__HSIOM EQU CYREG_HSIOM_PORT_SEL0
Push_button__0__HSIOM_MASK EQU 0xF0000000
Push_button__0__HSIOM_SHIFT EQU 28
Push_button__0__INTCFG EQU CYREG_PRT0_INTCFG
Push_button__0__INTSTAT EQU CYREG_PRT0_INTSTAT
Push_button__0__MASK EQU 0x80
Push_button__0__PA__CFG0 EQU CYREG_UDB_PA0_CFG0
Push_button__0__PA__CFG1 EQU CYREG_UDB_PA0_CFG1
Push_button__0__PA__CFG10 EQU CYREG_UDB_PA0_CFG10
Push_button__0__PA__CFG11 EQU CYREG_UDB_PA0_CFG11
Push_button__0__PA__CFG12 EQU CYREG_UDB_PA0_CFG12
Push_button__0__PA__CFG13 EQU CYREG_UDB_PA0_CFG13
Push_button__0__PA__CFG14 EQU CYREG_UDB_PA0_CFG14
Push_button__0__PA__CFG2 EQU CYREG_UDB_PA0_CFG2
Push_button__0__PA__CFG3 EQU CYREG_UDB_PA0_CFG3
Push_button__0__PA__CFG4 EQU CYREG_UDB_PA0_CFG4
Push_button__0__PA__CFG5 EQU CYREG_UDB_PA0_CFG5
Push_button__0__PA__CFG6 EQU CYREG_UDB_PA0_CFG6
Push_button__0__PA__CFG7 EQU CYREG_UDB_PA0_CFG7
Push_button__0__PA__CFG8 EQU CYREG_UDB_PA0_CFG8
Push_button__0__PA__CFG9 EQU CYREG_UDB_PA0_CFG9
Push_button__0__PC EQU CYREG_PRT0_PC
Push_button__0__PC2 EQU CYREG_PRT0_PC2
Push_button__0__PORT EQU 0
Push_button__0__PS EQU CYREG_PRT0_PS
Push_button__0__SHIFT EQU 7
Push_button__DR EQU CYREG_PRT0_DR
Push_button__INTCFG EQU CYREG_PRT0_INTCFG
Push_button__INTSTAT EQU CYREG_PRT0_INTSTAT
Push_button__MASK EQU 0x80
Push_button__PA__CFG0 EQU CYREG_UDB_PA0_CFG0
Push_button__PA__CFG1 EQU CYREG_UDB_PA0_CFG1
Push_button__PA__CFG10 EQU CYREG_UDB_PA0_CFG10
Push_button__PA__CFG11 EQU CYREG_UDB_PA0_CFG11
Push_button__PA__CFG12 EQU CYREG_UDB_PA0_CFG12
Push_button__PA__CFG13 EQU CYREG_UDB_PA0_CFG13
Push_button__PA__CFG14 EQU CYREG_UDB_PA0_CFG14
Push_button__PA__CFG2 EQU CYREG_UDB_PA0_CFG2
Push_button__PA__CFG3 EQU CYREG_UDB_PA0_CFG3
Push_button__PA__CFG4 EQU CYREG_UDB_PA0_CFG4
Push_button__PA__CFG5 EQU CYREG_UDB_PA0_CFG5
Push_button__PA__CFG6 EQU CYREG_UDB_PA0_CFG6
Push_button__PA__CFG7 EQU CYREG_UDB_PA0_CFG7
Push_button__PA__CFG8 EQU CYREG_UDB_PA0_CFG8
Push_button__PA__CFG9 EQU CYREG_UDB_PA0_CFG9
Push_button__PC EQU CYREG_PRT0_PC
Push_button__PC2 EQU CYREG_PRT0_PC2
Push_button__PORT EQU 0
Push_button__PS EQU CYREG_PRT0_PS
Push_button__SHIFT EQU 7

/* DISPLAY_Timer */
DISPLAY_Timer_INT__INTC_CLR_EN_REG EQU CYREG_CM0_ICER
DISPLAY_Timer_INT__INTC_CLR_PD_REG EQU CYREG_CM0_ICPR
DISPLAY_Timer_INT__INTC_MASK EQU 0x01
DISPLAY_Timer_INT__INTC_NUMBER EQU 0
DISPLAY_Timer_INT__INTC_PRIOR_MASK EQU 0xC0
DISPLAY_Timer_INT__INTC_PRIOR_NUM EQU 3
DISPLAY_Timer_INT__INTC_PRIOR_REG EQU CYREG_CM0_IPR0
DISPLAY_Timer_INT__INTC_SET_EN_REG EQU CYREG_CM0_ISER
DISPLAY_Timer_INT__INTC_SET_PD_REG EQU CYREG_CM0_ISPR
DISPLAY_Timer_TimerUDB_rstSts_stsreg__0__MASK EQU 0x01
DISPLAY_Timer_TimerUDB_rstSts_stsreg__0__POS EQU 0
DISPLAY_Timer_TimerUDB_rstSts_stsreg__2__MASK EQU 0x04
DISPLAY_Timer_TimerUDB_rstSts_stsreg__2__POS EQU 2
DISPLAY_Timer_TimerUDB_rstSts_stsreg__3__MASK EQU 0x08
DISPLAY_Timer_TimerUDB_rstSts_stsreg__3__POS EQU 3
DISPLAY_Timer_TimerUDB_rstSts_stsreg__MASK EQU 0x0D
DISPLAY_Timer_TimerUDB_rstSts_stsreg__MASK_REG EQU CYREG_UDB_W8_MSK_03
DISPLAY_Timer_TimerUDB_rstSts_stsreg__MASK_ST_AUX_CTL_REG EQU CYREG_UDB_CAT16_ACTL_MSK_03
DISPLAY_Timer_TimerUDB_rstSts_stsreg__PER_ST_AUX_CTL_REG EQU CYREG_UDB_CAT16_ACTL_MSK_03
DISPLAY_Timer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG EQU CYREG_UDB_W8_ACTL_03
DISPLAY_Timer_TimerUDB_rstSts_stsreg__STATUS_CNT_REG EQU CYREG_UDB_CAT16_CTL_ST_03
DISPLAY_Timer_TimerUDB_rstSts_stsreg__STATUS_CONTROL_REG EQU CYREG_UDB_CAT16_CTL_ST_03
DISPLAY_Timer_TimerUDB_rstSts_stsreg__STATUS_REG EQU CYREG_UDB_W8_ST_03
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__7__MASK EQU 0x80
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__7__POS EQU 7
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_AUX_CTL_REG EQU CYREG_UDB_W8_ACTL_03
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG EQU CYREG_UDB_W8_CTL_03
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_ST_REG EQU CYREG_UDB_CAT16_CTL_ST_03
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__COUNT_REG EQU CYREG_UDB_W8_CTL_03
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__COUNT_ST_REG EQU CYREG_UDB_CAT16_CTL_ST_03
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__MASK EQU 0x80
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__MASK_CTL_AUX_CTL_REG EQU CYREG_UDB_CAT16_ACTL_MSK_03
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__PER_CTL_AUX_CTL_REG EQU CYREG_UDB_CAT16_ACTL_MSK_03
DISPLAY_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__PERIOD_REG EQU CYREG_UDB_W8_MSK_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG EQU CYREG_UDB_W16_A0_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__16BIT_A1_REG EQU CYREG_UDB_W16_A1_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG EQU CYREG_UDB_W16_D0_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__16BIT_D1_REG EQU CYREG_UDB_W16_D1_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__16BIT_DP_AUX_CTL_REG EQU CYREG_UDB_W16_ACTL_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG EQU CYREG_UDB_W16_F0_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__16BIT_F1_REG EQU CYREG_UDB_W16_F1_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__A0_A1_REG EQU CYREG_UDB_CAT16_A_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__A0_REG EQU CYREG_UDB_W8_A0_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__A1_REG EQU CYREG_UDB_W8_A1_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__D0_D1_REG EQU CYREG_UDB_CAT16_D_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__D0_REG EQU CYREG_UDB_W8_D0_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__D1_REG EQU CYREG_UDB_W8_D1_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__DP_AUX_CTL_REG EQU CYREG_UDB_W8_ACTL_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__F0_F1_REG EQU CYREG_UDB_CAT16_F_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__F0_REG EQU CYREG_UDB_W8_F0_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u0__F1_REG EQU CYREG_UDB_W8_F1_02
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__A0_A1_REG EQU CYREG_UDB_CAT16_A_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__A0_REG EQU CYREG_UDB_W8_A0_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__A1_REG EQU CYREG_UDB_W8_A1_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__D0_D1_REG EQU CYREG_UDB_CAT16_D_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__D0_REG EQU CYREG_UDB_W8_D0_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__D1_REG EQU CYREG_UDB_W8_D1_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__DP_AUX_CTL_REG EQU CYREG_UDB_W8_ACTL_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__F0_F1_REG EQU CYREG_UDB_CAT16_F_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__F0_REG EQU CYREG_UDB_W8_F0_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__F1_REG EQU CYREG_UDB_W8_F1_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__MSK_DP_AUX_CTL_REG EQU CYREG_UDB_CAT16_ACTL_MSK_03
DISPLAY_Timer_TimerUDB_sT16_timerdp_u1__PER_DP_AUX_CTL_REG EQU CYREG_UDB_CAT16_ACTL_MSK_03

/* Miscellaneous */
CYDEV_BCLK__HFCLK__HZ EQU 24000000
CYDEV_BCLK__HFCLK__KHZ EQU 24000
CYDEV_BCLK__HFCLK__MHZ EQU 24
CYDEV_BCLK__SYSCLK__HZ EQU 24000000
CYDEV_BCLK__SYSCLK__KHZ EQU 24000
CYDEV_BCLK__SYSCLK__MHZ EQU 24
CYDEV_CHIP_DIE_LEOPARD EQU 1
CYDEV_CHIP_DIE_PSOC4A EQU 24
CYDEV_CHIP_DIE_PSOC5LP EQU 2
CYDEV_CHIP_DIE_PSOC5TM EQU 3
CYDEV_CHIP_DIE_TMA4 EQU 4
CYDEV_CHIP_DIE_UNKNOWN EQU 0
CYDEV_CHIP_FAMILY_FM0P EQU 5
CYDEV_CHIP_FAMILY_FM3 EQU 6
CYDEV_CHIP_FAMILY_FM4 EQU 7
CYDEV_CHIP_FAMILY_PSOC3 EQU 1
CYDEV_CHIP_FAMILY_PSOC4 EQU 2
CYDEV_CHIP_FAMILY_PSOC5 EQU 3
CYDEV_CHIP_FAMILY_PSOC6 EQU 4
CYDEV_CHIP_FAMILY_UNKNOWN EQU 0
CYDEV_CHIP_FAMILY_USED EQU CYDEV_CHIP_FAMILY_PSOC4
CYDEV_CHIP_JTAG_ID EQU 0x04C81193
CYDEV_CHIP_MEMBER_3A EQU 1
CYDEV_CHIP_MEMBER_4A EQU 24
CYDEV_CHIP_MEMBER_4AA EQU 23
CYDEV_CHIP_MEMBER_4AB EQU 28
CYDEV_CHIP_MEMBER_4AC EQU 14
CYDEV_CHIP_MEMBER_4D EQU 18
CYDEV_CHIP_MEMBER_4E EQU 6
CYDEV_CHIP_MEMBER_4F EQU 25
CYDEV_CHIP_MEMBER_4G EQU 4
CYDEV_CHIP_MEMBER_4H EQU 22
CYDEV_CHIP_MEMBER_4I EQU 30
CYDEV_CHIP_MEMBER_4J EQU 19
CYDEV_CHIP_MEMBER_4K EQU 20
CYDEV_CHIP_MEMBER_4L EQU 29
CYDEV_CHIP_MEMBER_4M EQU 27
CYDEV_CHIP_MEMBER_4N EQU 11
CYDEV_CHIP_MEMBER_4O EQU 8
CYDEV_CHIP_MEMBER_4P EQU 26
CYDEV_CHIP_MEMBER_4Q EQU 15
CYDEV_CHIP_MEMBER_4R EQU 9
CYDEV_CHIP_MEMBER_4S EQU 12
CYDEV_CHIP_MEMBER_4T EQU 10
CYDEV_CHIP_MEMBER_4U EQU 5
CYDEV_CHIP_MEMBER_4V EQU 21
CYDEV_CHIP_MEMBER_4W EQU 13
CYDEV_CHIP_MEMBER_4X EQU 7
CYDEV_CHIP_MEMBER_4Y EQU 16
CYDEV_CHIP_MEMBER_4Z EQU 17
CYDEV_CHIP_MEMBER_5A EQU 3
CYDEV_CHIP_MEMBER_5B EQU 2
CYDEV_CHIP_MEMBER_6A EQU 31
CYDEV_CHIP_MEMBER_FM3 EQU 35
CYDEV_CHIP_MEMBER_FM4 EQU 36
CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE1 EQU 32
CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE2 EQU 33
CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE3 EQU 34
CYDEV_CHIP_MEMBER_UNKNOWN EQU 0
CYDEV_CHIP_MEMBER_USED EQU CYDEV_CHIP_MEMBER_4A
CYDEV_CHIP_DIE_EXPECT EQU CYDEV_CHIP_MEMBER_USED
CYDEV_CHIP_DIE_ACTUAL EQU CYDEV_CHIP_DIE_EXPECT
CYDEV_CHIP_REV_LEOPARD_ES1 EQU 0
CYDEV_CHIP_REV_LEOPARD_ES2 EQU 1
CYDEV_CHIP_REV_LEOPARD_ES3 EQU 3
CYDEV_CHIP_REV_LEOPARD_PRODUCTION EQU 3
CYDEV_CHIP_REV_PSOC4A_ES0 EQU 17
CYDEV_CHIP_REV_PSOC4A_PRODUCTION EQU 17
CYDEV_CHIP_REV_PSOC5LP_ES0 EQU 0
CYDEV_CHIP_REV_PSOC5LP_PRODUCTION EQU 0
CYDEV_CHIP_REV_PSOC5TM_ES0 EQU 0
CYDEV_CHIP_REV_PSOC5TM_ES1 EQU 1
CYDEV_CHIP_REV_PSOC5TM_PRODUCTION EQU 1
CYDEV_CHIP_REV_TMA4_ES EQU 17
CYDEV_CHIP_REV_TMA4_ES2 EQU 33
CYDEV_CHIP_REV_TMA4_PRODUCTION EQU 17
CYDEV_CHIP_REVISION_3A_ES1 EQU 0
CYDEV_CHIP_REVISION_3A_ES2 EQU 1
CYDEV_CHIP_REVISION_3A_ES3 EQU 3
CYDEV_CHIP_REVISION_3A_PRODUCTION EQU 3
CYDEV_CHIP_REVISION_4A_ES0 EQU 17
CYDEV_CHIP_REVISION_4A_PRODUCTION EQU 17
CYDEV_CHIP_REVISION_4AA_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4AB_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4AC_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4D_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4E_CCG2_NO_USBPD EQU 0
CYDEV_CHIP_REVISION_4E_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4F_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4F_PRODUCTION_256DMA EQU 0
CYDEV_CHIP_REVISION_4F_PRODUCTION_256K EQU 0
CYDEV_CHIP_REVISION_4G_ES EQU 17
CYDEV_CHIP_REVISION_4G_ES2 EQU 33
CYDEV_CHIP_REVISION_4G_PRODUCTION EQU 17
CYDEV_CHIP_REVISION_4H_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4I_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4J_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4K_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4L_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4M_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4N_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4O_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4P_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4Q_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4R_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4S_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4T_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4U_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4V_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4W_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4X_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4Y_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4Z_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_5A_ES0 EQU 0
CYDEV_CHIP_REVISION_5A_ES1 EQU 1
CYDEV_CHIP_REVISION_5A_PRODUCTION EQU 1
CYDEV_CHIP_REVISION_5B_ES0 EQU 0
CYDEV_CHIP_REVISION_5B_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_6A_ES EQU 17
CYDEV_CHIP_REVISION_6A_NO_UDB EQU 33
CYDEV_CHIP_REVISION_6A_PRODUCTION EQU 33
CYDEV_CHIP_REVISION_FM3_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_FM4_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_PDL_FM0P_TYPE1_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_PDL_FM0P_TYPE2_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_PDL_FM0P_TYPE3_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_USED EQU CYDEV_CHIP_REVISION_4A_PRODUCTION
CYDEV_CHIP_REV_EXPECT EQU CYDEV_CHIP_REVISION_USED
CYDEV_CONFIG_READ_ACCELERATOR EQU 1
CYDEV_CONFIG_UNUSED_IO_AllowButWarn EQU 0
CYDEV_CONFIG_UNUSED_IO EQU CYDEV_CONFIG_UNUSED_IO_AllowButWarn
CYDEV_CONFIG_UNUSED_IO_AllowWithInfo EQU 1
CYDEV_CONFIG_UNUSED_IO_Disallowed EQU 2
CYDEV_CONFIGURATION_COMPRESSED EQU 1
CYDEV_CONFIGURATION_MODE_COMPRESSED EQU 0
CYDEV_CONFIGURATION_MODE EQU CYDEV_CONFIGURATION_MODE_COMPRESSED
CYDEV_CONFIGURATION_MODE_DMA EQU 2
CYDEV_CONFIGURATION_MODE_UNCOMPRESSED EQU 1
CYDEV_DEBUG_PROTECT_KILL EQU 4
CYDEV_DEBUG_PROTECT_OPEN EQU 1
CYDEV_DEBUG_PROTECT EQU CYDEV_DEBUG_PROTECT_OPEN
CYDEV_DEBUG_PROTECT_PROTECTED EQU 2
CYDEV_DEBUGGING_DPS_Disable EQU 3
CYDEV_DEBUGGING_DPS EQU CYDEV_DEBUGGING_DPS_Disable
CYDEV_DEBUGGING_DPS_SWD EQU 2
CYDEV_DEBUGGING_ENABLE EQU 0
CYDEV_DFT_SELECT_CLK0 EQU 1
CYDEV_DFT_SELECT_CLK1 EQU 2
CYDEV_HEAP_SIZE EQU 0x0100
CYDEV_IMO_TRIMMED_BY_USB EQU 0
CYDEV_IMO_TRIMMED_BY_WCO EQU 0
CYDEV_IS_EXPORTING_CODE EQU 0
CYDEV_IS_IMPORTING_CODE EQU 0
CYDEV_PROJ_TYPE EQU 2
CYDEV_PROJ_TYPE_BOOTLOADER EQU 1
CYDEV_PROJ_TYPE_LAUNCHER EQU 5
CYDEV_PROJ_TYPE_LOADABLE EQU 2
CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER EQU 4
CYDEV_PROJ_TYPE_MULTIAPPBOOTLOADER EQU 3
CYDEV_PROJ_TYPE_STANDARD EQU 0
CYDEV_STACK_SIZE EQU 0x0400
CYDEV_USE_BUNDLED_CMSIS EQU 1
CYDEV_VARIABLE_VDDA EQU 1
CYDEV_VDDA_MV EQU 5000
CYDEV_VDDD_MV EQU 5000
CYDEV_WDT_GENERATE_ISR EQU 0
CYIPBLOCK_M0S8_CTBM_VERSION EQU 0
CYIPBLOCK_m0s8cpuss_VERSION EQU 0
CYIPBLOCK_m0s8csd_VERSION EQU 0
CYIPBLOCK_m0s8gpio2_VERSION EQU 0
CYIPBLOCK_m0s8hsiom4a_VERSION EQU 0
CYIPBLOCK_m0s8lcd_VERSION EQU 0
CYIPBLOCK_m0s8lpcomp_VERSION EQU 0
CYIPBLOCK_m0s8pclk_VERSION EQU 0
CYIPBLOCK_m0s8sar_VERSION EQU 0
CYIPBLOCK_m0s8scb_VERSION EQU 0
CYIPBLOCK_m0s8srssv2_VERSION EQU 1
CYIPBLOCK_m0s8tcpwm_VERSION EQU 0
CYIPBLOCK_m0s8udbif_VERSION EQU 0
CYIPBLOCK_S8_GPIO_VERSION EQU 2
CYDEV_BOOTLOADER_ENABLE EQU 0

#endif /* INCLUDED_CYFITTERIAR_INC */