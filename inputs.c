/* ========================================
 *
 * Copyright HEMI, 2021
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 * 
 * Functions used by BeoModern main PSoC to monitor input informaiton coming from
 * push buttons and over DataLink interface
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF HEMI.
 *
 * ========================================
*/

#include <inputs.h>
#include <operate.h>
#include <display.h>


/*******************************************************************************
* Function Name: CY_ISR(Periodic_Timer_INTHandler)
********************************************************************************
* Summary:
*  The Interrupt Service Routine. Triggered by software when timer expires after 
*  Periodic_Timer_Start() is called. It uses timer to count 200 mseconds wait.
*  This time is used by the system to perform periodic checks (like brightness, 
*  off timer expired, message timer).
*   
* Parameters:
*  Timer interrupt handler 
*
* Return:
*  updates Disp_Brightness variable when TC is called.
*
********************************************************************************/
CY_ISR(Periodic_Timer_INTHandler)
{
// Check if interrupt came from Timer Counter expired (indicating timer count elapsed)
    if (0u != (Periodic_Timer_ReadStatusRegister() & 0x01))
    {
// set flag indicating 200mS interrupt triggered
        mS200_flag = 1;
        
// check if clock message timer is set. If it is, decereasse its value by 1 (200mS per step)
        if (clock_message_timer > 0u)
        {
            clock_message_timer = clock_message_timer - 1;
        }
            
// clear all remining interrupts        
        Timer_INT_ClearPending();
    }
}




/*******************************************************************************
* Function Name: Slave_ISR
********************************************************************************
* Summary:
*  Function is called by SPI block when new SPI message is received. It sets flag 
*  that is served later on by main loop. 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Slave_ISR()
{
     
// Sets global flag for use by background thread 
    SPI_flag = 1;    

}



/*******************************************************************************
* Function Name: SysTickIsrHandler
********************************************************************************
* Summary:
*  The interrupt handler for SysTick interrupts. Interrupt used by internal RTC
*  function to produce time and date info. 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SysTickIsrHandler(void)
{
    Int_RTC_Update();
}



/*******************************************************************************
* Function Name: DCF_PIN_INTR_ISR
********************************************************************************
* Summary:
*  The interrupt handler for DCF pin state change. It is used to check and capture 
*  DCF timer state
*  NEEDS TO BE DEBUGGED WHEN DCF SIGNAL IS AVAILABLE
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DCF_PIN_INTR_ISR(void)
{

// stop timer    
    DCF_Timer_Stop();    
// read back timer value
    DCF_timer_count = DCF_Timer_ReadCounter();

    
// REMOVE REMOVE REMOVE    
//    UART_UartPutString("Cnt = ");
//    UART_UartPutChar('0' + DCF_timer_count/10000 );
//    UART_UartPutChar('0' + (DCF_timer_count/1000)%10 );
//    UART_UartPutChar('0' + (DCF_timer_count/100)%10 );
//    UART_UartPutChar('0' + (DCF_timer_count/10)%10 );
//    UART_UartPutChar('0' + DCF_timer_count%10 );
//    UART_UartPutString("\n\r");
// REMOVE REMOVE REMOVE    
    
    
// check if current DCF DATA pin state is 0    
// if it is 0 it indicates transition from 1 to 0 and counter stores 1 state timing
    if (DCF_DATA_Read() == 0x00)
    {
 
        
// REMOVE REMOVE REMOVE            
//        UART_UartPutString("previous ONE\n\r");
// REMOVE REMOVE REMOVE    
        
        
//	search for synchronizacji bit - time from 1,5s to 2s
		if ((DCF_timer_count > 15000) && (DCF_timer_count < 20000))
		{
 
            
// REMOVE REMOVE REMOVE                
//            UART_UartPutString("ONE for 1500-2000mS\n\r");
// REMOVE REMOVE REMOVE    
            
            
// set to 1 synchronization bit			
            DCF_sync=1; 
// set table intex to 0 (first data bit is at 0 index) 
			DCF_bit=0;	
		}
// if off time is outside standard range (750ms - 960ms) decrease ramge       
		else if (!((DCF_timer_count > 7500) && (DCF_timer_count < 9600))) 
		    {
               
                
// REMOVE REMOVE REMOVE                    
//                UART_UartPutString("ONE different than 750-960mS\n\r");
// REMOVE REMOVE REMOVE    
                
                
// clear synchronizaityon flag
				DCF_sync=0;
// clear table intex to 0 (first data bit is at 0 index) 
			    DCF_bit=0;  
                
// decrease strength indicator value                
                if (DCF_strength>0)
                {
		            DCF_strength--; 
                }
		    }
    }
// if it is 1 it indicates transition from 0 to 1 and counter stores 0 state timing
    else
        {
            

// REMOVE REMOVE REMOVE                
//            UART_UartPutString("previous ZERO\n\r");
// REMOVE REMOVE REMOVE                
            
            
// check if counter readout is in range of 40 to 130 mS            
        if ((DCF_timer_count > 400) && (DCF_timer_count < 1300)) 
			{


// REMOVE REMOVE REMOVE                    
//                UART_UartPutString("0 = ZERO for 40-130mS\n\r");
// REMOVE REMOVE REMOVE    
                                
                
// receive bit 0
				if (DCF_strength<4)
                {
// increase streangth indicator                
				    DCF_strength++;
                }
// if synchronization bit have been found add received bit to the table
				if (DCF_sync==1)
				{
// store bit 0 into table					
                    DCF_data[DCF_bit]=0;
// increase table index 
					DCF_bit++; 
				}
			}
// check if counter readout is in range of 140 to 250 mS          
            else if ((DCF_timer_count > 1400) && (DCF_timer_count < 2500))
			    {

                    
// REMOVE REMOVE REMOVE                        
//                    UART_UartPutString("1 = ZERO for 140-250mS\n\r");
// REMOVE REMOVE REMOVE    
                    
                    
// receive bit 1
				    if (DCF_strength<4)
                    {
// increase streangth indicator                
					    DCF_strength++;
                    }
// if synchronization bit have been found add received bit to the table				
				    if (DCF_sync==1)
				    {
// store bit 1 into table
                        DCF_data[DCF_bit]=1;
// increase table index 
                        DCF_bit++;
				    }
			    }
// if time is other than in ranges outlined above			
                else 
			    {
    
                    
// REMOVE REMOVE REMOVE                        
//                    UART_UartPutString("incorrect timming\n\r");
// REMOVE REMOVE REMOVE    
                    
                    
// clear synchronizaityon flag
				    DCF_sync=0;
// clear table intex to 0 (first data bit is at 0 index) 
			        DCF_bit=0;                    
// decrease strength indicator value                
                    if (DCF_strength>0)
                    {
		                DCF_strength--; 
                    }
			    }
        }
        
// re-initialize timer with 0 value
    DCF_Timer_WriteCounter(0x00);
// re-start timer
    DCF_Timer_Enable();        
        
// clear pin interrupt source
    DCF_DATA_ClearInterrupt();
    
// Clear interrupt source - CHECK IF THIS IS NEEDED.
//    DCF_PIN_INTR_ClearPending();
}



/*******************************************************************************
* Function Name: DCF_TIMER_INTR_ISR
********************************************************************************
* Summary:
*  The interrupt handler for DCF timer overload indicaton to the overall system.
*  NEEDS TO BE DEBUGGED WHEN DCF SIGNAL IS AVAILABLE
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DCF_TIMER_INTR_ISR(void)
{
  

// REMOVE REMOVE REMOVE        
//    UART_UartPutString("Terminal Count\n\r");
// REMOVE REMOVE REMOVE    
    
        
// clear synchronizaityon flag
    DCF_sync=0;
// clear table intex to 0 (first data bit is at 0 index) 
	DCF_bit=0;  
    
// check curret status of DCF signal and decrease strength if timer expired 
    if (DCF_strength>0)
    {
		DCF_strength--; 
    }

// stop timer    
    DCF_Timer_Stop();    
// re-initialize timer with 0 value
    DCF_Timer_WriteCounter(0x00);
// re-start timer
    DCF_Timer_Enable();
    
// Clear Terminal count interrupt source
    DCF_Timer_ClearInterrupt(DCF_Timer_INTR_MASK_TC);
    
}


/* [] END OF FILE */

