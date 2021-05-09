/* ========================================
 *
 * Copyright HEMI, 2021
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 * 
 * Functions used by BeoModern main PSoC to display informaiton on BeoModern
 * front display as well as front LEDs. Includes funcitons that manage internal 
 * timer and clock. 
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF HEMI.
 *
 * ========================================
*/

#ifndef DISPLAY_H_   /* Include guard */
#define DISPLAY_H_

#include <project.h>
#include <operate.h>    

// variable storing current state of LEDs display
   uint16 LED_state; 

// variable to hold auto off timer value  
   uint8 auto_off_timer_current_state;

// variable to hold auto off timer value  
   uint8 auto_off_timer_new_settings;
    
// variable used to store posiiton of digits (time and date) that will be
// modified by the end user    
   int8 position_of_modified_digit;

// array to store new display data    
    uint8 modified_time_date[15]; 
    
// table containing max values for input data for each digit of time and date    
    static uint8 max_value[14] = {0x09, 0x09, 0x09, 0x09, 0x09, 0x03, 0x09, 0x01, 0x09, 0x05, 0x09, 0x05, 0x09, 0x02}; 
// table containinig information which digit should flash acording to position 
// when modifying clock and/or date    
    static uint32 flashing_digits[14] = {0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00010000, 0x00008000, 0x00001000, 0x00000800, 0x00000080, 0x00000040, 0x00000010, 0x00000008, 0x00000002, 0x00000001}; 

// variable to store timer off value in form of 4 BCD codes representing hours and minutes
    uint64 timer_off_time;     
    
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
*           disp_ctrl1 - Each bit represents LED on front interface
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
*           disp_ctrl2 - Each bit represents LED on front interface
*               LED control
*               CTRL1.0 - BT Tx
*               CTRL1.1 - Online status
*               CTRL1.2 - Signal Level
*               CTRL1.3 - Spectrum Analyser1
*               CTRL1.4 - Spectrum Analyser2
*               CTRL1.5 - Clock 1 (clock)
*               CTRL1.6 - Clock 2 (clock+date)
*               CTRL1.7 - NOT USED
*               more than one LED can be ON at particualr moment in time
*   display_content - pointer to table that stores 24 characters that should be printed on 
*               alphanumeric display
* Return:
*   pointer     - pointer to table that stores 24 new characters that should be printed on 
*               alphanumeric display
*
*******************************************************************************/
uint8* SPI_message_reader(uint8 *input_table, uint8 *display_content);    


/*******************************************************************************
* Function Name: Display_time
********************************************************************************
* Summary:
*  Funciton responsible for taking data from RTC block and present it on 
*  alphanumeric display. It handles two modes where moreinformaiton about time 
*  is displayed or less.
*
* Parameters:
*  clock_mode = 0x01 - display only hour and minutes 
*             = 0x02 - display time and date
*  new_display_data[24] - array to store new display data 
*
* Return:
*  0x01 when OK
*  0x00 when called with incorrect parameter
*
*******************************************************************************/
uint8 Display_time(uint8 clock_mode, uint8 new_display_data[24]);


/*******************************************************************************
* Function Name: Data_to_alphanumeric
********************************************************************************
* Summary:
*  Funciton that pints table content on alphanumeric display
*
* Parameters:
*  input_table[] - pointer to table with 24 characters that shoudl be printed 
*                  on an alphanumeric display
*
* Return:
*  0x01 when OK
*
*******************************************************************************/
uint8 Data_to_alphanumeric(uint8 input_table[]);


/*******************************************************************************
* Function Name: Clocks_control()
********************************************************************************
* Summary:
*  Funciton 
*
* Parameters:
*  mode = 1 report back clocks info for user to know when setting date and time
*       = 2 set internal and external RTC clocks using data provided by the 
*           user using remote controller or coming from atomic clock module
*       = 3 read external (battery operated) RTC module and update internal RTC 
* internal_time_date_table[] = table that stores time and date information. Each
*           BCD code has its own table index
*
* Return:
*  0x01 when OK
*
*******************************************************************************/
uint8 Clocks_control(uint8 mode, uint8 new_time_and_date_table[]);


/*******************************************************************************
* Function Name: Timer_control()
********************************************************************************
* Summary:
*  Funciton responsible for setting up power off timer that operates based on
*  delay selected by the user when beomodern is in clock mode. Once timer off
*  is enaled Beomodern will monitor internal RTC and power down whole system
*  when selected time expires.
*
* Parameters:
* timer_mode  = 0x00 - clear timer settings - timer off disabled
*             = 0x01 - report back remining time to off
*                        if return = 0x0 - no timer set
*                        if return = positive nubmer = remining minutes to off
*             = 0x02 - power off in 15 minutes
*             = 0x03 - power off in 30 minutes
*             = 0x04 - power off in 45 minutes
*             = 0x05 - power off in 60 minutes
*             = 0x06 - power off in 90 minutes
*
* Return:
*  0x0 - if no timer set
*  positive nubmer - equal to remining number of minutes to off in BCD format
*
*******************************************************************************/
uint32 Timer_control(uint8 timer_mode);



/*******************************************************************************
* Function Name: welcome_message()
********************************************************************************
* Summary:
*  Funciton called at initializaiton of beomodern. It checks time and reports
*  back mesassage to aplhanumeric display based on time of the day
*
* Parameters:
*   display_characters[24] - 24 character table storing display characters 
*
* Return:
*  0x1
*
*******************************************************************************/
uint8 welcome_message(uint8 display_characters[24]);



/*******************************************************************************
* Function Name: uint8 check_atomic_clock(void)
********************************************************************************
* Summary:
*  Funciton reponsible for updating RTC clocks (internal and external) based on
*  data received over radio from atomic DCF transmitter. 
*  In first step fucniton checks if it is 2:59 at night. If this time is
*  reached, Atomic LED is cleared indicating request to re-check and update time.
*  Internal RTC block is updated with data read from external battery assisted RTC
*  module.
*  In second step when time is between 3:00 and 6:00 and Atomic LED is off start
*  DCF receiver operation.
*  In third step keep checking DCF_data[] table with latest bits received by DCF 
*  antenna. When received data is valid, populate table with received data and 
*  update internal and external RTC blocks. When data is updated, enable Atomic 
*  LED and stop DCF receivign procedure for next ~24 hours (untill 2:59 next day)
*
* Parameters:
*   atomic_mode
*
* Return:
*  0x1
*
*******************************************************************************/
uint8 check_atomic_clock(void);


#endif // DISPLAY_H_

/* [] END OF FILE */
