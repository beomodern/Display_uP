/*******************************************************************************
* File Name: DCF_PDN.h  
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

#if !defined(CY_PINS_DCF_PDN_H) /* Pins DCF_PDN_H */
#define CY_PINS_DCF_PDN_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DCF_PDN_aliases.h"


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
} DCF_PDN_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   DCF_PDN_Read(void);
void    DCF_PDN_Write(uint8 value);
uint8   DCF_PDN_ReadDataReg(void);
#if defined(DCF_PDN__PC) || (CY_PSOC4_4200L) 
    void    DCF_PDN_SetDriveMode(uint8 mode);
#endif
void    DCF_PDN_SetInterruptMode(uint16 position, uint16 mode);
uint8   DCF_PDN_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void DCF_PDN_Sleep(void); 
void DCF_PDN_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(DCF_PDN__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define DCF_PDN_DRIVE_MODE_BITS        (3)
    #define DCF_PDN_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DCF_PDN_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the DCF_PDN_SetDriveMode() function.
         *  @{
         */
        #define DCF_PDN_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define DCF_PDN_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define DCF_PDN_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define DCF_PDN_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define DCF_PDN_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define DCF_PDN_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define DCF_PDN_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define DCF_PDN_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define DCF_PDN_MASK               DCF_PDN__MASK
#define DCF_PDN_SHIFT              DCF_PDN__SHIFT
#define DCF_PDN_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DCF_PDN_SetInterruptMode() function.
     *  @{
     */
        #define DCF_PDN_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define DCF_PDN_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define DCF_PDN_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define DCF_PDN_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(DCF_PDN__SIO)
    #define DCF_PDN_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(DCF_PDN__PC) && (CY_PSOC4_4200L)
    #define DCF_PDN_USBIO_ENABLE               ((uint32)0x80000000u)
    #define DCF_PDN_USBIO_DISABLE              ((uint32)(~DCF_PDN_USBIO_ENABLE))
    #define DCF_PDN_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define DCF_PDN_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define DCF_PDN_USBIO_ENTER_SLEEP          ((uint32)((1u << DCF_PDN_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << DCF_PDN_USBIO_SUSPEND_DEL_SHIFT)))
    #define DCF_PDN_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << DCF_PDN_USBIO_SUSPEND_SHIFT)))
    #define DCF_PDN_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << DCF_PDN_USBIO_SUSPEND_DEL_SHIFT)))
    #define DCF_PDN_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(DCF_PDN__PC)
    /* Port Configuration */
    #define DCF_PDN_PC                 (* (reg32 *) DCF_PDN__PC)
#endif
/* Pin State */
#define DCF_PDN_PS                     (* (reg32 *) DCF_PDN__PS)
/* Data Register */
#define DCF_PDN_DR                     (* (reg32 *) DCF_PDN__DR)
/* Input Buffer Disable Override */
#define DCF_PDN_INP_DIS                (* (reg32 *) DCF_PDN__PC2)

/* Interrupt configuration Registers */
#define DCF_PDN_INTCFG                 (* (reg32 *) DCF_PDN__INTCFG)
#define DCF_PDN_INTSTAT                (* (reg32 *) DCF_PDN__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define DCF_PDN_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(DCF_PDN__SIO)
    #define DCF_PDN_SIO_REG            (* (reg32 *) DCF_PDN__SIO)
#endif /* (DCF_PDN__SIO_CFG) */

/* USBIO registers */
#if !defined(DCF_PDN__PC) && (CY_PSOC4_4200L)
    #define DCF_PDN_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define DCF_PDN_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define DCF_PDN_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define DCF_PDN_DRIVE_MODE_SHIFT       (0x00u)
#define DCF_PDN_DRIVE_MODE_MASK        (0x07u << DCF_PDN_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins DCF_PDN_H */


/* [] END OF FILE */
