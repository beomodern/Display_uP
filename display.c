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

#include <display.h>
#include <inputs.h>

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
uint8* SPI_message_reader(uint8 *input_table, uint8 *display_content)
{
// variable to hold counter usued to scan thru received packadge    
    uint8 counter = 0;
// variale used to store LED display state    
    uint16 new_LED_state = LED_state;
// table used to store incoming messages when controling clock or timer
    uint8 tmp_buffer[24] = {0};
// table used during clock mode to sent menu info when modifying clock or timer
    static uint8 config_menu_display_data[24] = {0};
// table used to store values later on printed on alphanumeric display    
    static uint8 display_characters[25] = {0};

// reset clock flag - it will be set if clock modes are selected    
    clock_flag = 0x00;

// scan thru incoming SPI message and print relevant information to alphanumeric
// display    
    for (counter = 0; counter <= (PACKET_SIZE-5) ; counter++)
    {
// convert information from SPI from ASCI to ASCI + 0x80. This moves alphanumerc
// memory location to custom specific characters. Used with signal level and 
// spectrum analyser modes
        if ( ((input_table[PACKET_CTRL2_POS-1]) & 0x04) == 0x04 )
        {
            display_characters[counter] = input_table[counter] | 0x80;
        }
        else if ( ( ((input_table[PACKET_CTRL2_POS-1]) & 0x08) == 0x08 ) || ( ((input_table[PACKET_CTRL2_POS-1]) & 0x10) == 0x10 ))
        {
// alter characters 0, 1, 12 and 13 back to ASCI one for spectrum analyser            
            if  ( (counter == 0) || (counter == 1) || (counter == 12) || (counter == 13) )
            {
                display_characters[counter] = input_table[counter];
            }
            else
                {
                    display_characters[counter] = input_table[counter] | 0x80;
                }
        }
        else 
// fill table that will be print on alphanumeric display with characters 
// arriving over SPI interface        
            {
                display_characters[counter] = input_table[counter];
            }
    }

// map internal display charracters pointer to display content one (provided in fucntion call)
    display_content = display_characters;
    
// check if status bajt is clear system is booting up or powering down
    if ( input_table[PACKET_CTRL1_POS-1] == 0x00)
    {
// check if this is powering up state (UART message states "Booting up... "        
        if ( (display_characters[0] == 'B') && (display_characters[1] == 'o'))
        {
// call funciton to alter display message to welcome one based on current time            
           welcome_message(display_characters);
        }
    }
    
    
// scan thru two control words and display relevant information on LEDs 
// if CTRL0.0 = 0x01 -> Player
    if ( ((input_table[PACKET_CTRL1_POS-1]) & 0x01) == 0x01)
    {
        if ( (new_LED_state & 0x0040) != 0x0040)
        {
// enable LED indicating Player mode            
            new_LED_state = (new_LED_state & 0xFF00) | 0x0040;
        }
    }
    
// if CTRL0.1 = 0x01 -> iRadio
    if ( ((input_table[PACKET_CTRL1_POS-1]) & 0x02) == 0x02)
    {
        if ( (new_LED_state & 0x0008) != 0x0008)
        {
// enable LED indicating iRadio mode
            new_LED_state = (new_LED_state & 0xFF00) | 0x0008;
        }
    }
    
// if CTRL0.2 = 0x01 -> DAB Radio
    if ( ((input_table[PACKET_CTRL1_POS-1]) & 0x04) == 0x04)
    {
        if ( (new_LED_state & 0x0010) != 0x0010)
        {
// enable LED indicating DAB Radio mode            
            new_LED_state = (new_LED_state & 0xFF00) | 0x0010;
        }
    }
    
// if CTRL0.3 = 0x01 -> Aux IN mode
    if ( ((input_table[PACKET_CTRL1_POS-1]) & 0x08) == 0x08)
    {
        if ( (new_LED_state & 0x0001) != 0x0001)
        {
// enable LED indicating Aux IN mode            
            new_LED_state = (new_LED_state & 0xFF00) | 0x0001;
        }
    }
    
// if CTRL0.4 = 0x01 -> Clock
    if ( ((input_table[PACKET_CTRL1_POS-1]) & 0x10) == 0x10)
    {
        if ( (new_LED_state & 0x0004) != 0x0004)
        {
// enable LED indicating Clock mode            
            new_LED_state = (new_LED_state & 0xFF00) | 0x0004;
        }
        
// check if MENU button was pressed        
        strcpy((char*)tmp_buffer, "CMD:+MENU+--------------");
        if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
        {
// if so, put prompt info on display
            strcpy((char*)config_menu_display_data, "Set Clock?   OK or EXIT ");
// set timer for prompt message to appear 5 seconds (25 * 200mS) on display
            clock_message_timer = 25u;
        }

// check if EXIT button was pressed
        strcpy((char*)tmp_buffer, "CMD:+EXIT+--------------");
        if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
        {
// if so, reset clock modification timer - back to normal time display operation
            clock_message_timer = 0x00;
// finish clock+date user modification - set position to negative value               
            position_of_modified_digit = -1;
// disable flashing digits on alphanumeric display            
            ControlAlphaNumDisplay(0x08000000);
            ControlAlphaNumDisplay(0x00000000);
// clear auto-off setup value - preparing for another user timer set/check
            auto_off_timer_current_state = 0x00;
            auto_off_timer_new_settings = 0x00;
        }
        
// check if timer is active indicating user want to modify time and/or date        
        if ( clock_message_timer > 0u )
        {
// check if auto-off timer was checked by the user. If not operate to set
// clock parameters            
            if (auto_off_timer_current_state == 0x00)
            {
// display prompt command and wait for user to press OK            
                if ( position_of_modified_digit < 0 )
                {
                    strcpy((char*)tmp_buffer, "CMD:+OKGO+--------------");
                    if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
                    {
// read back current clock time and date            
                        Clocks_control(0x01, modified_time_date);
// set position of modiefied digit to 0 - indicate that user want to 
// adjust time and date            
                        position_of_modified_digit = 0;
                    }
                }
                else
                    {
// if second OK is presses, store new time and date
                        strcpy((char*)tmp_buffer, "CMD:+OKGO+--------------");
                        if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
                        {
// Set new clock and date info into internal RTC block and external RTC module
                            Clocks_control(0x02, modified_time_date);
// finish clock+date user modification - set position to negative value                 
                            position_of_modified_digit = -1;
// reset clock modification timer - back to normal time display operation
                            clock_message_timer = 0x00;   
// disable flashing digits on alphanumeric display            
                            ControlAlphaNumDisplay(0x00000000);
                        }
                    }                        
            }
// if auto-off timer was checked by the user allow its configurability
            else
                {
                    if ( (auto_off_timer_new_settings >= 0x01) && (auto_off_timer_new_settings <= 0x06) )
                    {
// if condiiton is true, user decided on new power off timer setting
                        strcpy((char*)tmp_buffer, "CMD:+OKGO+--------------");
                        if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
                        {
// if new settings are 0x01 that means user wants to disable timer                             
                            if (auto_off_timer_new_settings == 0x01)
                            {
// call new timer settings with disabled timer                                
                                Timer_control(0x00);
// disable/switch_off timer off LED
                                new_LED_state = new_LED_state & 0xFDFF;
                            }
                            else
                                {
// otherwise call new timer settings with device off time
                                    Timer_control(auto_off_timer_new_settings);
// enable/switch_on timer off LED
                                    new_LED_state = new_LED_state | 0x0200; 
                                }
                            
// disable indication that user wants to modify timer setting                            
                            auto_off_timer_new_settings = 0x00;
// disable flashing digits on alphanumeric display            
                            ControlAlphaNumDisplay(0x00000000);
                            strcpy((char*)config_menu_display_data, "   Auto-Off timer set   ");
                        }
                    }
                    else
                        {
// if it is outside of that range no modificaiton ocured
// of OK is pressed, enable editing mode
                            strcpy((char*)tmp_buffer, "CMD:+OKGO+--------------");
                            if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
                            {
// indicate that user wants to modify timer setting                            
                                auto_off_timer_new_settings = 0x01;
                            }
                
                        }
                }
        }
        
// check if user wants to modify off-timer or time
// if time                    
        if (auto_off_timer_new_settings == 0x00)
        { 
// check if user wants to modify time or date        
            if ( position_of_modified_digit >= 0 )
            {
// print to display time and date in edit format        
                config_menu_display_data[0] = '0' + modified_time_date[13];
                config_menu_display_data[1] = '0' + modified_time_date[12];
                config_menu_display_data[2] = ':';
                config_menu_display_data[3] = '0' + modified_time_date[11];
                config_menu_display_data[4] = '0' + modified_time_date[10];
                config_menu_display_data[5] = '.';
                config_menu_display_data[6] = '0' + modified_time_date[9];
                config_menu_display_data[7] = '0' + modified_time_date[8];
                config_menu_display_data[8] = ' ';
                config_menu_display_data[9] = ' ';
                config_menu_display_data[10] = 'M';
                config_menu_display_data[11] = '0' + modified_time_date[7];
                config_menu_display_data[12] = '0' + modified_time_date[6];
                config_menu_display_data[13] = '/';
                config_menu_display_data[14] = 'D';
                config_menu_display_data[15] = '0' + modified_time_date[5];
                config_menu_display_data[16] = '0' + modified_time_date[4];
                config_menu_display_data[17] = '/';
                config_menu_display_data[18] = 'Y';
                config_menu_display_data[19] = '0' + modified_time_date[3];
                config_menu_display_data[20] = '0' + modified_time_date[2];
                config_menu_display_data[21] = '0' + modified_time_date[1];
                config_menu_display_data[22] = '0' + modified_time_date[0];
                config_menu_display_data[23] = ' ';

// set active flashing digit on display                
                ControlAlphaNumDisplay( flashing_digits[position_of_modified_digit]);
// enable digit flashing mode                
                ControlAlphaNumDisplay(0x08000000);

// check if left button pressed                 
                strcpy((char*)tmp_buffer, "CMD:+LEFT+--------------");
                if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
                {
// if so, move digit to edit by one posiiton to the left
                    position_of_modified_digit = position_of_modified_digit + 1; 
                    if (position_of_modified_digit == 14)
                    {
                        position_of_modified_digit = 0;
                    }
// set clock message timer to 5 seconds (25 * 200mS)
                    clock_message_timer = 25u;                    
                }

// check if right button pressed                 
                strcpy((char*)tmp_buffer, "CMD:+RIGT+--------------");
                if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
                {
// if so, move digit to edit by one posiiton to the right                    
                    position_of_modified_digit = position_of_modified_digit - 1; 
                    if (position_of_modified_digit < 0)
                    {
                        position_of_modified_digit = 13;
                    }
// set clock message timer to 5 seconds (25 * 200mS)
                    clock_message_timer = 25u;
                }
            
// check if up button pressed 
                strcpy((char*)tmp_buffer, "CMD:+UPUP+--------------");
                if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
                {
// if up button pressed increase value of selected digit
// use table to make sure that values are within bounds of date and time format
                    if (modified_time_date[position_of_modified_digit] == max_value[position_of_modified_digit])
                    {
                        modified_time_date[position_of_modified_digit] = 0x00;
                    }
                    else
                        {
                            modified_time_date[position_of_modified_digit] = modified_time_date[position_of_modified_digit] + 1;
                        }
// set clock message timer to 5 seconds (25 * 200mS)
                    clock_message_timer = 25u;
                }

// check if down button pressed                
                strcpy((char*)tmp_buffer, "CMD:+DOWN+--------------");
                if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
                {
// if down button pressed decrease value of selected digit
// use table to make sure that values are within bounds of date and time format                    
                    if (modified_time_date[position_of_modified_digit] <= 0x00)
                    {
                        modified_time_date[position_of_modified_digit] = max_value[position_of_modified_digit];
                    }
                    else
                        {
                            modified_time_date[position_of_modified_digit] = modified_time_date[position_of_modified_digit] - 1;
                        }
// set clock message timer to 5 seconds (25 * 200mS)
                    clock_message_timer = 25u;
                }
            }
        }
        else
// if off timer          
            {
                // print to display time and date in edit format        
                strcpy((char*)config_menu_display_data, "Set Auto-Off in XX min? ");
                if (auto_off_timer_new_settings == 0x01)
                {
                    config_menu_display_data[16] = '-';
                    config_menu_display_data[17] = '-';
                }
                else if (auto_off_timer_new_settings == 0x02)
                {
                    config_menu_display_data[16] = '1';
                    config_menu_display_data[17] = '5';
                }
                else if (auto_off_timer_new_settings == 0x03)
                {
                    config_menu_display_data[16] = '3';
                    config_menu_display_data[17] = '0';
                }
                else if (auto_off_timer_new_settings == 0x04)
                {
                    config_menu_display_data[16] = '4';
                    config_menu_display_data[17] = '5';
                }
                else if (auto_off_timer_new_settings == 0x05)
                {
                    config_menu_display_data[16] = '6';
                    config_menu_display_data[17] = '0';
                }
                else if (auto_off_timer_new_settings == 0x06)
                {
                    config_menu_display_data[16] = '9';
                    config_menu_display_data[17] = '0';
                }

// set active flashing digit on display                
                ControlAlphaNumDisplay(0x00030000);
// enable digit flashing mode                
                ControlAlphaNumDisplay(0x08000000);
// check if up button pressed 
                strcpy((char*)tmp_buffer, "CMD:+UPUP+--------------");
                if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
                {
// increse new value for off timer
                    auto_off_timer_new_settings = auto_off_timer_new_settings + 1;
// make sure not to exceed max value                            
                    if (auto_off_timer_new_settings >= 0x07)
                    {
// value of 1 indicates off-timer disabled
                        auto_off_timer_new_settings = 0x01;
                    }
// set clock message timer to 5 seconds (25 * 200mS)
                    clock_message_timer = 25u;
                }
                
// check if down button pressed                
                strcpy((char*)tmp_buffer, "CMD:+DOWN+--------------");
                if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
                {
// decrease new value for off timer
                    auto_off_timer_new_settings = auto_off_timer_new_settings - 1;
// make sure not to exceed min value of 0x01 
                    if (auto_off_timer_new_settings < 0x01)
                    {
// value of 6 indicates off-timer set to 90 minutes
                        auto_off_timer_new_settings = 0x06;
                    }
// set clock message timer to 5 seconds (25 * 200mS)
                    clock_message_timer = 25u;
                }
            }
        
// check if button STOP was pressed        
        strcpy((char*)tmp_buffer, "CMD:+STOP+--------------");
        if ( strcmp((char*)display_characters,(char*)tmp_buffer) == 0 )
        {
// read back auto off timer state            
            auto_off_timer_current_state = Timer_control(0x01);    
// check if timer control is set and currently active?
            if (auto_off_timer_current_state == 0u)
            {
// if it is inactive, put prompt message on display "Set OFF-Timer? OK / EXIT"
                strcpy((char*)config_menu_display_data, "Set Off-Timer? OK / EXIT");
// set auto-off timer state to 0xFF indicating that user wants to set-up timer.
                auto_off_timer_current_state = 0xFF;
            }
            else
                {
// if it is active, put prompt message on display stating how much more time was left to off
                    strcpy((char*)config_menu_display_data, "Auto-Off in xx minutes  ");
                    config_menu_display_data[12] = '0' + ((auto_off_timer_current_state >> 4) & 0x0F);
                    config_menu_display_data[13] = '0' + (auto_off_timer_current_state & 0x0F);
                }
// set clock message timer to 5 seconds (25 * 200mS)
            clock_message_timer = 25u;
        }
 
// Main statement that updates display in Clock mode        
        if ( clock_message_timer > 0u )
        {
// print new message on display
            strcpy((char*)display_characters,(char*)config_menu_display_data);
        }
        else
            {
// set flag to display clock+data info   
                clock_flag = 0x02;
// check if timer expired, if so reset position informaiton and flashing flag                
                if ( (position_of_modified_digit != (-1)))
                {
// disable flashing digits on alphanumeric display            
                    ControlAlphaNumDisplay(0x00000000);
// clear flag indicatign clock modification - set position to negative value                  
                    position_of_modified_digit = -1;
// clear auto-off timer state 
                    auto_off_timer_current_state = 0x00;
                }
            }
    }
    
// if CTRL0.5 = 0x01 -> RDS/FM display
    if ( ((input_table[PACKET_CTRL1_POS-1]) & 0x20) == 0x20)
    {
        if ( (new_LED_state & 0x0080) != 0x0080)
        {
// enable LED indicating Bluetooth RDS/FM display mode            
            new_LED_state = (new_LED_state & 0xFF00) | 0x0080;
        }
    }
    
// if CTRL0.6 = 0x01 -> BlueTooth Rx mode
    if ( ((input_table[PACKET_CTRL1_POS-1]) & 0x40) == 0x40)
    {
        if ( (new_LED_state & 0x0002) != 0x0002)
        {
// enable LED indicating Bluetooth Rx ON mode            
            new_LED_state = (new_LED_state & 0xFF00) | 0x0002;
        }
    }
// if CTRL0.7 = 0x01 -> Not used
//    if ( ((RxBuffer[RxPACKET_CTRL1_POS-1]) & 0x80) == 0x80)
//    {
//        Set_I2C_LED_Driver(0x0080);
//    }            

// if CTRL1.0 = 0x01 -> BlueTooth Tx
    if ( ((input_table[PACKET_CTRL2_POS-1]) & 0x01) == 0x01)
    {
        if ( (new_LED_state & 0x1000) == 0x0000)
        {
// enable LED indicating Bluetooth Tx ON mode            
            new_LED_state = new_LED_state | 0x1000;
// disable LED indicating Bluetooth Rx ON mode            
            new_LED_state = new_LED_state & 0xFFFD;
        }
    }
    else
        {
            if ( (new_LED_state & 0x1000) == 0x1000)
            {
// disable LED indicating Bluetooth Tx ON mode
                new_LED_state = new_LED_state & 0xEFFF;
            }
        }

// if CTRL1.1 = 0x01 -> ONLINE status        
    if ( ((input_table[PACKET_CTRL2_POS-1]) & 0x02) == 0x02)
    {
        if ( (new_LED_state & 0x0100) == 0x0000)
        {
// enable LED indicating online status
            new_LED_state = new_LED_state | 0x0100;
        }
    }
    else
        {
            if ( (new_LED_state & 0x0100) == 0x0100)
            {
// disable LED indicating online status
                new_LED_state = new_LED_state & 0xFEFF;
            }
        }

// if CTRL1.2 = 0x01 -> Signal Level mode
    if ( ((input_table[PACKET_CTRL2_POS-1]) & 0x04) == 0x04)
    {
        if ( (new_LED_state & 0x8000) != 0x8000)
        {
// enable LED indicating signal level mode            
            new_LED_state = new_LED_state | 0x8000;
// load display with signal level symbols           
            LoadAlphaNumDisplay(0x01);
        }
// disable clock flag (in case of clock mode to display signal level data)
        clock_flag = 0x00;
    }
    else
        {
            if ( (new_LED_state & 0x8000) == 0x8000)
            {
// disable LED indicating signal level mode            
                new_LED_state = new_LED_state & 0x7FFF;
            }
        }        

// if CTRL1.3 = 0x01 -> Spectrum Analyser1 mode       
    if ( ((input_table[PACKET_CTRL2_POS-1]) & 0x08) == 0x08)
    {
        if ( (new_LED_state & 0x0800) != 0x0800)
        {
// enable LED indicating spectrum analyser 1 mode            
            new_LED_state = new_LED_state | 0x0800;
// load display with spectrum analyser1 symbols            
            LoadAlphaNumDisplay(0x02);
        }
// disable clock flag (in case of SpecAnl_1 mode to display signal level data)
        clock_flag = 0x00;
    }
    else
        {
            if ( (new_LED_state & 0x0800) == 0x0800)
            {
// disable LED indicating spectrum analyser 1 mode                
                new_LED_state = new_LED_state & 0xF7FF;
            }
        }

// if CTRL1.4 = 0x01 -> Spectrum Analyser2 mode       
    if ( ((input_table[PACKET_CTRL2_POS-1]) & 0x10) == 0x10)
    {
        if ( (new_LED_state & 0x0400) != 0x0400)
        {
// enable LED indicating spectrum analyser 2 mode            
            new_LED_state = new_LED_state | 0x0400;
// load display with spectrum analyser2 symbols            
            LoadAlphaNumDisplay(0x03);
        }
// disable clock flag (in case of SpecAnl_2 mode to display signal level data)
        clock_flag = 0x00;
    }
    else
        {
            if ( (new_LED_state & 0x0400) == 0x0400)
            {
// disable LED indicating spectrum analyser 2 mode
                new_LED_state = new_LED_state & 0xFBFF;
            }
        }        
        
// if CTRL1.5 = 0x01 -> Clock 1 mode        
    if ( ((input_table[PACKET_CTRL2_POS-1]) & 0x20) == 0x20)
    {
// set flag to display simple clock info
        clock_flag = 0x01;
    }

// if CTRL1.6 = 0x01 -> Clock 2 mode        
    if ( ((input_table[PACKET_CTRL2_POS-1]) & 0x40) == 0x40)
    {
// set flag to display clock+date into        
        clock_flag = 0x02;
    }
    
// if CTRL1.7 = 0x01 -> not used
//    if ( ((RxBuffer[RxPACKET_CTRL2_POS-1]) & 0x80) == 0x80)
//    {
//        Set_I2C_LED_Driver(0x4000);
//    }

// update LED display if new state value is detected
    if (new_LED_state != LED_state)
    {
        LED_state = new_LED_state;
        Set_I2C_LED_Driver(LED_state);    
    }
    
// return pointer to table with new display data   
    return display_content;
}


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
uint8 Display_time(uint8 clock_mode, uint8 new_display_data[24]) 
{
//variables used to store clock and data info
    uint32 time_disp;
    uint32 date_disp;
 
// populate table with simple clock info    
    if (clock_mode == 0x01)
    {
 // get back time from internal RTC block
        time_disp = Int_RTC_GetTime();
// populate table with simple display information
        new_display_data[0] = ' ';
        new_display_data[1] = ' ';
        new_display_data[2] = ' ';
        new_display_data[3] = ' ';
        new_display_data[4] = ' ';
        new_display_data[5] = ' ';
        new_display_data[6] = ' ';
        new_display_data[7] = ' ';
        new_display_data[8] = ' ';
        new_display_data[9] = ' ';
        new_display_data[10] = '0' + ((time_disp & 0x00F00000) >> 20);
        new_display_data[11] = '0' + ((time_disp & 0x000F0000) >> 16);
        new_display_data[12] = ':';
        new_display_data[13] = '0' + ((time_disp & 0x0000F000) >> 12);
        new_display_data[14] = '0' + ((time_disp & 0x00000F00) >> 8);
        new_display_data[15] = ' ';
        new_display_data[16] = ' ';
        new_display_data[17] = ' ';
        new_display_data[18] = ' ';
        new_display_data[19] = ' ';
        new_display_data[20] = ' ';
        new_display_data[21] = ' ';
        new_display_data[22] = ' ';
        new_display_data[23] = ' ';
// return 1 when finish successfully           
        return 0x01;
    }

// populate table with clock and data info    
    if (clock_mode == 0x02)
    {
// read back time and data info from internal RTC block
        time_disp = Int_RTC_GetTime();
        date_disp = Int_RTC_GetDate();
// populate table with clock informaiton hours:minutes.seconds        
        new_display_data[0] = ' ';
        new_display_data[1] = ' ';
        new_display_data[2] = ' ';
        new_display_data[3] = '0' + ((time_disp & 0x00F00000) >> 20);
        new_display_data[4] = '0' + ((time_disp & 0x000F0000) >> 16);
        new_display_data[5] = ':';
        new_display_data[6] = '0' + ((time_disp & 0x0000F000) >> 12);
        new_display_data[7] = '0' + ((time_disp & 0x00000F00) >> 8);
        new_display_data[8] = '.';
        new_display_data[9] = '0' + ((time_disp & 0x000000F0) >> 4);
        new_display_data[10] = '0' + (time_disp & 0x0000000F);
        new_display_data[11] = ' ';
        new_display_data[12] = ' ';
        new_display_data[13] = ' ';

//populate table with day of the week info        
        switch (Int_RTC_currentTimeDate.dayOfWeek)
        {
            case (1):
                new_display_data[14] = 'S';
                new_display_data[15] = 'u';
                new_display_data[16] = 'n';
                break;
            case (2):
                new_display_data[14] = 'M';
                new_display_data[15] = 'o';
                new_display_data[16] = 'n';
                break;
            case (3):
                new_display_data[14] = 'T';
                new_display_data[15] = 'u';
                new_display_data[16] = 'e';
                break;
            case (4):
                new_display_data[14] = 'W';
                new_display_data[15] = 'e';
                new_display_data[16] = 'd';
                break;
            case (5):
                new_display_data[14] = 'T';
                new_display_data[15] = 'h';
                new_display_data[16] = 'r';
                break;
            case (6):
                new_display_data[14] = 'F';
                new_display_data[15] = 'r';
                new_display_data[16] = 'i';
                break;
            case (7):
                new_display_data[14] = 'S';
                new_display_data[15] = 'a';
                new_display_data[16] = 't';
                break;
        }

// populate table with day of the month info
        new_display_data[17] = ' ';
        new_display_data[18] = '0' + ((date_disp & 0x00F00000) >> 20);
        new_display_data[19] = '0' + ((date_disp & 0x000F0000) >> 16);
        new_display_data[20] = ' ';

// populate table with month info
        switch ( ((date_disp & 0xF0000000) >> 28) + ((date_disp & 0x0F000000) >> 24))
        {
            case (1):
                new_display_data[21] = 'J';
                new_display_data[22] = 'a';
                new_display_data[23] = 'n';
                break;
            case (2):
                new_display_data[21] = 'F';
                new_display_data[22] = 'e';
                new_display_data[23] = 'b';
                break;                
            case (3):
                new_display_data[21] = 'M';
                new_display_data[22] = 'a';
                new_display_data[23] = 'r';
                break;
            case (4):
                new_display_data[21] = 'A';
                new_display_data[22] = 'p';
                new_display_data[23] = 'r';
                break;
           case (5):
                new_display_data[21] = 'M';
                new_display_data[22] = 'a';
                new_display_data[23] = 'y';
                break;
            case (6):
                new_display_data[21] = 'J';
                new_display_data[22] = 'u';
                new_display_data[23] = 'n';
                break;
            case (7):
                new_display_data[21] = 'J';
                new_display_data[22] = 'u';
                new_display_data[23] = 'l';
                break;
            case (8):
                new_display_data[21] = 'A';
                new_display_data[22] = 'u';
                new_display_data[23] = 'g';
                break;
            case (9):
                new_display_data[21] = 'S';
                new_display_data[22] = 'e';
                new_display_data[23] = 'p';
                break;
            case (10):
                new_display_data[21] = 'O';
                new_display_data[22] = 'c';
                new_display_data[23] = 't';
                break;
            case (11):
                new_display_data[21] = 'N';
                new_display_data[22] = 'o';
                new_display_data[23] = 'v';
                break;
            case (12):
                new_display_data[21] = 'D';
                new_display_data[22] = 'e';
                new_display_data[23] = 'c';
                break;
        }
// return 1 when finish successfully      
        return 0x01;
    }
    
// return 0 when called with incorrect parameter
    return 0x00;
}



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
uint8 Data_to_alphanumeric(uint8 input_table[])
{
// variable to hold counter usued to scan thru received packadge    
    uint8 counter;
    
// scan thru table and print characters on it    
    for (counter = 0; counter <=23 ; counter++)
        {
            CharacterToAlphaNumDisplay(input_table[counter], 23-counter);
        } 
        
    return 1;
}



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
* Ext_RTC_GetTime
* The time value in the HH:MM:SS format. <br>
* "HH"- The 2nd 8-bit MSB that denotes the hour value.
* (0-23 for the 24-hour format and 1-12 for the 12-hour format. The MSB bit of 
* the value denotes AM/PM for the 12-hour format (0-AM and 1-PM). <br>
* "MM" - The 3nd 8-bit MSB denotes the minutes value, the valid 
* entries -> 0-59.<br>
* "SS" - The 8-bit LSB denotes the seconds value, the valid entries -> 0-59.
* Each byte is in the BCD format. Invalid time entries retain the
* previously set values.
*
* Ext_RTC_GetDate
* The date value in the format selected in the customizer.
* For the MM/DD/YYYY format: <br>
* "MM" - The 8-bit MSB denotes the month value in BCD, the valid 
* entries -> 1-12<br>
* "DD" - The 2nd 8-bit MSB denotes a day of the month value in BCD, the valid
* entries -> 1-31.<br>
* "YYYY" - The 16-bit LSB denotes a year in BCD, the valid entries -> 1900-2200.
* Each byte is in the BCD format. Invalid date entries retain the
* previously set values.
*
* Return:
*  0x01 when OK
*
*******************************************************************************/
uint8 Clocks_control(uint8 mode, uint8 internal_time_date_table[])
{
    
//variables used to store clock and data info
    uint32 timedisp = 0;
    uint32 datedisp = 0;

// return variable
    uint8 ret_val = 0x00;
    
// for loop counter
    uint8 loop_counter = 0;
    
    switch (mode)
    {
// report back clocks info for user to know when settign date and time  
        case (0x01):
// read back time and data info from internal RTC block
            timedisp = Int_RTC_GetTime();
            datedisp = Int_RTC_GetDate();
// loop that scans thru variables with time and date and populates table with current values
            for (loop_counter = 0; loop_counter <15; loop_counter++)
            {
                if (loop_counter < 8)
                {
                        internal_time_date_table[loop_counter] = ((datedisp >> (4*loop_counter)) & 0x0000000F);
                }
                else
                    {
                        internal_time_date_table[loop_counter] = ((timedisp >> (4*(loop_counter-8))) & 0x0000000F);
                    }
            }
// function finished successfully            
            ret_val = 0x01;
            break;

            
// set internal and external RTC clocks using data provided by the 
// user using remote controller    
        case (0x02):
// read data supplied to the funciton thru table and extract time and data parameters
            timedisp = timedisp | ((0x0F & modified_time_date[13]) << 20);
            timedisp = timedisp | ((0x0F & modified_time_date[12]) << 16);
            timedisp = timedisp | ((0x0F & modified_time_date[11]) << 12);
            timedisp = timedisp | ((0x0F & modified_time_date[10]) << 8);
            timedisp = timedisp | ((0x0F & modified_time_date[9]) << 4);
            timedisp = timedisp | (0x0F & modified_time_date[8]);

            datedisp = datedisp | ((0x0F & modified_time_date[7]) << 28);
            datedisp = datedisp | ((0x0F & modified_time_date[6]) << 24);
            datedisp = datedisp | ((0x0F & modified_time_date[5]) << 20);
            datedisp = datedisp | ((0x0F & modified_time_date[4]) << 16);
            datedisp = datedisp | ((0x0F & modified_time_date[3]) << 12);
            datedisp = datedisp | ((0x0F & modified_time_date[2]) << 8);
            datedisp = datedisp | ((0x0F & modified_time_date[1]) << 4);
            datedisp = datedisp | (0x0F & modified_time_date[0]);
                
// set external battery assisted RTC block with value provided by the user 
            Ext_RTC_SetDateAndTime( timedisp, datedisp );        
// set internal RTC block with value provided by the user
            Int_RTC_SetDateAndTime( timedisp, datedisp );  
// function finished successfully           
            ret_val = 0x01;
            break;

            
// read external (battery operated) RTC module and update internal RTC    
        case (0x03):
// read back time and data info from external battery-assisted RTC module
            timedisp = Ext_RTC_GetTime();
            datedisp = Ext_RTC_GetDate();
// set internal RTC block with read back time and data into
            Int_RTC_SetDateAndTime( timedisp, datedisp );
// function finished successfully
            ret_val = 0x01;
            break;
    }

// return funciton value, if 0x00 fucniton called with incorrect parameter
    return ret_val;    
}



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
*                       if return = 0x0 - no timer set
*                       if return = positive nubmer = remining minutes to off
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
uint32 Timer_control(uint8 timer_mode)
{
// value used to store data returend by Timer_control function    
    uint32 return_value;

// variables used to store clock and data info
    uint64 current_UNIX_time_and_date;  
    uint64 remining_UNIX_time_and_date;  
    
// delay timer value
    uint16 delay_value = 0u;

// switch funciton that manages function modes    
    switch (timer_mode)
    {
        
// clear timer settings - timer off disabled        
        case (0x00):
// clear timer off variable        
            timer_off_time = 0x00000000;
// return 0x00 indicating timer is off        
            return_value = 0x00000000;
        break;

// check and report back remining time to off
        case (0x01):
            delay_value = 0xFFFF;
        break;

// power off in 15 minutes        
        case (0x02):
            delay_value = 900u;
// 1 minute delay for debug
//            delay_value = 30u;
        break;

// power off in 30 minutes        
        case (0x03):
            delay_value = 1800u;
        break;

// power off in 45 minutes        
        case (0x04):
            delay_value = 2700u;
        break;

// power off in 60 minutes        
        case (0x05):
            delay_value = 3600u;
        break;

// power off in 90 minutes        
        case (0x06):
            delay_value = 5400u;
        break;
    }

    
    if ((delay_value > 0u) && (delay_value < 0xFFFF))
    {
// read back current time and date in UNIX format
        current_UNIX_time_and_date = Int_RTC_DateTimeToUnix(Int_RTC_GetDate(), Int_RTC_GetTime());
// add selected delay number to readback value
        timer_off_time = current_UNIX_time_and_date + delay_value;
// return number of remining minutes        
        return_value = delay_value;
    }
    else if (delay_value == 0xFFFF)
        {
// if timer is not set            
            if (timer_off_time == 0x00000000)
            {
// return 0x00 value to indicate that                
                return_value = 0x00000000;
            }
// if timer is set, calculate remining minutes and report them back            
            else
                {
// read back current time and date in UNIX format
                    current_UNIX_time_and_date = Int_RTC_DateTimeToUnix(Int_RTC_GetDate(), Int_RTC_GetTime()); 
// calculate remining time to off in UNIX format
                    remining_UNIX_time_and_date = timer_off_time - current_UNIX_time_and_date;
// return back number of minutes that are to off state      
                    return_value = (remining_UNIX_time_and_date / 60 ) + 1;
                }
        }
// convert returned value back to BCD format
    return_value = Int_RTC_ConvertDecToBCD(return_value);
// return value in BCD format    
    return return_value;
}



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
uint8 welcome_message(uint8 display_characters[24])
{
//variables used to store clock and data info
    uint32 timevalue = 0;
// read back time and data info from internal RTC block
// "HH"- The 2nd 8-bit MSB that denotes the hour value.
// (0-23 for the 24-hour format and 1-12 for the 12-hour format. The MSB bit of 
// the value denotes AM/PM for the 12-hour format (0-AM and 1-PM). <br>  
    timevalue = Int_RTC_GetTime();    
// convert current hour (in BCD code) to simple number    
    timevalue = (((timevalue >> 20) & 0x03) * 10 ) + ((timevalue >> 16) & 0x0F);

// populate display with welcome message    
    display_characters[0] = 'G';
    display_characters[1] = 'o';
    display_characters[2] = 'o';
    display_characters[3] = 'd';
    display_characters[4] = ' ';

// customize welcome message based on current time
    if ( ( timevalue >= 5 ) && ( timevalue < 12 ) )
    {
        display_characters[5] = 'm';
        display_characters[6] = 'o';
        display_characters[7] = 'r';
        display_characters[8] = 'n';
        display_characters[9] = 'i';
        display_characters[10] = 'n';
        display_characters[11] = 'g';
        display_characters[12] = ' ';
        display_characters[13] = ' ';
    }
    else if ( ( timevalue >= 12 ) && ( timevalue < 18 ) )
    {
        display_characters[5] = 'a';
        display_characters[6] = 'f';
        display_characters[7] = 't';
        display_characters[8] = 'e';
        display_characters[9] = 'r';
        display_characters[10] = 'n';
        display_characters[11] = 'o';
        display_characters[12] = 'o';
        display_characters[13] = 'n';
    }
    else
        {
            display_characters[5] = 'e';
            display_characters[6] = 'v';
            display_characters[7] = 'e';
            display_characters[8] = 'n';
            display_characters[9] = 'i';
            display_characters[10] = 'n';
            display_characters[11] = 'g';
            display_characters[12] = ' ';
            display_characters[13] = ' ';
        }

// populate remining display characters with spaces
    display_characters[14] = ' ';        
    display_characters[15] = ' ';        
    display_characters[16] = ' ';        
    display_characters[17] = ' ';        
    display_characters[18] = ' ';        
    display_characters[19] = ' ';       
    display_characters[20] = ' ';        
    display_characters[21] = ' ';        
    
    return 0x1;
}



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
uint8 check_atomic_clock(void)
{
//variables used to store clock and data info
    uint32 time_value = 0u;
    uint8 hour_value = 0u;
    uint8 minute_value = 0u;
// structure to store returnd DCF data    
    DCF_datetime *DCF_return;
    
// read back time info from internal RTC block
    time_value = Int_RTC_GetTime();
// convert current hour (in BCD code) to simple number    
    hour_value = (((time_value >> 20) & 0x03) * 10 ) + ((time_value >> 16) & 0x0F);

// check hour and clear Atomic LED at 2.59 at night indicating process to re-start 
// atomic time check    
    if ( hour_value == 2 )
    {
// convert current minute (in BCD code) to simple number    
        minute_value = (((time_value >> 12) & 0x07) * 10 ) + ((time_value >> 8) & 0x0F);
        if ( minute_value == 59 )
        {   
// check if Atomic LED is ON        
// if Atomic LED is ON, switch it OFF to enable checking and updatign procedures
            if ((LED_state & 0x4000) == 0x4000)
            {
// disable/switch_off timer off LED
                LED_state = LED_state & 0xBFFF;
                Set_I2C_LED_Driver(LED_state); 
            }
// update intenral RTC module with info stored in external battery assisted RTC module
// this ensures that at least one in 24 hour intenrla RTC block is updated
            Clocks_control(0x03, NULL);
        }
    }
     
    
// run atomic time check only between 3.00 and 6.00 at night
    if ( (time_value > 3) && (time_value < 6) )
    {
// check with Atomic LED is OFF        
        if ((LED_state & 0x4000) == 0x0000)
        {
// if LED is OFF, check if DCF block is disabled, if it is disabled, enable it        
            if (DCF_block_activity == 0x00)
            {
// initialize DCF block        
                DCF_Control(0x01);  
            }
        }
    }
      
    
// check if DCF block is enabled, if it is start listenning to the radio signals
    if (DCF_block_activity == 0x01)
    {
// decode received DCF frame if returned frame is valid
        if ( DCF_decode(DCF_return) == 0x11)
        {
// map DCF structure output into table that will updated internal and external RTC blocks
// hours                
            modified_time_date[13] = ( ( DCF_return->h >> 4 ) & 0x0F );  
            modified_time_date[12] = ( DCF_return->h & 0x0F ); 
// minutes
            modified_time_date[11] = ( ( DCF_return->min >> 4 ) & 0x0F );  
            modified_time_date[10] = ( DCF_return->min & 0x0F ); 
// seconds - set to 0
            modified_time_date[9] = 0x00;  
            modified_time_date[8] = 0x00; 
// months                
            modified_time_date[7] = ( ( DCF_return->month >> 4 ) & 0x0F );  
            modified_time_date[6] = ( DCF_return->month & 0x0F ); 
// days
            modified_time_date[5] = ( ( DCF_return->day >> 4 ) & 0x0F );  
            modified_time_date[4] = ( DCF_return->day & 0x0F ); 
// years
            modified_time_date[3] = ( ( DCF_return->year >> 12 ) & 0x0F );  
            modified_time_date[2] = ( ( DCF_return->year >> 8 ) & 0x0F );  
            modified_time_date[1] = ( ( DCF_return->year >> 4 ) & 0x0F );  
            modified_time_date[0] = ( DCF_return->year & 0x0F ); 
// set values for clock and data based on latest received atomic clock data                
            Clocks_control(0x02, modified_time_date);
// enable/switch_on timer off LED
            LED_state = LED_state | 0x4000; 
            Set_I2C_LED_Driver(LED_state); 
// check if DCF block is enabled, if it is disable it after clock update       
            if (DCF_block_activity == 0x01)
            {
// disable DCF block        
                DCF_Control(0x02);  
            }
        }
    }
    
    return 0x01;
}


/* [] END OF FILE */

