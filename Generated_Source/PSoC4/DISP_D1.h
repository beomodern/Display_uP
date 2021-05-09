/*******************************************************************************
* File Name: DISP_D1.h  
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

#if !defined(CY_PINS_DISP_D1_H) /* Pins DISP_D1_H */
#define CY_PINS_DISP_D1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DISP_D1_aliases.h"


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
} DISP_D1_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   DISP_D1_Read(void);
void    DISP_D1_Write(uint8 value);
uint8   DISP_D1_ReadDataReg(void);
#if defined(DISP_D1__PC) || (CY_PSOC4_4200L) 
    void    DISP_D1_SetDriveMode(uint8 mode);
#endif
void    DISP_D1_SetInterruptMode(uint16 position, uint16 mode);
uint8   DISP_D1_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void DISP_D1_Sleep(void); 
void DISP_D1_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(DISP_D1__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define DISP_D1_DRIVE_MODE_BITS        (3)
    #define DISP_D1_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DISP_D1_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the DISP_D1_SetDriveMode() function.
         *  @{
         */
        #define DISP_D1_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define DISP_D1_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define DISP_D1_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define DISP_D1_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define DISP_D1_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define DISP_D1_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define DISP_D1_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define DISP_D1_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define DISP_D1_MASK               DISP_D1__MASK
#define DISP_D1_SHIFT              DISP_D1__SHIFT
#define DISP_D1_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DISP_D1_SetInterruptMode() function.
     *  @{
     */
        #define DISP_D1_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define DISP_D1_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define DISP_D1_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define DISP_D1_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(DISP_D1__SIO)
    #define DISP_D1_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(DISP_D1__PC) && (CY_PSOC4_4200L)
    #define DISP_D1_USBIO_ENABLE               ((uint32)0x80000000u)
    #define DISP_D1_USBIO_DISABLE              ((uint32)(~DISP_D1_USBIO_ENABLE))
    #define DISP_D1_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define DISP_D1_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define DISP_D1_USBIO_ENTER_SLEEP          ((uint32)((1u << DISP_D1_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << DISP_D1_USBIO_SUSPEND_DEL_SHIFT)))
    #define DISP_D1_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << DISP_D1_USBIO_SUSPEND_SHIFT)))
    #define DISP_D1_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << DISP_D1_USBIO_SUSPEND_DEL_SHIFT)))
    #define DISP_D1_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(DISP_D1__PC)
    /* Port Configuration */
    #define DISP_D1_PC                 (* (reg32 *) DISP_D1__PC)
#endif
/* Pin State */
#define DISP_D1_PS                     (* (reg32 *) DISP_D1__PS)
/* Data Register */
#define DISP_D1_DR                     (* (reg32 *) DISP_D1__DR)
/* Input Buffer Disable Override */
#define DISP_D1_INP_DIS                (* (reg32 *) DISP_D1__PC2)

/* Interrupt configuration Registers */
#define DISP_D1_INTCFG                 (* (reg32 *) DISP_D1__INTCFG)
#define DISP_D1_INTSTAT                (* (reg32 *) DISP_D1__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define DISP_D1_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(DISP_D1__SIO)
    #define DISP_D1_SIO_REG            (* (reg32 *) DISP_D1__SIO)
#endif /* (DISP_D1__SIO_CFG) */

/* USBIO registers */
#if !defined(DISP_D1__PC) && (CY_PSOC4_4200L)
    #define DISP_D1_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define DISP_D1_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define DISP_D1_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define DISP_D1_DRIVE_MODE_SHIFT       (0x00u)
#define DISP_D1_DRIVE_MODE_MASK        (0x07u << DISP_D1_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins DISP_D1_H */


/* [] END OF FILE */
