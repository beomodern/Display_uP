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

#include <project.h>
#include <modes.h>
#include <operate.h>

// This dummy buffer used by SPIS when it receives command packet.
uint8 RxBuffer[RxPACKET_SIZE] = {0};
uint8 TxBuffer[TxPACKET_SIZE] = {0};

// This dummy buffer used by SPI Slave when it receives data packet.
const uint8 dummyRxBuffer[RxPACKET_SIZE] = {0};
// This dummy buffer used by SPI Slave when it transmitts status packet.
const uint8 dummyTxBuffer[TxPACKET_SIZE] = {0};



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
uint8 SPIS_WaitForCommand(uint8 *RxBuffer)
{
    uint8 tmpBuffer[RxPACKET_SIZE];
    uint32 i;
    
// Wait for the end of the transfer
    while ((RxPACKET_SIZE > SPIS_SpiUartGetRxBufferSize()) & (RxPACKET_SIZE < SPIS_SpiUartGetRxBufferSize()) )
    {
    }
        
// Read packet from the buffer
    i = 0u;
    while (0u != SPIS_SpiUartGetRxBufferSize())
    {
        tmpBuffer[i] = SPIS_SpiUartReadRxData();
        i++;
    }

// Check start and end of packet markers
    if ((tmpBuffer[RxPACKET_SOP_POS] == PACKET_SOP) &&
        (tmpBuffer[RxPACKET_EOP_POS] == PACKET_EOP))
    {
// Return table stripped out of start and stop markers
// data only is returned 24 characters plus 2 control words
        for (i = 1; i<RxPACKET_SIZE; i++)
        {
            RxBuffer[i-1] = (tmpBuffer[i]);
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
void SPIS_CleanupAfterRead(void)
{
// Wait for the end of the transfer
    while ( (RxPACKET_SIZE > SPIS_SpiUartGetRxBufferSize()) & 
            (RxPACKET_SIZE < SPIS_SpiUartGetRxBufferSize()) )
    {
    }

// Clear RX buffer from dummy bytes
    SPIS_SpiUartClearRxBuffer();
// Put dummy data into TX buffer to be transmitted to SPIM
    SPIS_SpiUartPutArray(dummyTxBuffer, TxPACKET_SIZE);
}


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
void SPIS_UpdateStatus(uint32 status)
{
    SPIS_SpiUartClearTxBuffer();
    static uint8 sTxBuffer[TxPACKET_SIZE] = {PACKET_SOP, 0x00, PACKET_EOP};

    sTxBuffer[TxPACKET_STS_POS] = (uint8) status;
// Put data into the slave TX buffer to be transferred while following
// master access.
    SPIS_SpiUartPutArray(sTxBuffer, TxPACKET_SIZE);
}

/* [] END OF FILE */
