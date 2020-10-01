/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef OPEARATE_H_   /* Include guard */
#define OPERATE_H_

#include <project.h>
    
/***************************************
*            Constants
****************************************/
// Rx
// Packet size
#define RxPACKET_SIZE      (28u)

// Byte position within the packet (start, end)
#define RxPACKET_SOP_POS  (0u)
#define RxPACKET_CTRL1_POS  (25u)
#define RxPACKET_CTRL2_POS  (26u)
#define RxPACKET_EOP_POS  (27u)
    
// Tx    
// Packet size
#define TxPACKET_SIZE      (3u)    

// Byte position within the packet (start, status, end)
#define TxPACKET_SOP_POS  (0u)
#define TxPACKET_STS_POS  (1u)
#define TxPACKET_EOP_POS  (2u)    

// Start and end of the packet markers/
#define PACKET_SOP      (0x01u)
#define PACKET_EOP      (0x17u)    
     
// This dummy buffer used by SPIS when it receives command packet.
uint8 RxBuffer[RxPACKET_SIZE];
uint8 TxBuffer[TxPACKET_SIZE];    
    
// This dummy buffer used by SPI Slave when it receives data packet.
const uint8 dummyRxBuffer[RxPACKET_SIZE];
// This dummy buffer used by SPI Slave when it transmitts status packet.
const uint8 dummyTxBuffer[TxPACKET_SIZE];


// This dummy buffer used by SPIS when it receives command packet.
//uint8 RxBuffer[RxPACKET_SIZE];
//uint8 TxBuffer[TxPACKET_SIZE];

// This dummy buffer used by SPI Slave when it receives data packet.
//const uint8 dummyRxBuffer[RxPACKET_SIZE];
// This dummy buffer used by SPI Slave when it transmitts status packet.
//const uint8 dummyTxBuffer[TxPACKET_SIZE];

/*******************************************************************************
* Function Name: SPIS_WaitForCommand
********************************************************************************
* Summary:
*  SPIS waits for completion of the transfer initiated by the SPIM. After packet
*  reception the format of the packet is verified and the command is returned.
*  If the format of the packet is invalid the unknown command is returned.
*
* Parameters:
*  None
*
* Return:
*  Returns command from the received packet.
*
*******************************************************************************/
uint8 SPIS_WaitForCommand(uint8 *RxBuffer);



/*******************************************************************************
* Function Name: SPIS_CleanupAfterRead
********************************************************************************
* Summary:
*  SPIS waits for completion of the read transfer initiated by the SPIM. The
*  received packet is discarded as it contains only dummy data. Then the SPIS
*  prepares for the following command packet by putting dummy bytes into the
*  TX buffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPIS_CleanupAfterRead(void);



/*******************************************************************************
* Function Name: SPIS_UpdateStatus
********************************************************************************
* Summary:
*  SPIS copies packet with response into the buffer.
*
* Parameters:
*  status - status to insert into the response packet.
*
* Return:
*  None
*
*******************************************************************************/
void SPIS_UpdateStatus(uint32 status);  
    


#endif // OPERATE_H_

/* [] END OF FILE */
