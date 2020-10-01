/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
* Date: 2020
*
* Description:
*  This project is intended to be used as main code that runs on PSoC KIT
*  inside BeoModern system. 
*  It is responsible for monitoring
*  - two push buttons (short and long press)
*  - listenning and responding to commands sent over BeO Datalink
*  It is responsible for controling 
*  - Raspberry Pi that acts as iRadio, mp3/flac player, DAB radio, FM radio
*    station name display
*  - SigmaDSP processor togethre with audio DAC responsible for processing 
*    audio and producing analog sound
*  - Bluetooth module
*  - power supply system (power up and down different blocks)
*  - information sent to the display module
*
*******************************************************************************/

#include <project.h>
#include <inputs.h>
#include <modes.h>
#include <operate.h>
#include <display.h>

 // TO BE REMOVED TO BE REMOVED
    uint32 ch;          /* variable used by routine used to quickly move to load mode */
    uint8 display_status = 0;
 // TO BE REMOVED TO BE REMOVED


void main()
{   


//Manually set P4.2 as input. Used to Read Raspberry Pi status
//    CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT4_PC,2,CY_SYS_PINS_DM_DIG_HIZ);
//Manually set P4.3 as input. Used to Read BT AAC status
//    CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT4_PC,3,CY_SYS_PINS_DM_DIG_HIZ);


// Starts DISPLAY timer
    DISPLAY_Timer_Start();
    
// Enable DISPLAY timer interrupt and set up handler for it
    DISPLAY_Timer_INT_StartEx(DISPLAY_Timer_INTHandler);    

// Connects user made function to interrupt
    SPIS_SetCustomInterruptHandler(*Slave_ISR);
    
// Enable interrupt routines
    CyGlobalIntEnable;
    
// Start SCB (UART mode) operation 
    UART_Start();

// Start SCB (SPI mode) operation 
    SPIS_Start();
      
 /*
    // Writing data into the SPIS TX software buffer 
    SPIS_SpiUartWriteTxData(0x11u);
    SPIS_SpiUartWriteTxData(0x22u);

    // Writing data into the SPIM software buffer 
    SPIS_SpiUartWriteTxData(0x99u);
    SPIS_SpiUartWriteTxData(0xAAu);
    
    while(0u == (SPIS_GetMasterInterruptSource() & SPIS_INTR_MASTER_SPI_DONE))
    {
        // Wait while Master completes transfer 
    }
    
    // Clear interrupt source after transfer completion 
    SPIS_ClearMasterInterruptSource(SPIS_INTR_MASTER_SPI_DONE);
  
*/  
   
    
// Put dummy data into the slave TX buffer to be transferred while following
// first master access.
    SPIS_UpdateStatus(0x00);
        
    for(;;)
    {
// press push button to reset SPI interface        
        if (Push_button_Read() == 0)
        {
// Clear TX buffer from dummy bytes            
            SPIS_SpiUartClearTxBuffer();
// Clear RX buffer from dummy bytes
            SPIS_SpiUartClearRxBuffer();
// clear flag set by interrupt               
            flag = 0;            
// Clears the interrupt source
            SPIS_ClearRxInterruptSource(SPIS_INTR_RX_NOT_EMPTY);
// switch on/off LED when software works
            PSoC_LED_Write(~PSoC_LED_Read());
        }
        
// Check if SPI interface set interrupt (new data is available)
        if (flag == 1)
        {
// set display status to 0
            display_status = 0;
// Check if new data is available
            if (SPIS_WaitForCommand(RxBuffer) == 1)
            {
                display_status = SPI_message_reader(RxBuffer);
            }
// Put data into the slave TX buffer to be transferred while following
// master access.           
            SPIS_UpdateStatus(display_status);    
// Clear slave buffer after response has been read        
            SPIS_CleanupAfterRead();
// clear flag set by interrupt            
            flag = 0;
// Clears the interrupt source
            SPIS_ClearRxInterruptSource(SPIS_INTR_RX_NOT_EMPTY);
        }
        
// TO BE REMOVED TO BE REMOVED        
        ch = UART_UartGetChar();
//  Enters bootload state when "/" is pressed
        if ('/' == ch)        
            {
            Bootloadable_1_Load();
            }   
// TO BE REMOVED TO BE REMOVED
    }
}


/* [] END OF FILE */
