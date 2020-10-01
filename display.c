/* ========================================
 *
 * Copyright HEMI, 2020
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 * 
 * Functions used by BeoModern main PSoC to display informaiton on BeoModern
 * front display as well as front LEDs
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF HEMI.
 *
 * ========================================
*/

#include <display.h>

/*******************************************************************************
* Function Name: SPI_message_reader
********************************************************************************
* Summary:
*  Funciton reponsible for decoding and displayin commands that were sent over 
*  SPI interface to display. It intercepts 24 ASCI characters and pushes them to 
*  display. It also interprets two control words and manipulates LEDs acordingly
*  to control words state.
*
* Parameters:
*  input_table[] - table returned by SPI slave routine contining 24 ASCI
*                  characters and 2 control words for LED ON/OFF states
*  disp_ctrl1 - Each bit represents LED on front interface
*               LED control
*               CTRL0.0 - Player
*               CTRL0.1 - iRadio
*               CTRL0.2 - DAB Radio
*               CTRL0.3 - Aux IN
*               CTRL0.4 - Clock    
*               CTRL0.5 - RDS/FM Display
*               CTRL0.6 - BT Rx    
*               CTRL0.7 - NOT USED
                only one LED can be ON at particualr moment in time
*  disp_ctrl2 - Each bit represents LED on front interface
*               LED control
*               CTRL1.0 - BT Tx
*               CTRL1.1 - Online status
*               CTRL1.2 - Signal Level
*               CTRL1.3 - Spectrum Analyser
*               CTRL1.4 - Clock 1
*               CTRL1.5 - Clock 2
*               CTRL1.6 - NOT USED
*               CTRL1.7 - NOT USED
*               more than one LED can be ON at particualr moment in time
* Return:
*  Returns display status
*  0 - power down
*  1 - operational
*  2 - ...
*
*******************************************************************************/
uint8 SPI_message_reader(uint8 input_table[])
{
    uint8 counter;
// scan thru incoming table and print information to display    
    for (counter = 0; counter <RxPACKET_SIZE-4 ; counter++)
    {
// REPLACE WITH LED DISPLAY        
        UART_UartPutChar(input_table[counter]);
// REPLACE WITH LED DISPLAY        
    }
    
// TO BE REMOVED WHEN IN DISPLAY    
    UART_UartPutString("   ");
// TO BE REMOVED WHEN IN DISPLAY

// scan thru two control words and display LED information    
// REPLACE WITH LEDs in DISPLAY MODULE
    if ( ((RxBuffer[RxPACKET_CTRL1_POS-1]) & 0x01) == 0x01)
    {
        UART_UartPutString("PLAYER ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL1_POS-1]) & 0x02) == 0x02)
    {
        UART_UartPutString("iRADIO ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL1_POS-1]) & 0x04) == 0x04)
    {
        UART_UartPutString("DAB ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL1_POS-1]) & 0x08) == 0x08)
    {
        UART_UartPutString("AUXIN ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL1_POS-1]) & 0x10) == 0x10)
    {
        UART_UartPutString("CLOCK ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL1_POS-1]) & 0x20) == 0x20)
    {
        UART_UartPutString("RDS/FM ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL1_POS-1]) & 0x40) == 0x40)
    {
        UART_UartPutString("BT_RX ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL1_POS-1]) & 0x80) == 0x80)
    {
        UART_UartPutString("CTRL1=0x80 ");
    }            
    
    if ( ((RxBuffer[RxPACKET_CTRL2_POS-1]) & 0x01) == 0x01)
    {
        UART_UartPutString("BT_TX ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL2_POS-1]) & 0x02) == 0x02)
    {
        UART_UartPutString("ONLINE ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL2_POS-1]) & 0x04) == 0x04)
    {
        UART_UartPutString("SIGLEVEL ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL2_POS-1]) & 0x08) == 0x08)
    {
        UART_UartPutString("SPECANALYZER ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL2_POS-1]) & 0x10) == 0x10)
    {
        UART_UartPutString("CLOCK 1 ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL2_POS-1]) & 0x20) == 0x20)
    {
        UART_UartPutString("CLOCK 2");
    }
    if ( ((RxBuffer[RxPACKET_CTRL2_POS-1]) & 0x40) == 0x40)
    {
        UART_UartPutString("CTRL2=0x40 ");
    }
    if ( ((RxBuffer[RxPACKET_CTRL2_POS-1]) & 0x80) == 0x80)
    {
        UART_UartPutString("CTRL2=0x80 ");
    }
// REPLACE WITH LEDs in DISPLAY MODULE

// TO BE REMOVED WHEN IN DISPLAY    
    UART_UartPutString("\r\n");
// TO BE REMOVED WHEN IN DISPLAY
    
// ENACHANCE WITH MORE INFO LATER ON    
    return 1;
}



/* [] END OF FILE */
