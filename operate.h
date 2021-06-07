/* ========================================
 *
 * Copyright HEMI, 2021
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 *  This project is intended to be used as code that runs on PSoC KIT
 *  inside BeoModern system in display module. 
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef OPERATE_H_   /* Include guard */
#define OPERATE_H_

#include <project.h>

// variable to store informaiton if DCF block is active (0x01) or disabled (0x00)
uint8 DCF_block_activity;    
// variable to store DCF synchfronizaiton flag
uint8 DCF_sync;
// variable to store DCF bit value
uint8 DCF_bit;

// structure to store decoded date and time
typedef struct DCF_dt {
			uint8 min;
			uint8 h;
			uint8 month;
			uint8 day;
			uint16 year;
} DCF_datetime;


// Variable to store current brightness level
// 0 = brighest (100%)
// :
// 6 = darkest (<40%)    
uint8 Disp_Brightness; 
    
/***************************************
*            Brightness control
****************************************/
// Brightness treshold levels in 7 steps
#define B_MIN (0u)       //   0%
#define B_L1 (1638u)     //  40%
#define B_L2 (2457u)     //  60%
#define B_L3 (2966u)     //  70%
#define B_L4 (3153u)     //  77%
#define B_L5 (3480u)     //  85%
#define B_L6 (3767u)     //  92%
#define B_MAX (4095u)    // 100%

// Brightness hysteresis
#define B_HYSTERESIS (120u)  // 3%
    
/***************************************
*            Constants
****************************************/
// SPI Interface
// Packet size
#define PACKET_SIZE      (28u)

// Byte position within the packet (start, end)
#define PACKET_SOP_POS  (0u)
#define PACKET_CTRL1_POS  (25u)
#define PACKET_CTRL2_POS  (26u)
#define PACKET_EOP_POS  (27u)
// Byte position within the packet (start, status, end)
#define PACKET_STS_POS  (1u)

// Start and end of the packet markers/
#define PACKET_SOP      (0x08u)
#define PACKET_EOP      (0x17u)    
     
// This dummy buffer used by SPIS when it receives command packet.
uint8 TxBuffer[PACKET_SIZE];    
uint8 tmpBuffer[PACKET_SIZE];
uint8 InputSPIBuffer[PACKET_SIZE];


//I2C Interface
// I2C Slave address to communicate with 
#define I2C_SLAVE_ADDR      (0x40u)

// Buffer and packet size 
#define I2C_BUFFER_SIZE         (2u)
#define I2C_PACKET_SIZE         (I2C_BUFFER_SIZE)

// Packet positions 
#define I2C_ADR_POS      (0u)
#define I2C_DATA_POS      (1u)

// Transfer statuses 
#define I2C_TRANSFER_CMPLT      (0x00u)
#define I2C_TRANSFER_ERROR      (0xABu)





/*******************************************************************************
* Function Name: SPIS_WaitForCommand
********************************************************************************
* Summary:
*  SPIS waits for completion of the transfer initiated by the SPIM. After packet
*  reception the format of the packet is verified and the command is returned.
*  If the format of the packet is invalid the unknown command is returned.
*  While receiving command from master, slave reports back its status on 
*  bajt 1. Message = 0x5A, indicates entering power down state. 
*
* Parameters:
*  None
*
* Return:
*  Returns pointer to tabele with 26 received bajts.
*
*******************************************************************************/
uint8 SPIS_WaitForCommand(uint8 *InputSPIBuffer, uint8 status);
 


/*******************************************************************************
* Function Name: I2C_WriteCommandPacket
********************************************************************************
* 
* Summary:
*  Master initiates the transfer to write command packet into the slave.
*  The status of the transfer is returned.
*
* Parameters:
*  adr  - I2C address to which command needs to be sett
*  data - dat athat needs to be sent to specified address
*
* Return:
*  Status of the data transfer. There are two statuses
*  - TRANSFER_CMPLT: transfer completed successfully.
*  - TRANSFER_ERROR: the error occurred while transfer.
*
*******************************************************************************/
uint32 I2C_WriteCommandPacket(uint8 adr, uint8 data);



/*******************************************************************************
* Function Name: I2C_ReadStatusPacket
********************************************************************************
* 
* Summary:
*  Master initiates the transfer to read status registers from I2C slave.
*
* Parameters:
*  adr - address from which data needs to be read back. 
*
* Return:
*  read back data
*
*******************************************************************************/
uint8 I2C_ReadStatusPacket(uint8 adr);



/*******************************************************************************
* Function Name: Initialize_I2C_LED_Driver(void)
********************************************************************************
* 
* Summary:
*  This funciton is responsible for setting up I2C LED driver (PCA9685). 
*  1. It enables an on-board oscillator and use internal clock. 
*  2. It use totem-pole structure for LED drivers. 
*  3. It can sets pre-scaler frequeny to 400Hz refresh rate (default 200Hz is used)
*  4. Finally it sets OE pin to 0x00 which means outputs are ON
*
* Parameters:
*  no input parameters
*
* Return:
*  0x01 - fixed value of 1.
*
*******************************************************************************/
uint8 Initialize_I2C_LED_Driver(void);



/*******************************************************************************
* Function Name: Set_I2C_LED_Driver(uint16 configure)
********************************************************************************
* 
* Summary:
*  This funciton is responsible for turning ON and OFF LEDs connected to 
*  I2C LED driver (PCA9685). It takes as a paremater 16 bit word. 
*  Bits in that word set to 1 represents LED ON. Bits in that word set to 0 
*  represents LED OFF. Brightness of ON LEDs is determined by value of global 
*  variable Disp_Brightness where:
*       0 = brighest (between 85% and 100% from optoresistor SAR ADC)
*       :
*       6 = darkest (between 0% and 14% from optoresistor SAR ADC)
*
* Parameters:
*  configure - 16 bit word. Bits in that word set to 1 represnets LED ON.
*                           Bits in that word set to 0 represents LED OFF.
*
* Return:
*  0x01 - fixed value of 1.
*
*******************************************************************************/
uint8 Set_I2C_LED_Driver(uint16 configure);



/*******************************************************************************
* Function Name: Check_brightness(void)
********************************************************************************
* Summary:
*  Function checks SAR ADC value. Its value coresponds to voltage across 
*  photo-resistor. Value measured by SAR ADC is then mapped to Disp_Brightness
*  variable that is used to control LED display brightness. Hysteresis is 
*  implemented to avoid flickerign of LED brightness when value is on the transition
*  border between brightness bands.
*
*  Brightness treshold levels in 7 equal steps
*  B_MIN (0u)       //   0%
*  B_L1 (1638u)     //  40%
*  B_L2 (2457u)     //  60%
*  B_L3 (2966u)     //  70%
*  B_L4 (3153u)     //  77%
*  B_L5 (3480u)     //  85%
*  B_L6 (3767u)     //  92%
*  B_MAX (4095u)    // 100%
*
*  Brightness hysteresis
*  B_HYSTERESIS (61u)  // 1,5%
*
* Return:
*  None
*  sets value of globaal variable Disp_Brightness where:
*  0 = brighest (between 92% and 100% from optoresistor SAR ADC)
*  :
*  6 = darkest (between 0% and 40% from optoresistor SAR ADC)
*
*******************************************************************************/
uint8 Check_brightness(void);



/*******************************************************************************
* Function Name: ReadRTC
********************************************************************************
* 
* Summary:
*  Function that reads data from RTC module. Pin assignment for this funciton is
*    DATA = P3.4
*    CLOCK = P3.5
*    CS = P0.6
*  Addresses that are valid to be read are
*    0x81 = Seconds
*    0x83 = Minutes
*    0x85 = Hours
*    0x87 = Day
*    0x89 = Month
*    0x0B = Week day
*    0x8D = Year
*
* Parameters:
*  adr - address to be read for. Funciton does not check if address is within 
*    valid range
*
* Return:
*  val - value stored in read back register. Refer to DS1302 for details
*
*******************************************************************************/
uint8 ReadRTC(uint8 adr);




/*******************************************************************************
* Function Name: WriteRTC(uint8 adr, uint8 data)
********************************************************************************
* 
* Summary:
*  Function that writes data into RTC module. Pin assignment for this funciton is
*    DATA = P3.4
*    CLOCK = P3.5
*    CS = P0.6
*  Addresses that are valid to be written to are
*    0x80 = Seconds
*    0x82 = Minutes
*    0x84 = Hours
*    0x86 = Day
*    0x88 = Month
*    0x0A = Week day
*    0x8C = Year
*    0x8E = write protect
*    0x90 = trickle charge
*
* Parameters:
*  adr - address to be written to. Funciton does not check if address is within 
*    valid range
*  data - data to be written into selcted address
*
* Return:
*  val - value stored in read back register. Refer to DS1302 for details
*
*******************************************************************************/
uint8 WriteRTC(uint8 adr, uint8 data);



/*******************************************************************************
* Function Name: Ext_RTC_SetDateAndTime
****************************************************************************//**
*
* Sets the time and date values in external RTC module as the current time and date.
*
* \param inputTime
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
* \param inputDate
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
*******************************************************************************/
void Ext_RTC_SetDateAndTime(uint32 inputTime, uint32 inputDate);



/*******************************************************************************
* Function Name: Ext_RTC_GetTime
****************************************************************************//**
*
* Reads the current time from external RTC module.
*
* \return
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
*******************************************************************************/
uint32 Ext_RTC_GetTime(void);



/*******************************************************************************
* Function Name: Ext_RTC_GetDate
****************************************************************************//**
*
* Reads the current date from external RTC.
*
* \return
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
*
*******************************************************************************/
uint32 Ext_RTC_GetDate(void);



/*******************************************************************************
* Function Name: ReadAlphaNumDisplay(uint8 adr, uint8 disp_sel)
********************************************************************************
* 
* Summary:
*  Function that reads data from selected LED Alphanumeric display. 
*  Pin assignment for this funciton are as follow:
*    DATA = Display_DATA (register build of 8 GPIO data lines)
*    ADDRESS = Display_ADR (register build of 6 GPIO - FL + 5 Address lines)
*    CS1 = P0.0
*    CS2 = P0.1
*    CS3 = P0.2
*    /WR = P2.6
*    /RD = P0.7
*    /RST = P4.2
*  Addresses that are valid to be written to are:
*    0x000x xnnn = Flash RAM (nnn - address of digit to flash)
*    0x0010 0xxx = UDC Address Register (character address on data lines)
*    0x0010 1nnn = UDS RAM (nnn - row address to write to)
*    0x0011 0xxx = Control Word Register
*    0x0011 1nnn = ASCII Character RAM (character address)
*
* Parameters:
*   adr
*      0x000x xnnn = Flash RAM (nnn - address of digit to flash)
*      0x0010 0xxx = UDC Address Register (character address on data lines)
*      0x0010 1nnn = UDS RAM (nnn - row address to write to)
*      0x0011 0xxx = Control Word Register
*      0x0011 1nnn = ASCII Character RAM (character address)      
*   disp_sel 
*      bit.1 -> display 1 (CE1)
*      bit.2 -> display 2 (CE2)
*      bit.3 -> display 3 (CE3)
*
* Return:
*      valRet - value read back from 8-bit wide display data bus
*
*******************************************************************************/
uint8 ReadAlphaNumDisplay(uint8 adr, uint8 disp_sel);




/*******************************************************************************
* Function Name: WriteAlphaNumDisplay(uint8 adr, uint8 data, uint8 disp_sel)
********************************************************************************
* 
* Summary:
*  Function that writes to LED Alphanumeric display. Single function is used to 
*  write to user defined characters and to characters RAM. Pin assignment for 
*  this funciton are as follow:
*    DATA = Display_DATA (register build of 8 GPIO data lines)
*    ADDRESS = Display_ADR (register build of 6 GPIO - FL + 5 Address lines)
*    CS1 = P0.0
*    CS2 = P0.1
*    CS3 = P0.2
*    /WR = P2.6
*    /RD = P0.7
*    /RST = P4.2
*  Addresses that are valid to be written to are:
*    0x000x xnnn = Flash RAM (nnn - address of digit to flash)
*    0x0010 0xxx = UDC Address Register (character address on data lines)
*    0x0010 1nnn = UDS RAM (nnn - row address to write to)
*    0x0011 0xxx = Control Word Register
*    0x0011 1nnn = ASCII Character RAM (character address)
*
* Parameters:
*   adr
*      0x0010 0xxx = UDC Address Register (character address on data lines)
*      0x0010 1nnn = UDS RAM (nnn - row address to write to)
*      0x0011 0xxx = Control Word Register
*      0x0011 1nnn = ASCII Character RAM (character address)      
*   data
*      8 bit data that is sent to display on 8-bit wide parralel bus
*   disp_sel 
*      bit.1 -> display 1 (CE1)
*      bit.2 -> display 2 (CE2)
*      bit.3 -> display 3 (CE3)
*
* Return:
*      0x00 - incorrect command 
*      0x01 - correct command and command executed
*
*******************************************************************************/
uint8 WriteAlphaNumDisplay(uint8 adr, uint8 data, uint8 disp_sel);



/*******************************************************************************
* Function Name: ControlAlphaNumDisplay(uint8 command)
********************************************************************************
* 
* Summary:
*  Function that controls LED Alphanumeric display. Single function is used to 
*  control brightness and reset all displays at the same time. Pin assignment for 
*  this funciton are as follow:
*    DATA = Display_DATA (register build of 8 GPIO data lines)
*    ADDRESS = Display_ADR (register build of 6 GPIO - FL + 5 Address lines)
*    CS1 = P0.0
*    CS2 = P0.1
*    CS3 = P0.2
*    /WR = P2.6
*    /RD = P0.7
*    /RST = P4.2
*  Addresses that are valid to be written to are:
*    0x000x xnnn = Flash RAM (nnn - address of digit to flash)
*    0x0010 0xxx = UDC Address Register (character address on data lines)
*    0x0010 1nnn = UDS RAM (nnn - row address to write to)
*    0x0011 0xxx = Control Word Register
*    0x0011 1nnn = ASCII Character RAM (character address)
*
* Parameters:
*  command - it is a 32 bit command where each bit has its representaiton
*     0x80 00 00 00 - reset all 3 display modules
*     0x08 00 00 00 - when set display flasing is enabled. 
*                     when reset, display flashing is disabled
*     0x07 00 00 00 - 3 bits that represents display brightness. Values are 
*                       0x00 - brightness = 100%
*                       0x01 - brightness =  80%
*                       0x02 - brightness =  53%
*                       0x03 - brightness =  40%
*                       0x04 - brightness =  27%
*                       0x05 - brightness =  20%
*                       0x06 - brightness =  13%
*                       0x07 - brightness =   0%
*     0x80 FF FF FF - 24 bits that represents flashing status of display
*                      when bit it set its coresponding digit will flash
*                      when bit it cleared its coresponding digit will operate normaly
*
* Return:
*     0x00 - incorrect command 
*     0x01 - correct command and command executed
*
*******************************************************************************/
uint8 ControlAlphaNumDisplay(uint32 command);



/*******************************************************************************
* Function Name: LoadAlphaNumDisplay(uint8 command)
********************************************************************************
* 
* Summary:
*  Function 
*
* Parameters:
*  command 
*     0x01 - load signal level characters
*       location 0x0 = blank display
*       location 0x1 = bottom (right channel) empty square, top blank
*       location 0x2 = bottom blank, top (left channel) empty square
*       location 0x3 = both bottom and top (left and right channels) empty squares        
*       location 0x5 = bottom (right channel) full square, top blank
*       location 0x6 = bottom blank, top (left channel) full square
*       location 0x7 = both bottom and top (left and right channels) full squares        
*       location 0x8 = arrow left
*       location 0x9 = arrow right
*     0x02 - load spectrum analyzer characters - envelope mode
*       location 0x0 - ASCI 0 -> blank with one bottom dot (in the middle) ON
*       location 0x1 - ASCI 1 -> 3 bottom dots ON
*       location 0x2 - ASCI 2 -> 5 bottom dots ON
*       location 0x3 - ASCI 3 -> 5 bottom dots and 3 dots in 2nd line ON
*       location 0x4 - ASCI 4 -> 5 bottom dots and 5 dots in 2nd line ON
*       location 0x5 - ASCI 5 -> …
*
*       location 0xF - ASCI > -> full array of 7 x 5 dots ON
*     0x03 - load spectrum analyzer characters - level mode
*       location 0x0 - ASCI 0 -> blank 
*       location 0x1 - ASCI 1 -> 5 bottom dots ON
*       location 0x2 - ASCI 2 -> 5 bottom dots ON
*       location 0x3 - ASCI 3 -> 5 bottom dots and 5 dots in 2nd line ON
*       location 0x4 - ASCI 4 -> 5 bottom dots and 5 dots in 2nd line ON
*       location 0x5 - ASCI 5 -> …
*
*       location 0xF - ASCI > -> full array of 7 x 5 dots ON
*
* Return:
*     0x00 - incorrect command 
*     0x01 - correct command and command executed
*
*******************************************************************************/
uint8 LoadAlphaNumDisplay(uint8 command);





/*******************************************************************************
* Function Name: CharacterToAlphaNumDisplay(uint8 character, uint8 position)
********************************************************************************
* 
* Summary:
*  Function 
*
* Parameters:
*  character: 
*     0x00 - 0x7F - ASCI caracters from internal table
*     0x80 - 0x8F - 16 user defined characters
*  position:
*       0u -> most right character on display module (CS3)
*      23u -> most left character on display module (CS1)
*
* Return:
*     0x00 - incorrect command 
*     0x01 - correct command and command executed
*
*******************************************************************************/
uint8 CharacterToAlphaNumDisplay(uint8 character, uint8 position);



/*******************************************************************************
* Function Name: DCF_Control()
********************************************************************************
* 
* Summary:
*  Function starts or stops DCF timer and its clock source as well as 
*  resets DCF enviroment variables to its init state.
*  NEEDS TO BE DEBUGGED WHEN DCS SIGNAL IS AVAILABLE
*
* Parameters:
*     mode = 0x01 - starts DCF timer block
*            0x02 - stops DCF timer block
*
* Return:
*     mode value if called with correct mode state
*     0 if called with incorrect state
*
*******************************************************************************/
uint8 DCF_Control(uint8 mode);



/*******************************************************************************
* Function Name: DCF_decode(DCF_datetime *DCF_dt)
********************************************************************************
* 
* Summary:
*  Function that decodes DCF signal and returns structure with reeived time and 
*  date
*  NEEDS TO BE DEBUGGED WHEN DCS SIGNAL IS AVAILABLE
*
* Parameters:
*     DCF_datetime *DCF_dt - returns structure with received data and time 
*         from DCF
*
* Return:
*     1 - when decode finished with success
*     0 - when decode finished with incorrect result or input data was invalid
*
*******************************************************************************/
uint8 DCF_decode(DCF_datetime *DCF_dt);


#endif // OPERATE_H_

/* [] END OF FILE */
