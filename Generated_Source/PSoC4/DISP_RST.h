/*******************************************************************************
* File Name: DISP_RST.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_DISP_RST_H) /* Pins DISP_RST_H */
#define CY_PINS_DISP_RST_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DISP_RST_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} DISP_RST_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   DISP_RST_Read(void);
void    DISP_RST_Write(uint8 value);
uint8   DISP_RST_ReadDataReg(void);
#if defined(DISP_RST__PC) || (CY_PSOC4_4200L) 
    void    DISP_RST_SetDriveMode(uint8 mode);
#endif
void    DISP_RST_SetInterruptMode(uint16 position, uint16 mode);
uint8   DISP_RST_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void DISP_RST_Sleep(void); 
void DISP_RST_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(DISP_RST__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define DISP_RST_DRIVE_MODE_BITS        (3)
    #define DISP_RST_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DISP_RST_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the DISP_RST_SetDriveMode() function.
         *  @{
         */
        #define DISP_RST_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define DISP_RST_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define DISP_RST_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define DISP_RST_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define DISP_RST_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define DISP_RST_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define DISP_RST_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define DISP_RST_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define DISP_RST_MASK               DISP_RST__MASK
#define DISP_RST_SHIFT              DISP_RST__SHIFT
#define DISP_RST_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DISP_RST_SetInterruptMode() function.
     *  @{
     */
        #define DISP_RST_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define DISP_RST_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define DISP_RST_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define DISP_RST_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(DISP_RST__SIO)
    #define DISP_RST_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(DISP_RST__PC) && (CY_PSOC4_4200L)
    #define DISP_RST_USBIO_ENABLE               ((uint32)0x80000000u)
    #define DISP_RST_USBIO_DISABLE              ((uint32)(~DISP_RST_USBIO_ENABLE))
    #define DISP_RST_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define DISP_RST_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define DISP_RST_USBIO_ENTER_SLEEP          ((uint32)((1u << DISP_RST_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << DISP_RST_USBIO_SUSPEND_DEL_SHIFT)))
    #define DISP_RST_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << DISP_RST_USBIO_SUSPEND_SHIFT)))
    #define DISP_RST_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << DISP_RST_USBIO_SUSPEND_DEL_SHIFT)))
    #define DISP_RST_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(DISP_RST__PC)
    /* Port Configuration */
    #define DISP_RST_PC                 (* (reg32 *) DISP_RST__PC)
#endif
/* Pin State */
#define DISP_RST_PS                     (* (reg32 *) DISP_RST__PS)
/* Data Register */
#define DISP_RST_DR                     (* (reg32 *) DISP_RST__DR)
/* Input Buffer Disable Override */
#define DISP_RST_INP_DIS                (* (reg32 *) DISP_RST__PC2)

/* Interrupt configuration Registers */
#define DISP_RST_INTCFG                 (* (reg32 *) DISP_RST__INTCFG)
#define DISP_RST_INTSTAT                (* (reg32 *) DISP_RST__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define DISP_RST_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(DISP_RST__SIO)
    #define DISP_RST_SIO_REG            (* (reg32 *) DISP_RST__SIO)
#endif /* (DISP_RST__SIO_CFG) */

/* USBIO registers */
#if !defined(DISP_RST__PC) && (CY_PSOC4_4200L)
    #define DISP_RST_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define DISP_RST_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define DISP_RST_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define DISP_RST_DRIVE_MODE_SHIFT       (0x00u)
#define DISP_RST_DRIVE_MODE_MASK        (0x07u << DISP_RST_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins DISP_RST_H */


/* [] END OF FILE */
