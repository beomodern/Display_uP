/*******************************************************************************
* File Name: DCF_Timer.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the DCF_Timer
*  component
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DCF_Timer.h"

uint8 DCF_Timer_initVar = 0u;


/*******************************************************************************
* Function Name: DCF_Timer_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default DCF_Timer configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (DCF_Timer__QUAD == DCF_Timer_CONFIG)
        DCF_Timer_CONTROL_REG = DCF_Timer_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        DCF_Timer_TRIG_CONTROL1_REG  = DCF_Timer_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        DCF_Timer_SetInterruptMode(DCF_Timer_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        DCF_Timer_SetCounterMode(DCF_Timer_COUNT_DOWN);
        DCF_Timer_WritePeriod(DCF_Timer_QUAD_PERIOD_INIT_VALUE);
        DCF_Timer_WriteCounter(DCF_Timer_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (DCF_Timer__QUAD == DCF_Timer_CONFIG) */

    #if (DCF_Timer__TIMER == DCF_Timer_CONFIG)
        DCF_Timer_CONTROL_REG = DCF_Timer_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        DCF_Timer_TRIG_CONTROL1_REG  = DCF_Timer_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        DCF_Timer_SetInterruptMode(DCF_Timer_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        DCF_Timer_WritePeriod(DCF_Timer_TC_PERIOD_VALUE );

        #if (DCF_Timer__COMPARE == DCF_Timer_TC_COMP_CAP_MODE)
            DCF_Timer_WriteCompare(DCF_Timer_TC_COMPARE_VALUE);

            #if (1u == DCF_Timer_TC_COMPARE_SWAP)
                DCF_Timer_SetCompareSwap(1u);
                DCF_Timer_WriteCompareBuf(DCF_Timer_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == DCF_Timer_TC_COMPARE_SWAP) */
        #endif  /* (DCF_Timer__COMPARE == DCF_Timer_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (DCF_Timer_CY_TCPWM_V2 && DCF_Timer_TIMER_UPDOWN_CNT_USED && !DCF_Timer_CY_TCPWM_4000)
            DCF_Timer_WriteCounter(1u);
        #elif(DCF_Timer__COUNT_DOWN == DCF_Timer_TC_COUNTER_MODE)
            DCF_Timer_WriteCounter(DCF_Timer_TC_PERIOD_VALUE);
        #else
            DCF_Timer_WriteCounter(0u);
        #endif /* (DCF_Timer_CY_TCPWM_V2 && DCF_Timer_TIMER_UPDOWN_CNT_USED && !DCF_Timer_CY_TCPWM_4000) */
    #endif  /* (DCF_Timer__TIMER == DCF_Timer_CONFIG) */

    #if (DCF_Timer__PWM_SEL == DCF_Timer_CONFIG)
        DCF_Timer_CONTROL_REG = DCF_Timer_CTRL_PWM_BASE_CONFIG;

        #if (DCF_Timer__PWM_PR == DCF_Timer_PWM_MODE)
            DCF_Timer_CONTROL_REG |= DCF_Timer_CTRL_PWM_RUN_MODE;
            DCF_Timer_WriteCounter(DCF_Timer_PWM_PR_INIT_VALUE);
        #else
            DCF_Timer_CONTROL_REG |= DCF_Timer_CTRL_PWM_ALIGN | DCF_Timer_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (DCF_Timer_CY_TCPWM_V2 && DCF_Timer_PWM_UPDOWN_CNT_USED && !DCF_Timer_CY_TCPWM_4000)
                DCF_Timer_WriteCounter(1u);
            #elif (DCF_Timer__RIGHT == DCF_Timer_PWM_ALIGN)
                DCF_Timer_WriteCounter(DCF_Timer_PWM_PERIOD_VALUE);
            #else 
                DCF_Timer_WriteCounter(0u);
            #endif  /* (DCF_Timer_CY_TCPWM_V2 && DCF_Timer_PWM_UPDOWN_CNT_USED && !DCF_Timer_CY_TCPWM_4000) */
        #endif  /* (DCF_Timer__PWM_PR == DCF_Timer_PWM_MODE) */

        #if (DCF_Timer__PWM_DT == DCF_Timer_PWM_MODE)
            DCF_Timer_CONTROL_REG |= DCF_Timer_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (DCF_Timer__PWM_DT == DCF_Timer_PWM_MODE) */

        #if (DCF_Timer__PWM == DCF_Timer_PWM_MODE)
            DCF_Timer_CONTROL_REG |= DCF_Timer_CTRL_PWM_PRESCALER;
        #endif  /* (DCF_Timer__PWM == DCF_Timer_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        DCF_Timer_TRIG_CONTROL1_REG  = DCF_Timer_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        DCF_Timer_SetInterruptMode(DCF_Timer_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (DCF_Timer__PWM_PR == DCF_Timer_PWM_MODE)
            DCF_Timer_TRIG_CONTROL2_REG =
                    (DCF_Timer_CC_MATCH_NO_CHANGE    |
                    DCF_Timer_OVERLOW_NO_CHANGE      |
                    DCF_Timer_UNDERFLOW_NO_CHANGE);
        #else
            #if (DCF_Timer__LEFT == DCF_Timer_PWM_ALIGN)
                DCF_Timer_TRIG_CONTROL2_REG = DCF_Timer_PWM_MODE_LEFT;
            #endif  /* ( DCF_Timer_PWM_LEFT == DCF_Timer_PWM_ALIGN) */

            #if (DCF_Timer__RIGHT == DCF_Timer_PWM_ALIGN)
                DCF_Timer_TRIG_CONTROL2_REG = DCF_Timer_PWM_MODE_RIGHT;
            #endif  /* ( DCF_Timer_PWM_RIGHT == DCF_Timer_PWM_ALIGN) */

            #if (DCF_Timer__CENTER == DCF_Timer_PWM_ALIGN)
                DCF_Timer_TRIG_CONTROL2_REG = DCF_Timer_PWM_MODE_CENTER;
            #endif  /* ( DCF_Timer_PWM_CENTER == DCF_Timer_PWM_ALIGN) */

            #if (DCF_Timer__ASYMMETRIC == DCF_Timer_PWM_ALIGN)
                DCF_Timer_TRIG_CONTROL2_REG = DCF_Timer_PWM_MODE_ASYM;
            #endif  /* (DCF_Timer__ASYMMETRIC == DCF_Timer_PWM_ALIGN) */
        #endif  /* (DCF_Timer__PWM_PR == DCF_Timer_PWM_MODE) */

        /* Set other values from customizer */
        DCF_Timer_WritePeriod(DCF_Timer_PWM_PERIOD_VALUE );
        DCF_Timer_WriteCompare(DCF_Timer_PWM_COMPARE_VALUE);

        #if (1u == DCF_Timer_PWM_COMPARE_SWAP)
            DCF_Timer_SetCompareSwap(1u);
            DCF_Timer_WriteCompareBuf(DCF_Timer_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == DCF_Timer_PWM_COMPARE_SWAP) */

        #if (1u == DCF_Timer_PWM_PERIOD_SWAP)
            DCF_Timer_SetPeriodSwap(1u);
            DCF_Timer_WritePeriodBuf(DCF_Timer_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == DCF_Timer_PWM_PERIOD_SWAP) */
    #endif  /* (DCF_Timer__PWM_SEL == DCF_Timer_CONFIG) */
    
}


/*******************************************************************************
* Function Name: DCF_Timer_Enable
********************************************************************************
*
* Summary:
*  Enables the DCF_Timer.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    DCF_Timer_BLOCK_CONTROL_REG |= DCF_Timer_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (DCF_Timer__PWM_SEL == DCF_Timer_CONFIG)
        #if (0u == DCF_Timer_PWM_START_SIGNAL_PRESENT)
            DCF_Timer_TriggerCommand(DCF_Timer_MASK, DCF_Timer_CMD_START);
        #endif /* (0u == DCF_Timer_PWM_START_SIGNAL_PRESENT) */
    #endif /* (DCF_Timer__PWM_SEL == DCF_Timer_CONFIG) */

    #if (DCF_Timer__TIMER == DCF_Timer_CONFIG)
        #if (0u == DCF_Timer_TC_START_SIGNAL_PRESENT)
            DCF_Timer_TriggerCommand(DCF_Timer_MASK, DCF_Timer_CMD_START);
        #endif /* (0u == DCF_Timer_TC_START_SIGNAL_PRESENT) */
    #endif /* (DCF_Timer__TIMER == DCF_Timer_CONFIG) */
    
    #if (DCF_Timer__QUAD == DCF_Timer_CONFIG)
        #if (0u != DCF_Timer_QUAD_AUTO_START)
            DCF_Timer_TriggerCommand(DCF_Timer_MASK, DCF_Timer_CMD_RELOAD);
        #endif /* (0u != DCF_Timer_QUAD_AUTO_START) */
    #endif  /* (DCF_Timer__QUAD == DCF_Timer_CONFIG) */
}


/*******************************************************************************
* Function Name: DCF_Timer_Start
********************************************************************************
*
* Summary:
*  Initializes the DCF_Timer with default customizer
*  values when called the first time and enables the DCF_Timer.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  DCF_Timer_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time DCF_Timer_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the DCF_Timer_Start() routine.
*
*******************************************************************************/
void DCF_Timer_Start(void)
{
    if (0u == DCF_Timer_initVar)
    {
        DCF_Timer_Init();
        DCF_Timer_initVar = 1u;
    }

    DCF_Timer_Enable();
}


/*******************************************************************************
* Function Name: DCF_Timer_Stop
********************************************************************************
*
* Summary:
*  Disables the DCF_Timer.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_BLOCK_CONTROL_REG &= (uint32)~DCF_Timer_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the DCF_Timer. This function is used when
*  configured as a generic DCF_Timer and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the DCF_Timer to operate in
*   Values:
*   - DCF_Timer_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - DCF_Timer_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - DCF_Timer_MODE_QUAD - Quadrature decoder
*         - DCF_Timer_MODE_PWM - PWM
*         - DCF_Timer_MODE_PWM_DT - PWM with dead time
*         - DCF_Timer_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_MODE_MASK;
    DCF_Timer_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - DCF_Timer_MODE_X1 - Counts on phi 1 rising
*         - DCF_Timer_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - DCF_Timer_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_QUAD_MODE_MASK;
    DCF_Timer_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - DCF_Timer_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - DCF_Timer_PRESCALE_DIVBY2    - Divide by 2
*         - DCF_Timer_PRESCALE_DIVBY4    - Divide by 4
*         - DCF_Timer_PRESCALE_DIVBY8    - Divide by 8
*         - DCF_Timer_PRESCALE_DIVBY16   - Divide by 16
*         - DCF_Timer_PRESCALE_DIVBY32   - Divide by 32
*         - DCF_Timer_PRESCALE_DIVBY64   - Divide by 64
*         - DCF_Timer_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_PRESCALER_MASK;
    DCF_Timer_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the DCF_Timer runs
*  continuously or stops when terminal count is reached.  By default the
*  DCF_Timer operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_ONESHOT_MASK;
    DCF_Timer_CONTROL_REG |= ((uint32)((oneShotEnable & DCF_Timer_1BIT_MASK) <<
                                                               DCF_Timer_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetPWMMode(uint32 modeMask)
{
    DCF_Timer_TRIG_CONTROL2_REG = (modeMask & DCF_Timer_6BIT_MASK);
}



/*******************************************************************************
* Function Name: DCF_Timer_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_PWM_SYNC_KILL_MASK;
    DCF_Timer_CONTROL_REG |= ((uint32)((syncKillEnable & DCF_Timer_1BIT_MASK)  <<
                                               DCF_Timer_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_PWM_STOP_KILL_MASK;
    DCF_Timer_CONTROL_REG |= ((uint32)((stopOnKillEnable & DCF_Timer_1BIT_MASK)  <<
                                                         DCF_Timer_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_PRESCALER_MASK;
    DCF_Timer_CONTROL_REG |= ((uint32)((deadTime & DCF_Timer_8BIT_MASK) <<
                                                          DCF_Timer_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - DCF_Timer_INVERT_LINE   - Inverts the line output
*         - DCF_Timer_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_INV_OUT_MASK;
    DCF_Timer_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: DCF_Timer_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_WriteCounter(uint32 count)
{
    DCF_Timer_COUNTER_REG = (count & DCF_Timer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: DCF_Timer_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 DCF_Timer_ReadCounter(void)
{
    return (DCF_Timer_COUNTER_REG & DCF_Timer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - DCF_Timer_COUNT_UP       - Counts up
*     - DCF_Timer_COUNT_DOWN     - Counts down
*     - DCF_Timer_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - DCF_Timer_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_UPDOWN_MASK;
    DCF_Timer_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_WritePeriod(uint32 period)
{
    DCF_Timer_PERIOD_REG = (period & DCF_Timer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: DCF_Timer_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 DCF_Timer_ReadPeriod(void)
{
    return (DCF_Timer_PERIOD_REG & DCF_Timer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_RELOAD_CC_MASK;
    DCF_Timer_CONTROL_REG |= (swapEnable & DCF_Timer_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_WritePeriodBuf(uint32 periodBuf)
{
    DCF_Timer_PERIOD_BUF_REG = (periodBuf & DCF_Timer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: DCF_Timer_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 DCF_Timer_ReadPeriodBuf(void)
{
    return (DCF_Timer_PERIOD_BUF_REG & DCF_Timer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_CONTROL_REG &= (uint32)~DCF_Timer_RELOAD_PERIOD_MASK;
    DCF_Timer_CONTROL_REG |= ((uint32)((swapEnable & DCF_Timer_1BIT_MASK) <<
                                                            DCF_Timer_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void DCF_Timer_WriteCompare(uint32 compare)
{
    #if (DCF_Timer_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (DCF_Timer_CY_TCPWM_4000) */

    #if (DCF_Timer_CY_TCPWM_4000)
        currentMode = ((DCF_Timer_CONTROL_REG & DCF_Timer_UPDOWN_MASK) >> DCF_Timer_UPDOWN_SHIFT);

        if (((uint32)DCF_Timer__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)DCF_Timer__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (DCF_Timer_CY_TCPWM_4000) */
    
    DCF_Timer_COMP_CAP_REG = (compare & DCF_Timer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: DCF_Timer_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 DCF_Timer_ReadCompare(void)
{
    #if (DCF_Timer_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (DCF_Timer_CY_TCPWM_4000) */

    #if (DCF_Timer_CY_TCPWM_4000)
        currentMode = ((DCF_Timer_CONTROL_REG & DCF_Timer_UPDOWN_MASK) >> DCF_Timer_UPDOWN_SHIFT);
        
        regVal = DCF_Timer_COMP_CAP_REG;
        
        if (((uint32)DCF_Timer__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)DCF_Timer__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & DCF_Timer_16BIT_MASK);
    #else
        return (DCF_Timer_COMP_CAP_REG & DCF_Timer_16BIT_MASK);
    #endif /* (DCF_Timer_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: DCF_Timer_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void DCF_Timer_WriteCompareBuf(uint32 compareBuf)
{
    #if (DCF_Timer_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (DCF_Timer_CY_TCPWM_4000) */

    #if (DCF_Timer_CY_TCPWM_4000)
        currentMode = ((DCF_Timer_CONTROL_REG & DCF_Timer_UPDOWN_MASK) >> DCF_Timer_UPDOWN_SHIFT);

        if (((uint32)DCF_Timer__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)DCF_Timer__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (DCF_Timer_CY_TCPWM_4000) */
    
    DCF_Timer_COMP_CAP_BUF_REG = (compareBuf & DCF_Timer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: DCF_Timer_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 DCF_Timer_ReadCompareBuf(void)
{
    #if (DCF_Timer_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (DCF_Timer_CY_TCPWM_4000) */

    #if (DCF_Timer_CY_TCPWM_4000)
        currentMode = ((DCF_Timer_CONTROL_REG & DCF_Timer_UPDOWN_MASK) >> DCF_Timer_UPDOWN_SHIFT);

        regVal = DCF_Timer_COMP_CAP_BUF_REG;
        
        if (((uint32)DCF_Timer__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)DCF_Timer__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & DCF_Timer_16BIT_MASK);
    #else
        return (DCF_Timer_COMP_CAP_BUF_REG & DCF_Timer_16BIT_MASK);
    #endif /* (DCF_Timer_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: DCF_Timer_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 DCF_Timer_ReadCapture(void)
{
    return (DCF_Timer_COMP_CAP_REG & DCF_Timer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: DCF_Timer_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 DCF_Timer_ReadCaptureBuf(void)
{
    return (DCF_Timer_COMP_CAP_BUF_REG & DCF_Timer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - DCF_Timer_TRIG_LEVEL     - Level
*     - DCF_Timer_TRIG_RISING    - Rising edge
*     - DCF_Timer_TRIG_FALLING   - Falling edge
*     - DCF_Timer_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_TRIG_CONTROL1_REG &= (uint32)~DCF_Timer_CAPTURE_MASK;
    DCF_Timer_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - DCF_Timer_TRIG_LEVEL     - Level
*     - DCF_Timer_TRIG_RISING    - Rising edge
*     - DCF_Timer_TRIG_FALLING   - Falling edge
*     - DCF_Timer_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_TRIG_CONTROL1_REG &= (uint32)~DCF_Timer_RELOAD_MASK;
    DCF_Timer_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << DCF_Timer_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - DCF_Timer_TRIG_LEVEL     - Level
*     - DCF_Timer_TRIG_RISING    - Rising edge
*     - DCF_Timer_TRIG_FALLING   - Falling edge
*     - DCF_Timer_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_TRIG_CONTROL1_REG &= (uint32)~DCF_Timer_START_MASK;
    DCF_Timer_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << DCF_Timer_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - DCF_Timer_TRIG_LEVEL     - Level
*     - DCF_Timer_TRIG_RISING    - Rising edge
*     - DCF_Timer_TRIG_FALLING   - Falling edge
*     - DCF_Timer_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_TRIG_CONTROL1_REG &= (uint32)~DCF_Timer_STOP_MASK;
    DCF_Timer_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << DCF_Timer_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - DCF_Timer_TRIG_LEVEL     - Level
*     - DCF_Timer_TRIG_RISING    - Rising edge
*     - DCF_Timer_TRIG_FALLING   - Falling edge
*     - DCF_Timer_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_TRIG_CONTROL1_REG &= (uint32)~DCF_Timer_COUNT_MASK;
    DCF_Timer_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << DCF_Timer_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - DCF_Timer_CMD_CAPTURE    - Trigger Capture/Switch command
*     - DCF_Timer_CMD_RELOAD     - Trigger Reload/Index command
*     - DCF_Timer_CMD_STOP       - Trigger Stop/Kill command
*     - DCF_Timer_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    DCF_Timer_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: DCF_Timer_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the DCF_Timer.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - DCF_Timer_STATUS_DOWN    - Set if counting down
*     - DCF_Timer_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 DCF_Timer_ReadStatus(void)
{
    return ((DCF_Timer_STATUS_REG >> DCF_Timer_RUNNING_STATUS_SHIFT) |
            (DCF_Timer_STATUS_REG & DCF_Timer_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: DCF_Timer_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - DCF_Timer_INTR_MASK_TC       - Terminal count mask
*     - DCF_Timer_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetInterruptMode(uint32 interruptMask)
{
    DCF_Timer_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: DCF_Timer_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - DCF_Timer_INTR_MASK_TC       - Terminal count mask
*     - DCF_Timer_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 DCF_Timer_GetInterruptSourceMasked(void)
{
    return (DCF_Timer_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: DCF_Timer_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - DCF_Timer_INTR_MASK_TC       - Terminal count mask
*     - DCF_Timer_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 DCF_Timer_GetInterruptSource(void)
{
    return (DCF_Timer_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: DCF_Timer_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - DCF_Timer_INTR_MASK_TC       - Terminal count mask
*     - DCF_Timer_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_ClearInterrupt(uint32 interruptMask)
{
    DCF_Timer_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: DCF_Timer_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - DCF_Timer_INTR_MASK_TC       - Terminal count mask
*     - DCF_Timer_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void DCF_Timer_SetInterrupt(uint32 interruptMask)
{
    DCF_Timer_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
