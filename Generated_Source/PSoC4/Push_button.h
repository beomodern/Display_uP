/*******************************************************************************
* File Name: Push_button.h  
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

#if !defined(CY_PINS_Push_button_H) /* Pins Push_button_H */
#define CY_PINS_Push_button_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Push_button_aliases.h"


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
} Push_button_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Push_button_Read(void);
void    Push_button_Write(uint8 value);
uint8   Push_button_ReadDataReg(void);
#if defined(Push_button__PC) || (CY_PSOC4_4200L) 
    void    Push_button_SetDriveMode(uint8 mode);
#endif
void    Push_button_SetInterruptMode(uint16 position, uint16 mode);
uint8   Push_button_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Push_button_Sleep(void); 
void Push_button_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Push_button__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Push_button_DRIVE_MODE_BITS        (3)
    #define Push_button_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Push_button_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Push_button_SetDriveMode() function.
         *  @{
         */
        #define Push_button_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Push_button_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Push_button_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Push_button_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Push_button_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Push_button_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Push_button_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Push_button_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Push_button_MASK               Push_button__MASK
#define Push_button_SHIFT              Push_button__SHIFT
#define Push_button_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Push_button_SetInterruptMode() function.
     *  @{
     */
        #define Push_button_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Push_button_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Push_button_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Push_button_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Push_button__SIO)
    #define Push_button_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Push_button__PC) && (CY_PSOC4_4200L)
    #define Push_button_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Push_button_USBIO_DISABLE              ((uint32)(~Push_button_USBIO_ENABLE))
    #define Push_button_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Push_button_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Push_button_USBIO_ENTER_SLEEP          ((uint32)((1u << Push_button_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Push_button_USBIO_SUSPEND_DEL_SHIFT)))
    #define Push_button_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Push_button_USBIO_SUSPEND_SHIFT)))
    #define Push_button_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Push_button_USBIO_SUSPEND_DEL_SHIFT)))
    #define Push_button_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Push_button__PC)
    /* Port Configuration */
    #define Push_button_PC                 (* (reg32 *) Push_button__PC)
#endif
/* Pin State */
#define Push_button_PS                     (* (reg32 *) Push_button__PS)
/* Data Register */
#define Push_button_DR                     (* (reg32 *) Push_button__DR)
/* Input Buffer Disable Override */
#define Push_button_INP_DIS                (* (reg32 *) Push_button__PC2)

/* Interrupt configuration Registers */
#define Push_button_INTCFG                 (* (reg32 *) Push_button__INTCFG)
#define Push_button_INTSTAT                (* (reg32 *) Push_button__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Push_button_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Push_button__SIO)
    #define Push_button_SIO_REG            (* (reg32 *) Push_button__SIO)
#endif /* (Push_button__SIO_CFG) */

/* USBIO registers */
#if !defined(Push_button__PC) && (CY_PSOC4_4200L)
    #define Push_button_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Push_button_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Push_button_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Push_button_DRIVE_MODE_SHIFT       (0x00u)
#define Push_button_DRIVE_MODE_MASK        (0x07u << Push_button_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Push_button_H */


/* [] END OF FILE */
