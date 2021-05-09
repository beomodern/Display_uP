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

#include <project.h>
#include <inputs.h>
#include <operate.h>


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
uint8 SPIS_WaitForCommand(uint8 *InputSPIBuffer, uint8 status)

{
// variable used when scanning input table with new SPI data
     uint8 i;
// prepare data to be sent
    uint8 sTxBuffer[PACKET_SIZE] = {0};
// set start of Tx message 
    sTxBuffer[PACKET_SOP_POS] = (PACKET_SOP);
// put new 8 bit data into table at position 01 to be sent back to main uP
    sTxBuffer[PACKET_STS_POS] = (uint8) status;
// set end of Tx message    
    sTxBuffer[PACKET_EOP_POS] = (PACKET_EOP);

// Put data into the slave TX buffer to be transferred while following
// master access.
    SPIS_SpiUartPutArray(sTxBuffer, PACKET_SIZE);
        
// Wait for the end of the transfer 
    while (PACKET_SIZE != SPIS_SpiUartGetRxBufferSize());

// Read packet from the buffer 
    i = 0u;
    while (0u != SPIS_SpiUartGetRxBufferSize())
    {
        tmpBuffer[i] = (uint8) SPIS_SpiUartReadRxData();
        i++;
    }   

// Clear Tx buffer
    SPIS_SpiUartClearTxBuffer();
    
// Clear RX buffer
    SPIS_SpiUartClearRxBuffer();

// Put data again into the slave TX buffer to be transferred while following
// master access. Repeated operation needed for correct operation of SPI interface.
    SPIS_SpiUartPutArray(sTxBuffer, PACKET_SIZE);
    
// Check start and end of packet markers
    if ((tmpBuffer[PACKET_SOP_POS] == PACKET_SOP) &&
        (tmpBuffer[PACKET_EOP_POS] == PACKET_EOP))
    {
// Return table stripped out of start and stop markers
// data only is returned 24 characters plus 2 control words
        for (i = 1; i<PACKET_SIZE; i++)
        {
            InputSPIBuffer[i-1] = (tmpBuffer[i]);
        }
        return 1;
    }
// Incorrect packet format or not data - return 0
    else
        {
            return 0;        
        }
}


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
uint32 I2C_WriteCommandPacket(uint8 adr, uint8 data)
{
    uint8  buffer[I2C_BUFFER_SIZE];
    uint32 status = I2C_TRANSFER_ERROR;

// Initialize buffer with packet 
    buffer[I2C_ADR_POS] = adr;
    buffer[I2C_DATA_POS] = data;
    

    (void) I2C_M_I2CMasterClearStatus();
    
// Start I2C write and check status
    if(I2C_M_I2C_MSTR_NO_ERROR == I2C_M_I2CMasterWriteBuf(I2C_SLAVE_ADDR,
                                    buffer, I2C_PACKET_SIZE,
                                    I2C_M_I2C_MODE_COMPLETE_XFER))
    {
// If I2C write started without errors, 
// wait until I2C Master completes write transfer 
        while (0u == (I2C_M_I2CMasterStatus() & I2C_M_I2C_MSTAT_WR_CMPLT))
        {
// Wait 
        }
        
// Display transfer status 
        if (0u == (I2C_M_I2CMasterStatus() & I2C_M_I2C_MSTAT_ERR_XFER))
        {
// Check if all bytes was written
            if (I2C_M_I2CMasterGetWriteBufSize() == I2C_BUFFER_SIZE)
            {
                status = I2C_TRANSFER_CMPLT;
            }
        }
    }

    return (status);
}




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
uint8 I2C_ReadStatusPacket(uint8 adr)
{
    uint8  buffer[I2C_BUFFER_SIZE];
    uint8 status = 0x00;
    
    buffer[I2C_ADR_POS] = adr;
    
    (void) I2C_M_I2CMasterClearStatus();
    
    if(I2C_M_I2C_MSTR_NO_ERROR ==  I2C_M_I2CMasterReadBuf(I2C_SLAVE_ADDR,
                                    buffer, I2C_PACKET_SIZE,
                                    I2C_M_I2C_MODE_COMPLETE_XFER))
    {
// If I2C read started without errors, 
// wait until master complete read transfer
        while (0u == (I2C_M_I2CMasterStatus() & I2C_M_I2C_MSTAT_RD_CMPLT))
        {
// Wait 
        }
        
// Display transfer status
        if (0u == (I2C_M_I2C_MSTAT_ERR_XFER & I2C_M_I2CMasterStatus()))
        {
// Check packet structure 
            if ((I2C_M_I2CMasterGetReadBufSize() == I2C_BUFFER_SIZE) )
            {
                status = buffer[I2C_DATA_POS];
            }
        }
    }

    return (status);
}





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
uint8 Initialize_I2C_LED_Driver(void)
{
// LED driver initialization 
// enable on-board oscillator and use internal clock
            I2C_WriteCommandPacket(0x00, 0x00);
// enable on-board oscillator use totem-pole structure for LED drivers
            I2C_WriteCommandPacket(0x01, 0x04);
// set pre-scaler frequency to 400Hz -> 15 = 0x0F (default 200Hz -> 0x1E)
// default    I2C_WriteCommandPacket(0xFE, 0x1E);
//            I2C_WriteCommandPacket(0xFE, 0x0F);            
// set OE = 0x00 = ON
            LED_OE_Write(0x00);
    
    return 0x01;
}



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
uint8 Set_I2C_LED_Driver(uint16 configure)
{
// loop counter for loop that updates LED state and brightness
    uint8 loop_counter = 0x00;

// variables used to store LED addresses inside LED driver
    uint8 LED_ON_L_adr = 0x06;
    uint8 LED_ON_H_adr = 0x07;
    uint8 LED_OFF_L_adr = 0x08;
    uint8 LED_OFF_H_adr = 0x09;
    
// variables for LED when it is ON. LED is ON from the start
    uint8 LED_ON_L_val = 0x00;
    uint8 LED_ON_H_val = 0x00;

// variables for LED when it is ON. LED is OFF after indicated nubmer below
// brightness will be adjusted using switch function
    uint8 LED_OFF_L_val = 0x00;
    uint8 LED_OFF_H_val = 0x00;

// variables for LED when it is OFF bit 4 set to 1 switch LED completelly OFF
    uint8 LED_FULL_OFF_L = 0x00;
    uint8 LED_FULL_OFF_H = 0x10;
    
    switch (Disp_Brightness)
    {
// 6 = darkest - brightness = 10% = 4095 * 0.1 = 409 (0x199)
// 1% = OFF Time start -> 0x029          
    case (6):
        LED_OFF_H_val = 0x01;
        LED_OFF_L_val = 0x99;
        break;
// 5 = brightness = 20% = 4095 * 0.2 = 819 (0x333)     
// 2% = OFF Time start -> 0x051 
    case (5):
        LED_OFF_H_val = 0x03;
        LED_OFF_L_val = 0x33;
        break;
// 4 = brightness = 30% = 4095 * 0.3 = 1228 (0x4CC)
// 5% = OFF Time start -> 0x0CC      
    case (4):
        LED_OFF_H_val = 0x04;
        LED_OFF_L_val = 0xCC;
        break;
// 3 = brightness = 50% = 4095 * 0.5 = 2047 (0x7FF)    
// 10% = OFF Time start -> 0x199      
    case (3):
        LED_OFF_H_val = 0x07;
        LED_OFF_L_val = 0xFF;
        break;
// 2 = brightness = 70% = 4095 * 0.7 = 2866 (0xB32)  
// 30% = OFF Time start -> 0x4CC     
    case (2):
        LED_OFF_H_val = 0x0B;
        LED_OFF_L_val = 0x32;
        break;
// 1 = brightness = 80% = 4095 * 0.8 = 3276 (0xCCC)      
// 60% = OFF Time start -> 0x999    
    case (1):
        LED_OFF_H_val = 0x0C;
        LED_OFF_L_val = 0xCC;
        break;
// 0 = brighest - brightness = 100% = 4095 (0xFFF)  
// 100% = OFF Time start -> 0xFFF     
    case (0):
        LED_OFF_H_val = 0x0F;
        LED_OFF_L_val = 0xFF;
        break;
    }

    
/*    
    switch (Disp_Brightness)
    {
// 6 = darkest - brightness = 14% = 4095 * 0.14 = 573 (0x23D)
// 1% = OFF Time start -> 0x029          
    case (6):
        LED_OFF_H_val = 0x02;
        LED_OFF_L_val = 0x3D;
        break;
// 5 = brightness = 26% = 4095 * 0.26 = 1064 (0x428)     
// 2% = OFF Time start -> 0x051 
    case (5):
        LED_OFF_H_val = 0x04;
        LED_OFF_L_val = 0x28;
        break;
// 4 = brightness = 43% = 4095 * 0.43 = 1761 (0x6E1)
// 5% = OFF Time start -> 0x0CC      
    case (4):
        LED_OFF_H_val = 0x06;
        LED_OFF_L_val = 0xE1;
        break;
// 3 = brightness = 57% = 4095 * 0.57 = 2334 (0x91E)    
// 10% = OFF Time start -> 0x199      
    case (3):
        LED_OFF_H_val = 0x09;
        LED_OFF_L_val = 0x1E;
        break;
// 2 = brightness = 71% = 4095 * 0.71 = 2907 (0xB5B)  
// 30% = OFF Time start -> 0x4CC     
    case (2):
        LED_OFF_H_val = 0x0B;
        LED_OFF_L_val = 0x5B;
        break;
// 1 = brightness = 85% = 4095 * 0.85 = 3480 (0xD98)      
// 60% = OFF Time start -> 0x999    
    case (1):
        LED_OFF_H_val = 0x0D;
        LED_OFF_L_val = 0x98;
        break;
// 0 = brighest - brightness = 100% = 4095 (0xFFF)  
// 100% = OFF Time start -> 0xFFF     
    case (0):
        LED_OFF_H_val = 0x0F;
        LED_OFF_L_val = 0xFF;
        break;
    }
*/   

    
// Writing 16 x 4 I2C registers for LEDs brightness control
    for (loop_counter = 0; loop_counter <=15; loop_counter++)
    {
// check if LED associated with counter value is ON or OFF
        if ((configure & 0x0001) == 0x0001)
        {
// if LED is ON set brightness of active LED as per global brightness value settings
            I2C_WriteCommandPacket(LED_ON_L_adr, LED_ON_L_val);
            I2C_WriteCommandPacket(LED_ON_H_adr, LED_ON_H_val);
            I2C_WriteCommandPacket(LED_OFF_L_adr, LED_OFF_L_val);
            I2C_WriteCommandPacket(LED_OFF_H_adr, LED_OFF_H_val);
        }
        else 
            {
// if LED is OFF set brightness of active LED to OFF
                I2C_WriteCommandPacket(LED_ON_L_adr, LED_ON_L_val);
                I2C_WriteCommandPacket(LED_ON_H_adr, LED_ON_H_val);
                I2C_WriteCommandPacket(LED_OFF_L_adr, LED_FULL_OFF_L);
                I2C_WriteCommandPacket(LED_OFF_H_adr, LED_FULL_OFF_H);                
            }
// increase LED control addresses by 4 to move to next LED control           
        LED_ON_L_adr = LED_ON_L_adr + 0x04;
        LED_ON_H_adr = LED_ON_H_adr + 0x04;
        LED_OFF_L_adr = LED_OFF_L_adr + 0x04;
        LED_OFF_H_adr = LED_OFF_H_adr + 0x04;            
// shift right by one control word value while scanning it by for loop
        configure = configure >> 1;    
    }

    return 1;
}



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
uint8 Check_brightness(void)
{

// variable to store ADC value
    uint16 ADC_SAR_value = 0;   

// flag used to indicate that new reading is outside current band + hysteresis
    uint8 change_brightness = 0;
    
// Start ADC conversion
    ADC_SAR_StartConvert();
    
// check if ADC conversion has been complete
    while ( 0u == ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT) )
    {
    }

// read back current SAR ADC value
    ADC_SAR_value = ADC_SAR_GetResult16(0);

// Stop ADC conversion    
    ADC_SAR_StopConvert();
    
// Value is measured and then hysteresis is applied. If new value is outside of current 
// band plus hysteresis, flag is set.
    switch (Disp_Brightness)
    {
    case (0x06):
        if ( ADC_SAR_value < (B_L6 - B_HYSTERESIS) )
        {   
            change_brightness = 1;
        }
        break;
    case (0x05):
        if ( ( ADC_SAR_value < (B_L5 - B_HYSTERESIS) ) || ( (B_L6 + B_HYSTERESIS ) <= ADC_SAR_value ) )
        {   
            change_brightness = 1;
        }
        break;
    case (0x04):
        if ( ( ADC_SAR_value < (B_L4 - B_HYSTERESIS) ) || ( (B_L5 + B_HYSTERESIS ) <= ADC_SAR_value ) )
        {   
            change_brightness = 1;
        }
        break;
    case (0x03):
        if ( ( ADC_SAR_value < (B_L3 - B_HYSTERESIS) ) || ( (B_L4 + B_HYSTERESIS ) <= ADC_SAR_value ) )
        {   
            change_brightness = 1;
        }
        break;
    case (0x02):
        if ( ( ADC_SAR_value < (B_L2 - B_HYSTERESIS) ) || ( (B_L3 + B_HYSTERESIS ) <= ADC_SAR_value ) )
        {   
            change_brightness = 1;
        }
        break;
   case (0x01):
        if ( ( ADC_SAR_value < (B_L1 - B_HYSTERESIS) ) || ( (B_L2 + B_HYSTERESIS ) <= ADC_SAR_value ) )
        {   
            change_brightness = 1;
        }
        break;
    case (0x00):
        if ( (B_L1 + B_HYSTERESIS ) <= ADC_SAR_value )
        {   
            change_brightness = 1;
        }
        break;
    }

// when flag is set new display brightness is applied based on SAR ADC readback value
    
    if ( change_brightness == 1)
    {
        if ( ( B_L6 < ADC_SAR_value ) && ( ADC_SAR_value <= B_MAX ) )
        {   
            Disp_Brightness = 0x00;
        }
        else if ( ( B_L5 < ADC_SAR_value ) && ( ADC_SAR_value <= B_L6 ) )
        {   
            Disp_Brightness = 0x01;
        }
        else if ( ( B_L4 < ADC_SAR_value ) && ( ADC_SAR_value <= B_L5 ) )
        {   
            Disp_Brightness = 0x02;
        }
        else if ( ( B_L3 < ADC_SAR_value ) && ( ADC_SAR_value <= B_L4 ) )
        {   
            Disp_Brightness = 0x03;
        }
        else if ( ( B_L2 < ADC_SAR_value ) && ( ADC_SAR_value <= B_L3 ) )
        {   
            Disp_Brightness = 0x04;
        }
        else if ( ( B_L1 < ADC_SAR_value ) && ( ADC_SAR_value <= B_L2 ) )
        {   
            Disp_Brightness = 0x05;
        }
        else if ( ( B_MIN <= ADC_SAR_value ) && ( ADC_SAR_value <= B_L1 ) )
        {   
            Disp_Brightness = 0x06;
        }
    }
    
    return 0;
}




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
uint8 ReadRTC(uint8 adr)
{
// loop counter 
    uint8 lp_counter;
// value to store address 
    uint8 val;
// copy adr to local variabel
    val = adr;
// set RTC CS to high    
    RTC_RST_CS_Write(0x01);
// wait 1uS for CS to settle
    CyDelayUs(1);
//run thru 8 bits address that will be read
    for (lp_counter=0; lp_counter<8; lp_counter++) 
    {
// check if coresponding address bit is high        
        if (val & 1) 
        {
// set data DGPIO to high when corespondign bit is high            
            RTC_DATA_Write(0x01);
        }
        else
            {
// set data DGPIO to low when corespondign bit is low            
                RTC_DATA_Write(0x00);
            }
// shift value by 1            
        val >>= 1; 
// wait 1uS for Clock to settle
        CyDelayUs(1);
// set RTC Clock to high    
        RTC_CLK_Write(0x01);
// wait 1uS for Clock to settle
        CyDelayUs(1);
// set RTC Clock to low
        RTC_CLK_Write(0x00);
// wait 1uS for CS to settle
        CyDelayUs(1);  
    }

// release DGPIO that is assciaed with DATA line  
    RTC_DATA_Write(0x00);    
    
//run thru 8 bits address that will be read
    for (lp_counter=0; lp_counter<8; lp_counter++) 
    {
// wait 1uS for CS to settle
        CyDelayUs(1);
// shift value by 1            
        val >>= 1; 
        if (RTC_DATA_Read()) 
        {
            val |= 0x80;
        }
        else
            {
                val &= 0x7F;         
            }
        
// set RTC Clock to high    
        RTC_CLK_Write(0x01);
// wait 1uS for Clock to settle
        CyDelayUs(1);
// set RTC Clock to low
        RTC_CLK_Write(0x00);
// wait 1uS for CS to settle
        CyDelayUs(1);  
    }

// set data DGPIO to low 
    RTC_DATA_Write(0x00);

// set RTC CS to low
    RTC_RST_CS_Write(0x00);     

    return val;

}



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
uint8 WriteRTC(uint8 adr, uint8 data)
{
// loop counter 
    uint8 lp_counter;
// value to store address 
    uint8 val;
// copy adr to local variabel
    val = adr;
// set RTC CS to high    
    RTC_RST_CS_Write(0x01);
// wait 1uS for CS to settle
    CyDelayUs(1);
//run thru 8 bits address that will be read
    for (lp_counter=0; lp_counter<8; lp_counter++) 
    {
// check if coresponding address bit is high        
        if (val & 1) 
        {
// set data DGPIO to high when corespondign bit is high            
            RTC_DATA_Write(0x01);
        }
        else
            {
// set data DGPIO to low when corespondign bit is low            
                RTC_DATA_Write(0x00);
            }
// shift value by 1            
        val >>= 1; 
// wait 1uS for Clock to settle
        CyDelayUs(1);
// set RTC Clock to high    
        RTC_CLK_Write(0x01);
// wait 1uS for Clock to settle
        CyDelayUs(1);
// set RTC Clock to low
        RTC_CLK_Write(0x00);
// wait 1uS for CS to settle
        CyDelayUs(1);  
    }
//copy data to write to local variable
   val = data;
//run thru 8 bits data that will be written
    for (lp_counter=0; lp_counter<8; lp_counter++) 
    {
// check if coresponding address bit is high        
        if (val & 1) 
        {
// set data DGPIO to high when corespondign bit is high            
            RTC_DATA_Write(0x01);
        }
        else
            {
// set data DGPIO to low when corespondign bit is low            
                RTC_DATA_Write(0x00);
            }
// shift value by 1            
        val >>= 1; 
// wait 1uS for Clock to settle
        CyDelayUs(1);
// set RTC Clock to high    
        RTC_CLK_Write(0x01);
// wait 1uS for Clock to settle
        CyDelayUs(1);
// set RTC Clock to low
        RTC_CLK_Write(0x00);
// wait 1uS for CS to settle
        CyDelayUs(1);  
    }    

// set data DGPIO to low 
    RTC_DATA_Write(0x00);

// set RTC CS to low
    RTC_RST_CS_Write(0x00);     

    return 0x01;
}



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
void Ext_RTC_SetDateAndTime(uint32 inputTime, uint32 inputDate)
{
// disable write proteciton flag (bit 7 -> 0)
// other bits in that register are do not care (set ot 0)    
    WriteRTC(0x8E, 0x00);

// disable clock halt (bit 7 -> 0) 
// set 10ths of seconds (bits 6, 5, 4) in BCD
// set seconds (bits 3, 2, 1, 0) in BCD
    WriteRTC(0x80, (inputTime & 0x0000007F) );

// set 10ths of minutes (bits 6, 5, 4) in BCD
// set minutes (bits 3, 2, 1, 0) in BCD
    WriteRTC(0x82, ((inputTime & 0x00007F00) >> 8) );

// set hour format (bit7 -> 0 = 24 hour format)
// set 10ths of hours (bits 5, 4) in BCD
// set hours (bits 3, 2, 1, 0) in BCD    
    WriteRTC(0x84, ((inputTime & 0x003F0000) >> 16) );
    
// set 10ths of days (bits 5, 4) in BCD
// set days (bits 3, 2, 1, 0) in BCD    
    WriteRTC(0x86, ((inputDate & 0x003F0000) >> 16) );
    
// set 10ths of months (bits 4) in BCD
// set months (bits 3, 2, 1, 0) in BCD    
    WriteRTC(0x88, ((inputDate & 0x1F000000) >> 24) );
    
// set 10ths of years (bits 7, 6, 5, 4) in BCD
// set years (bits 3, 2, 1, 0) in BCD    
    WriteRTC(0x8C, (inputDate & 0x000000FF) );    
    
// set trickle charge to lowest value
// bits 7, 6, 5, 4 -> 1010 = enable
// bits 3, 2 -> 10 = 2 diodes
// bits 1. 0 -> 11 = 8kR    
    WriteRTC(0x90, 0xAB );  
    
}



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
uint32 Ext_RTC_GetTime(void)
{
    uint32 time;
    time = 0x00000000;

// ensure that hour format is set to 24 hour (bit7 -> 0)
// read 10ths of hours (bits 5, 4) in BCD
// read hours (bits 3, 2, 1, 0) in BCD    
    time = (ReadRTC(0x85) & 0x3F );
    
// read 10ths of minutes (bits 6, 5, 4) in BCD
// read minutes (bits 3, 2, 1, 0) in BCD
    time = (time << 8) | ReadRTC(0x83);

// ensure clock halt is disabled (bit 7 -> 0) 
// set 10ths of seconds (bits 6, 5, 4) in BCD
// set seconds (bits 3, 2, 1, 0) in BCD
    time = (time << 8) | (ReadRTC(0x81) & 0x7F);    

    return(time);
}



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
uint32 Ext_RTC_GetDate(void)
{
    uint32 date;

// read 10ths of months (bits 4) in BCD
// read months (bits 3, 2, 1, 0) in BCD    
    date = ( ReadRTC(0x89) & 0x0000001F );

// read 10ths of days (bits 5, 4) in BCD
// read days (bits 3, 2, 1, 0) in BCD    
    date = (date << 8 ) | ( ReadRTC(0x87) & 0x0000003F );
    
// read 10ths of years (bits 7, 6, 5, 4) in BCD
// read years (bits 3, 2, 1, 0) in BCD    
    date = (date << 16 ) | ReadRTC(0x8D);

// set year to 2000    
      date = date | 0x00002000;
    
    return(date);
}



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
uint8 ReadAlphaNumDisplay(uint8 adr, uint8 disp_sel)
{
// function return value
    uint8 valRet = 0x00;
// set address register to provided address bits
    Display_ADR_Write(adr & 0x3F);
// wait 1uS 
        CyDelayUs(1u);        
// select display modules, thru CS lines based on disp_sel variable
// disp_sel bit.1 -> display 1 (CE1)
    if ( (disp_sel & 0x02) == 0x02)
    {
        DISP_CE1_Write(0u);
    }    
// disp_sel bit.2 -> display 2 (CE2)
    if ( (disp_sel & 0x04) == 0x04)
    {
        DISP_CE2_Write(0u);
    }    
// disp_sel bit.3 -> display 3 (CE3)
    if ( (disp_sel & 0x08) == 0x08)
    {
        DISP_CE3_Write(0u);
    }    
// wait 1uS 
    CyDelayUs(1u); 
// set READ signal low for all 3 displays - enable read
    DISP_RD_Write(0u);
// wait 1uS 
    CyDelayUs(1u); 
// read back value from -bit wide display parralel bus        
    valRet = Display_DATA_Read();
// set READ signal high for all 3 displays - disable read
    DISP_RD_Write(1u);
// disable chip select lines for all 3 displays       
    DISP_CE1_Write(1u);
    DISP_CE2_Write(1u);
    DISP_CE3_Write(1u);
    
    return valRet;
}



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
uint8 WriteAlphaNumDisplay(uint8 adr, uint8 data, uint8 disp_sel)
{
// function return value
    uint8 valRet = 0x00;
// check if addres value points to UDC or ASCII characters map
    if ( (adr & 0x20) == 0x20)
    {
// set address register to provided address bits
        Display_ADR_Write(adr & 0x3F);
// set data register with display brightness informaiton
        Display_DATA_Write(data);
// wait 10uS 
        CyDelayUs(10u);        
// select display modules, thru CS lines based on disp_sel variable
// disp_sel bit.1 -> display 1 (CE1)
        if ( (disp_sel & 0x02) == 0x02)
        {
            DISP_CE1_Write(0u);
        }    
// disp_sel bit.2 -> display 2 (CE2)
        if ( (disp_sel & 0x04) == 0x04)
        {
            DISP_CE2_Write(0u);
        }    
// disp_sel bit.3 -> display 3 (CE3)
        if ( (disp_sel & 0x08) == 0x08)
        {
            DISP_CE3_Write(0u);
        }    
// wait 10uS 
        CyDelayUs(10u); 
// set WRITE signal low for all 3 displays - enable write
        DISP_WR_Write(0u);
// wait 10uS 
        CyDelayUs(10u); 
// set WRITE signal high for all 3 displays - disable write
        DISP_WR_Write(1u);
// disable chip select lines for all 3 displays       
        DISP_CE1_Write(1u);
        DISP_CE2_Write(1u);
        DISP_CE3_Write(1u);
// set return value to 1
        valRet = 0x01;
    }
    
    return valRet;
}


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
uint8 ControlAlphaNumDisplay(uint32 command)
{
// function return value
    uint8 valRet = 0x00;
// for loop counter used to set bliniing display
    uint8 loop_counter = 0x00;
    
// display RESET procedure
    if ( (command & 0x80000000) == 0x80000000)
    {
// set chip select lines for all 3 displays       
        DISP_CE1_Write(0u);
        DISP_CE2_Write(0u);
        DISP_CE3_Write(0u);
// set RESET line low - RESET display units        
        DISP_RST_Write(0u);
// wait 200uS 
        CyDelayUs(200u);        
// set RESET line high - disable RESET       
        DISP_RST_Write(1u);
// disable chip select lines for all 3 displays       
        DISP_CE1_Write(1u);
        DISP_CE2_Write(1u);
        DISP_CE3_Write(1u);
// set return value to 1
        return 0x01;
    }

// display brightness settings   
    if (  ((command & 0x07000000) >= 0x01000000) && ((command & 0x07000000) <= 0x07000000)  )
    {
// set address register to Control Word Register
        Display_ADR_Write(0x30);
// set data register with display brightness informaiton
        Display_DATA_Write(Disp_Brightness & 0x07);
//        Display_DATA_Write( 0x06 );
// wait 1uS 
        CyDelayUs(1u);        
// set chip select lines for all 3 displays       
        DISP_CE1_Write(0u);
        DISP_CE2_Write(0u);
        DISP_CE3_Write(0u);
// wait 10uS 
        CyDelayUs(10u); 
// set WRITE signal low for all 3 displays - enable write
        DISP_WR_Write(0u);
// wait 10uS 
        CyDelayUs(10u); 
// set WRITE signal high for all 3 displays - disable write
        DISP_WR_Write(1u);
// disable chip select lines for all 3 displays       
        DISP_CE1_Write(1u);
        DISP_CE2_Write(1u);
        DISP_CE3_Write(1u);
// set return value to 1
        valRet = 0x01;
    }

// enabling/disabling digit flashing operation
    if ( ( command & 0x08000000 ) == 0x08000000 )
    {
// set address register to Control Word Register
        Display_ADR_Write(0x30);
// set data register with display flash enable bit and display brightness informaiton
        Display_DATA_Write( ( Disp_Brightness | 0x08 ) & 0x0F );
// wait 1uS 
        CyDelayUs(1u);        
// set chip select lines for all 3 displays       
        DISP_CE1_Write(0u);
        DISP_CE2_Write(0u);
        DISP_CE3_Write(0u);
// wait 10uS 
        CyDelayUs(10u); 
// set WRITE signal low for all 3 displays - enable write
        DISP_WR_Write(0u);
// wait 10uS 
        CyDelayUs(10u); 
// set WRITE signal high for all 3 displays - disable write
        DISP_WR_Write(1u);
// disable chip select lines for all 3 displays       
        DISP_CE1_Write(1u);
        DISP_CE2_Write(1u);
        DISP_CE3_Write(1u);
// set return value to 1
        valRet = 0x01;
    }
    else
        {
// set address register to Control Word Register
            Display_ADR_Write(0x30);
// clear display flash bit
            Display_DATA_Write(Disp_Brightness & 0x07);
// wait 1uS 
            CyDelayUs(1u);        
// set chip select lines for all 3 displays       
            DISP_CE1_Write(0u);
            DISP_CE2_Write(0u);
            DISP_CE3_Write(0u);
// wait 10uS 
            CyDelayUs(10u); 
// set WRITE signal low for all 3 displays - enable write
            DISP_WR_Write(0u);
// wait 10uS 
            CyDelayUs(10u); 
// set WRITE signal high for all 3 displays - disable write
            DISP_WR_Write(1u);
// disable chip select lines for all 3 displays       
            DISP_CE1_Write(1u);
            DISP_CE2_Write(1u);
            DISP_CE3_Write(1u);
// set return value to 1
            valRet = 0x01;   
        }
    
        
    
// display blinking settings   
    if ( ( command & 0x00FFFFFF ) > 0x00 )
    {
        
// scan thru all 8 characters in each of 3 displays
        for (loop_counter = 0 ; loop_counter < 24; loop_counter++)
        {
            
// set address of digiti to flash based on display order             
            if (loop_counter <=7)
            {
// set address to Control Word Register
                Display_ADR_Write(loop_counter);
            }
            else if ((loop_counter > 7) && (loop_counter <=15) )
            {
// set address to Control Word Register
                Display_ADR_Write(loop_counter - 8u);
            }
            else if ((loop_counter > 15) && (loop_counter <=23) )
            {
// set address to Control Word Register
                Display_ADR_Write(loop_counter - 16u);
            }

// check if display shoudl flash or not and set its flash bit accordingly
            if ( ( ( command >> loop_counter ) & 0x01 ) == 0x01 )
            {
// set data register with display flash enable bit enabled
                Display_DATA_Write(0x01);
            }
            else
                {
// reset data register with display flash enable bit cleared
                    Display_DATA_Write(0x00);
                }

// set correct chip select line based on loop counter (one of 3 displays)                
            if (loop_counter <=7)
            {
// set chip select lines for display 1
                    DISP_CE1_Write(0u);
            }
            else if ((loop_counter > 7) && (loop_counter <=15) )
            {
// set chip select lines for display 2
                    DISP_CE2_Write(0u); 
            }
            else if ((loop_counter > 15) && (loop_counter <=23) )
            {
// set chip select lines for display 3
                    DISP_CE3_Write(0u); 
            }            
                    
// write data to selected display
// wait 10uS 
            CyDelayUs(10u); 
// set WRITE signal low for all 3 displays - enable write
            DISP_WR_Write(0u);
// wait 10uS 
            CyDelayUs(10u); 
// set WRITE signal high for all 3 displays - disable write
            DISP_WR_Write(1u);    

// clear correct chip select line based on loop counter (one of 3 displays)
            if (loop_counter <=7)
            {
// reset chip select lines for display 1
                DISP_CE1_Write(1u);
            }
            else if ((loop_counter > 7) && (loop_counter <=15) )
            {
// reset chip select lines for display 2
                DISP_CE2_Write(1u); 
            }
            else if ((loop_counter > 15) && (loop_counter <=23) )
            {
// reset chip select lines for display 3
                DISP_CE3_Write(1u); 
            } 
        }

// set return value to 1
        valRet = 0x01; 
    }
    
    return valRet;
}




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
uint8 LoadAlphaNumDisplay(uint8 command)
{
// function return value
    uint8 valRet = 0x00;

// if command is for level characters load
    if ( command == 0x01)
    {   
// location 0x0 = blank display with one dot in the middle
// upload UDC code 0 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 0 address (0x00)         
        WriteAlphaNumDisplay(0x20, 0x00, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000100, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);

        
// location 0x1 = bottom (right channel) empty square, top blank
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 1 address (0x01)         
        WriteAlphaNumDisplay(0x20, 0x01, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00001100, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00001110, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00001100, 0x0E);

        
// location 0x2 = bottom blank, top (left channel) empty square
// upload UDC code 2 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 2 address (0x02)         
        WriteAlphaNumDisplay(0x20, 0x02, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00001100, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00001110, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00001100, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);

        
// location 0x3 = both bottom and top (left and right channels) empty squares        
// upload UDC code 0 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 3 address (0x03)         
        WriteAlphaNumDisplay(0x20, 0x03, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00001100, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00001110, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00001100, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00001100, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00001110, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00001100, 0x0E);        
        
        
// location 0x5 = bottom (right channel) full square, top blank
// upload UDC code 5 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 5 address (0x05)         
        WriteAlphaNumDisplay(0x20, 0x05, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);        
        
        
// location 0x6 = bottom blank, top (left channel) full square
// upload UDC code 6 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 6 address (0x06)         
        WriteAlphaNumDisplay(0x20, 0x06, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00011111, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00011111, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00011111, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);        
        
        
// location 0x7 = both bottom and top (left and right channels) full squares  
// upload UDC code 7 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 7 address (0x07)         
        WriteAlphaNumDisplay(0x20, 0x07, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00011111, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00011111, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00011111, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);        

        
// location 0x8 = arrow left 
// upload UDC code 8 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 8 address (0x08)         
        WriteAlphaNumDisplay(0x20, 0x08, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00001000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00001000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00001110, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00001110, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00001100, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00001010, 0x0E);  
        
        
        
// location 0x9 = arrow right 
// upload UDC code 9 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 9 address (0x09)         
        WriteAlphaNumDisplay(0x20, 0x09, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00001110, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00001110, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);          
        
// set return value to 1        
        valRet = 0x01;
    }
    
// if command is for level characters load
    if ( command == 0x02)
    {   
// location 0x0 - ASCI 0 -> blank with one bottom dot (in the middle) ON
// upload UDC code 0 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 0 address (0x00)         
        WriteAlphaNumDisplay(0x20, 0x00, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000100, 0x0E);
 
// location 0x1 - ASCI 1 -> 3 bottom dots ON
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 1 address (0x01)         
        WriteAlphaNumDisplay(0x20, 0x01, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000100, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00001010, 0x0E);

// location 0x2 - ASCI 1 -> 5 bottom dots ON
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 2 address (0x02)         
        WriteAlphaNumDisplay(0x20, 0x02, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000100, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011011, 0x0E);        

// location 0x3 - ASCI 1 -> 5 bottom dots ON and 3 doths in 2nd line ON
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 3 address (0x03)         
        WriteAlphaNumDisplay(0x20, 0x03, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000100, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00001010, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00010001, 0x0E);         
 
// location 0x4 - ASCI 1 -> 5 dots ON in 1st and 2nd bottom lines
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 4 address (0x04)         
        WriteAlphaNumDisplay(0x20, 0x04, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000100, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011011, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E); 

// location 0x5 - ASCI 1 -> 5 dots ON in 1st, 2nd bottom lines and 3 in 3rd line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 5 address (0x05)         
        WriteAlphaNumDisplay(0x20, 0x05, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000100, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00001010, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00010001, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E); 

// location 0x6 - ASCI 1 -> 5 dots ON in 1st, 2nd and 3rd line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 6 address (0x06)         
        WriteAlphaNumDisplay(0x20, 0x06, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000100, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011011, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);         

// location 0x7 - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd and 3 in 4th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 7 address (0x07)         
        WriteAlphaNumDisplay(0x20, 0x07, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000100, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00001010, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00010001, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);          

// location 0x8 - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd and 4th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 8 address (0x08)         
        WriteAlphaNumDisplay(0x20, 0x08, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000100, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00011011, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);   

// location 0x9 - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th and 3 in 5th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 9 address (0x09)         
        WriteAlphaNumDisplay(0x20, 0x09, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000100, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00001010, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00010001, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);         

// location 0xA - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th and 5th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code A address (0x0A)         
        WriteAlphaNumDisplay(0x20, 0x0A, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000100, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00011011, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);          

// location 0xB - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th, 5th and 3 in 6th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code B address (0x0B)         
        WriteAlphaNumDisplay(0x20, 0x0B, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000100, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00001010, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00010001, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);        

// location 0xC - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th, 5th and 6th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code C address (0x0C)         
        WriteAlphaNumDisplay(0x20, 0x0C, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000100, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00011011, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);         

// location 0xD - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th, 5th, 6th and 3 in 7th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code D address (0x0D)         
        WriteAlphaNumDisplay(0x20, 0x0D, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00001110, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00010001, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);         

// location 0xE - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th, 5th, 6th and 7th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code E address (0x0E)         
        WriteAlphaNumDisplay(0x20, 0x0E, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00011111, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);          
        
        valRet = 0x01;
    }    
 

    
// if command is for level characters load
    if ( command == 0x03)
    {   
// location 0x0 - ASCI 0 -> blank with one bottom dot (in the middle) ON
// upload UDC code 0 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 0 address (0x00)         
        WriteAlphaNumDisplay(0x20, 0x00, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00000000, 0x0E);
 
// location 0x1 - ASCI 1 -> 3 bottom dots ON
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 1 address (0x01)         
        WriteAlphaNumDisplay(0x20, 0x01, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);

// location 0x2 - ASCI 1 -> 5 bottom dots ON
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 2 address (0x02)         
        WriteAlphaNumDisplay(0x20, 0x02, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00000000, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);        

// location 0x3 - ASCI 1 -> 5 bottom dots ON and 3 doths in 2nd line ON
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 3 address (0x03)         
        WriteAlphaNumDisplay(0x20, 0x03, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);         
 
// location 0x4 - ASCI 1 -> 5 dots ON in 1st and 2nd bottom lines
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 4 address (0x04)         
        WriteAlphaNumDisplay(0x20, 0x04, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00000000, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E); 

// location 0x5 - ASCI 1 -> 5 dots ON in 1st, 2nd bottom lines and 3 in 3rd line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 5 address (0x05)         
        WriteAlphaNumDisplay(0x20, 0x05, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E); 

// location 0x6 - ASCI 1 -> 5 dots ON in 1st, 2nd and 3rd line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 6 address (0x06)         
        WriteAlphaNumDisplay(0x20, 0x06, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00000000, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);         

// location 0x7 - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd and 3 in 4th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 7 address (0x07)         
        WriteAlphaNumDisplay(0x20, 0x07, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00011111, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);          

// location 0x8 - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd and 4th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 8 address (0x08)         
        WriteAlphaNumDisplay(0x20, 0x08, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00000000, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00011111, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);   

// location 0x9 - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th and 3 in 5th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code 9 address (0x09)         
        WriteAlphaNumDisplay(0x20, 0x09, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00011111, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00011111, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);         

// location 0xA - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th and 5th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code A address (0x0A)         
        WriteAlphaNumDisplay(0x20, 0x0A, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00000000, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00011111, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00011111, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);          

// location 0xB - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th, 5th and 3 in 6th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code B address (0x0B)         
        WriteAlphaNumDisplay(0x20, 0x0B, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00011111, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00011111, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00011111, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);        

// location 0xC - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th, 5th and 6th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code C address (0x0C)         
        WriteAlphaNumDisplay(0x20, 0x0C, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00000000, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00011111, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00011111, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00011111, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);         

// location 0xD - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th, 5th, 6th and 3 in 7th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code D address (0x0D)         
        WriteAlphaNumDisplay(0x20, 0x0D, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00011111, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00011111, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00011111, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00011111, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);         

// location 0xE - ASCI 1 -> 5 dots ON in 1st, 2nd, 3rd, 4th, 5th, 6th and 7th line
// upload UDC code 1 to all 3 displays (0x0E)
// 1st - UDC address register (0x20), UDC code E address (0x0E)         
        WriteAlphaNumDisplay(0x20, 0x0E, 0x0E);
// 1st - UDC RAM address and row 1 (top) select (0x28), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x28, 0b00011111, 0x0E);
// 2nd - UDC RAM address and row 2 select (0x29), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x29, 0b00011111, 0x0E); 
// 3rd - UDC RAM address and row 3 select (0x2A), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2A, 0b00011111, 0x0E);
// 4th - UDC RAM address and row 4 select (0x2B), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2B, 0b00011111, 0x0E); 
// 5th - UDC RAM address and row 5 select (0x2C), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2C, 0b00011111, 0x0E);
// 6rd - UDC RAM address and row 6 select (0x2D), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2D, 0b00011111, 0x0E);
// 7nd - UDC RAM address and row 7 (bottom) select (0x2E), UDC dot data (0x00)         
        WriteAlphaNumDisplay(0x2E, 0b00011111, 0x0E);          
        
        valRet = 0x01;
        
    }
  
    return valRet;
}


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
uint8 CharacterToAlphaNumDisplay(uint8 character, uint8 position)
{
// function return value
    uint8 valRet = 0x00;
    uint8 digitAdr = 0x00;
    uint8 dispSel = 0x00;
    
   
// calculating address position for particular digit. 
// character position informaiton is inverted to match LED display mapping
// then 5 MSBs are zeroed and /FL+A4+A4 information (0x38) are added
    digitAdr = ( ( ( ~position ) & 0x07) | 0x38 );
    
// check if position is in regards to LSB 8-characters display    
    if ( (0 <= position) && (position <= 7) )
    {
        dispSel = 0x08;  
        valRet = 0x01;
    }
// check if position is in regards to middle 8-characters display    
    else if ( ( 8 <= position) && (position <= 15) )
    {
        dispSel = 0x04; 
        valRet = 0x01;
    }
// check if position is in regards to MSB 8-characters display    
    else if ( ( 16 <= position) && (position <= 23) )
    {
        dispSel = 0x02; 
        valRet = 0x01;
    }
    else
        {
            valRet = 0x00;
        }

// check if return value is 1 indicating correct display position.         
    if (valRet == 0x01)        
    {
// write character to particular display module at particular character position.        
        valRet = WriteAlphaNumDisplay(digitAdr, character, dispSel);
    }
    
    return valRet;
}




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
uint8 DCF_Control(uint8 mode)
{
    uint8 ret_value = 0x00;
    
    if (mode == 0x01)
    {
// Start Timer 10kHz Clock source 
        DCF_Timer_Clock_Start();

// Starts DCF timer
        DCF_Timer_Start();

// set init values for DCF variables
	    DCF_sync=0;
	    DCF_bit=0;
	    DCF_strength = 0;

// initialize interrupt routines for DCF pin activity
        DCF_PIN_INTR_StartEx(DCF_PIN_INTR_ISR);    
    
// initialize DCF timer overload interrupt
        DCF_TIMER_INTR_StartEx(DCF_TIMER_INTR_ISR);          

// set flag indicatign that DCF blcok was started
        DCF_block_activity = 0x01;
        
// set return value to mode value
        ret_value = mode;
    }
    else if (mode == 0x02)
    {
// Stop Timer 10kHz Clock source 
        DCF_Timer_Clock_Stop();

// Stop DCF timer
        DCF_Timer_Stop();

// re-set init values for DCF variables
	    DCF_sync=0;
	    DCF_bit=0;
	    DCF_strength = 0;

// disable interrupt routines for DCF pin activity
        DCF_PIN_INTR_Stop();    
    
// disable DCF timer overload interrupt
        DCF_TIMER_INTR_Stop();          

// set flag indicatign that DCF blcok was reset/desabled
        DCF_block_activity = 0x00;
        
// set return value to mode value
        ret_value = mode;
    }
    else
        {
// set return value to mode value
            ret_value = 0x00;
        }
        
    return ret_value;
}




/*******************************************************************************
* Function Name: DCF_decode(DCF_datetime *DCF_dt)
********************************************************************************
* 
* Summary:
*  Function that decodes DCF signal and returns structure with received time and 
*  date
*  NEEDS TO BE DEBUGGED WHEN DCS SIGNAL IS AVAILABLE
*
* Parameters:
*     DCF_datetime *DCF_dt - returns structure with received data and time 
*         from DCF
*
* Return:
*     0x11 - when decode finished with success
*     0x00 - when decode finished with incorrect result or input data was invalid
*
*******************************************************************************/
uint8 DCF_decode(DCF_datetime *DCF_dt)
{

// checks if 59 bits were received for decoding 
	if (DCF_bit<59)
		return 0;

// check if bit 0 = 0 and bit 20 = 1 (they are always with this value)
// if value is different finish decoding    
	if ((DCF_data[0]!=0)||(DCF_data[20]!=1)) 
		return 0;

// calculate parity bit for bits 21 - 27 (minutes)
	uint8 DCF_parity_m=0; 
	for (int a=21; a<29; a++) // add all minute bits
		DCF_parity_m+=DCF_data[a];

// finish if bit sum is odd 
    if ((DCF_parity_m%2)!=0) 
		return 0;

// calculate parity bit for bits 29 - 34 (hours) 
	uint8_t DCF_parity_h=0;
	for (int a=29; a<36; a++) 
		DCF_parity_h+=DCF_data[a];	

// finish if bit sum is odd 
 	if ((DCF_parity_h%2)!=0)
		return 0;

// calculate parity bit for bits 36 - 57 (date)
	uint8_t DCF_parity_date=0;
	for (int a=36; a<59; a++) 
			DCF_parity_date+=DCF_data[a];	

// finish if bit sum is odd             
	if ((DCF_parity_date%2)!=0)
		return 0;
			
// if all good conrinue with calculations
	
// calculate minuts
	DCF_dt->min	= DCF_data[21]
								+ 2*DCF_data[22]
								+ 4*DCF_data[23]
								+ 8*DCF_data[24]
								+ 10*DCF_data[25]
								+ 20*DCF_data[26]
								+ 40*DCF_data[27];
	
// calculate hours
  DCF_dt->h = DCF_data[29]
								+ 2*DCF_data[30]
								+ 4*DCF_data[31]
								+ 8*DCF_data[32]
								+ 10*DCF_data[33]
								+ 20*DCF_data[34];

// calculate day of the month
	DCF_dt->day	= DCF_data[36]
								+ 2*DCF_data[37]
							    + 4*DCF_data[38]
								+ 8*DCF_data[39]
								+ 10*DCF_data[40]
								+ 20*DCF_data[41];

// calculate month
	DCF_dt->month	= DCF_data[45]
								+ 2*DCF_data[46]
								+ 4*DCF_data[47]
								+ 8*DCF_data[48]
								+ 10*DCF_data[49];

// calculate year	
	DCF_dt->year = DCF_data[50] 
								+ 2 * DCF_data[51]
                                + 4 * DCF_data[52]
                                + 8 * DCF_data[53]
                                + 10 * DCF_data[54]
                                + 20 * DCF_data[55]
                                + 40 * DCF_data[56]
                                + 80 * DCF_data[57]
								+ 2000;

// check if received data make sense
// hour not bigger than 24, minutes not bigger than 60
// months not bigger than 12, number of days not bigger than 31
    if ((DCF_dt->h>24)||(DCF_dt->min>60)||(DCF_dt->month>12)||(DCF_dt->day>31))
		return 0x00;
		      
// success finish
	return 0x11;
} 



/* [] END OF FILE */
