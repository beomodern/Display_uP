/*******************************************************************************
* File Name: S_MOSI.h  
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

#if !defined(CY_PINS_S_MOSI_H) /* Pins S_MOSI_H */
#define CY_PINS_S_MOSI_H

#include "cytypes.h"
#include "cyfitter.h"
#include "S_MOSI_aliases.h"


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
} S_MOSI_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   S_MOSI_Read(void);
void    S_MOSI_Write(uint8 value);
uint8   S_MOSI_ReadDataReg(void);
#if defined(S_MOSI__PC) || (CY_PSOC4_4200L) 
    void    S_MOSI_SetDriveMode(uint8 mode);
#endif
void    S_MOSI_SetInterruptMode(uint16 position, uint16 mode);
uint8   S_MOSI_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void S_MOSI_Sleep(void); 
void S_MOSI_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(S_MOSI__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define S_MOSI_DRIVE_MODE_BITS        (3)
    #define S_MOSI_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - S_MOSI_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the S_MOSI_SetDriveMode() function.
         *  @{
         */
        #define S_MOSI_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define S_MOSI_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define S_MOSI_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define S_MOSI_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define S_MOSI_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define S_MOSI_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define S_MOSI_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define S_MOSI_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define S_MOSI_MASK               S_MOSI__MASK
#define S_MOSI_SHIFT              S_MOSI__SHIFT
#define S_MOSI_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in S_MOSI_SetInterruptMode() function.
     *  @{
     */
        #define S_MOSI_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define S_MOSI_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define S_MOSI_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define S_MOSI_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(S_MOSI__SIO)
    #define S_MOSI_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(S_MOSI__PC) && (CY_PSOC4_4200L)
    #define S_MOSI_USBIO_ENABLE               ((uint32)0x80000000u)
    #define S_MOSI_USBIO_DISABLE              ((uint32)(~S_MOSI_USBIO_ENABLE))
    #define S_MOSI_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define S_MOSI_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define S_MOSI_USBIO_ENTER_SLEEP          ((uint32)((1u << S_MOSI_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << S_MOSI_USBIO_SUSPEND_DEL_SHIFT)))
    #define S_MOSI_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << S_MOSI_USBIO_SUSPEND_SHIFT)))
    #define S_MOSI_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << S_MOSI_USBIO_SUSPEND_DEL_SHIFT)))
    #define S_MOSI_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(S_MOSI__PC)
    /* Port Configuration */
    #define S_MOSI_PC                 (* (reg32 *) S_MOSI__PC)
#endif
/* Pin State */
#define S_MOSI_PS                     (* (reg32 *) S_MOSI__PS)
/* Data Register */
#define S_MOSI_DR                     (* (reg32 *) S_MOSI__DR)
/* Input Buffer Disable Override */
#define S_MOSI_INP_DIS                (* (reg32 *) S_MOSI__PC2)

/* Interrupt configuration Registers */
#define S_MOSI_INTCFG                 (* (reg32 *) S_MOSI__INTCFG)
#define S_MOSI_INTSTAT                (* (reg32 *) S_MOSI__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define S_MOSI_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(S_MOSI__SIO)
    #define S_MOSI_SIO_REG            (* (reg32 *) S_MOSI__SIO)
#endif /* (S_MOSI__SIO_CFG) */

/* USBIO registers */
#if !defined(S_MOSI__PC) && (CY_PSOC4_4200L)
    #define S_MOSI_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define S_MOSI_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define S_MOSI_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define S_MOSI_DRIVE_MODE_SHIFT       (0x00u)
#define S_MOSI_DRIVE_MODE_MASK        (0x07u << S_MOSI_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins S_MOSI_H */


/* [] END OF FILE */
