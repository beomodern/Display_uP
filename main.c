/*******************************************************************************
* File Name: main.c
*
* Copyright HEMI, 2021
* All Rights Reserved
* UNPUBLISHED, LICENSED SOFTWARE.
*
* Description:
*  This project is intended to be used as code that runs on PSoC KIT
*  inside BeoModern system in display module. 
*  It is responsible for monitoring
*  - SPI interface from main PSoC
*  - photoresistor level that represents ambient brightness
*  It is responsible for controling 
*  - Informaiton presented on 24 digit LED alphanumeric display modules
*  - I2C interface to LED driver
*  - RTC module
*  - DCF module
*
*******************************************************************************/

#include <project.h>
#include <display.h>
#include <inputs.h>
#include <operate.h>

// variable used in display brightness testing
    uint8 local_display_brightness = 0;
// table used to store display characters
    uint8 *display_data = NULL;
// initialize variable that stores SPI return data 
// set it to 0 for normal operation, set to 0x5A will shut down whole system
    uint8 display_SPI_message;
// semafor to spread activity between SPI read + display write procedures
    uint8 activity_semafor;  
    
int main()
{   
// Initialize nubmer of variables at display module start up    
// reset activity semafor indicating start with brightness check and update
    activity_semafor = 0;
// reset timer off time
    timer_off_time = 0x00000000;
// clear flag set by SPI activity 
    SPI_flag = 0;
// clear flag set by 200mS timer 
    mS200_flag = 0;    
// reset auto-off timer settings to 0
    auto_off_timer_new_settings = 0x00;
// set display status to 0 - normal operation set to 0x5A will shut down whole system
    display_SPI_message = 0x00;
// set LED stat a variable to all LEDs OFF
    LED_state = 0x0000;
// clock message timer reset to zero.
    clock_message_timer = 0u; 
// set position of modified digit to -1 - normal clock display
    position_of_modified_digit = -1;
// Set initial brightness levels to medium level    
    Disp_Brightness = 0x03;
// disable DCF block
    DCF_block_activity = 0x00;    

// initialize nubmer of blocks at display module start up    
// display and LED brightness - Start Opamp component
    Opamp_Start();
// display and LED brightness - Start ADC SAR component
    ADC_SAR_Start();  
// Starts Periodic timer (used for display update)
    Periodic_Timer_Start();
// Enable Timer interrupt and set up handler for it
    Timer_INT_StartEx(Periodic_Timer_INTHandler);    
// Connects user made function to interrupt
    SPIS_SetCustomInterruptHandler(*Slave_ISR);
// communicaiton with main PSoC - Start SCB (SPI mode) operation 
    SPIS_Start();
// Enable interrupt routines
    CyGlobalIntEnable;
// LED communication - Start SCB (I2C Master mode) operation 
    I2C_M_Start();    
// Starts SysTick component - used by internal RTC
    CySysTickStart();
// Configure SysTick timer to generate interrupt every 100 ms 
// RTC operating of system clock (24MHz) - used by internal RTC   
    CySysTickSetReload(CYDEV_BCLK__SYSCLK__HZ / 10);
// Associated RTC call back function with the SysTick interrupt nubmer 0   
// used by internal RTC    
    CySysTickSetCallback(0, SysTickIsrHandler);
// Start internal RTC
    Int_RTC_Start();
// Set period for internal RTC to 1 second update  
    Int_RTC_SetPeriod(1u, 10u);
// funciton that pulls current time and date from external battery-assisted
// RTC module and loads that into internal RTC block
    Clocks_control(0x03, NULL);
// LED communication - Initialize I2C LED driver (PCA9685)    
    Initialize_I2C_LED_Driver();
// RESET alphanumeric displays        
    ControlAlphaNumDisplay(0x80000000);

// inifite loop where display module operates within        
    for(;;)
    {
// Check if SPI interface set interrupt (indicating new data is available)
        if (SPI_flag == 1)
        {
// Check if new data is available
            if (SPIS_WaitForCommand(InputSPIBuffer, display_SPI_message) == 1)
            {
// Put data into the slave TX buffer to be transferred while following
// master access. Read SPI message and load it into Rx buffer
                display_data = SPI_message_reader(InputSPIBuffer, display_data);
// check if clock flag is set, if it is display is updated by separate function                
                if (clock_flag == 0x00)
                {
                   
// REMOVE REMOVE REMOVE
// for display brigtness testing                    
//                    display_data[0] = '0' + local_display_brightness;
// REMOVE REMOVE REMOVE
                    
// if clock flag is cleared, display data is printed on alphanumeric display                    
                    Data_to_alphanumeric(display_data);
                }
            }
// clear flag set by interrupt            
            SPI_flag = 0;
        }
        else
            {
// Check if 200mS flag is set - indicating interrupt from 200mS timer was triggered
                if (mS200_flag == 1)
                {    
                    
// if activity semafor <= 1 check brightness and adjust display+LEDs accordingly
                    if (activity_semafor <= 1 )
                    {
// check ADC readouts to see if brightness changed  
                        Check_brightness();
// check change in display brightness variable        
                        if (Disp_Brightness != local_display_brightness)
                        {
// if there is a change (change in brightness detected) set alphanumeric brightness           
                            ControlAlphaNumDisplay( ( (Disp_Brightness+1) << 24 ) & 0x07000000 );
// refresh LED driver brighness with new settings            
                            Set_I2C_LED_Driver(LED_state); 
// set new brightness value into local variable
                            local_display_brightness = Disp_Brightness;
                        }
// set activity semafor to 2 - check timer
                        activity_semafor = 2;
                    }

// if activity semafor = 2 check if off timer is set. If it is check if programmed off time elapsed
// if it did, sent message of SPI to main PSoC to power down the system            
                    if (activity_semafor == 2)
                    {
// check if timer off is set        
                        if ( timer_off_time != 0x00000000 )
                        {
// if it is read back current time and date in UNIX format and compare results to set
// off time. If read back value exceeds re-set value, return back over SPI message
// 0x5A indicating request to shutdown whole system
                            if ( Int_RTC_DateTimeToUnix( Int_RTC_GetDate(), Int_RTC_GetTime()) >= timer_off_time )
                            {
// set SPI message back to main PSoC to 0x5A indicatign request to shutdown whole system
                                display_SPI_message = 0x5A; 
                            }
                        }
// set activity semafor to 3 - DCF clock check               
                        activity_semafor = 3;
                    }

// if activity semafor = 3 check if atomic clock is being received and if so, update internal and external
// RTC blocks with newly received data. Then clear semafor back to 0.            
                    if (activity_semafor >=3)
                    {
// check DCF data (beteeen 3:00 and 6:00) and when vlaid data is received, update 
// internal and external RTC modules
                        check_atomic_clock();
// clear semafor back to first task (brightness check)
                        activity_semafor = 1;
                    }
            
// reset 200mS flag            
                    mS200_flag = 0;
                }
            }
                
                
// when clock is selected display is updated more often than based on SPI interrupt
// below if statement check if clock flag is set and if it is updated display more         
// frequently. Display simple clock when flag is set to 1        
        if (clock_flag == 0x01)
        {
// read display data from display time funciton
            Display_time(0x01, display_data);
// print time on display            
            Data_to_alphanumeric(display_data);
        }
// display clock + date when flag is set to 2        
        else if (clock_flag == 0x02)
        {
// read display data from display time funciton            
            Display_time(0x02, display_data);
// print time on display
            Data_to_alphanumeric(display_data);
        }
      
        
// TO BE REMOVED TO BE REMOVED        
// Enters bootload state when button on PSoC board is pressed
        if (DISP_RD_Read() == 0x00)        
        {
            Bootloadable_1_Load();
        }               
// TO BE REMOVED TO BE REMOVED
    }
}


/* [] END OF FILE */
