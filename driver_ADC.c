/**
    \file driver_ADC.c
    \version 0.1.0
    \date 2014-07-31
    \brief Driver for the ADC Controller.
    \author Nelson Lombardo
    \license This file is released under the MIT License.
    \include LICENSE
 */

#include "driver_ADC.h"
#include "types.h"

static xtEventCallback g_pfnADCHandlerCallbacks[1] = {0};

/**
    \brief Init the ADC  Interrupt Callback function.

    \param pfnCallback is the callback function.
    
    When there is any ADC intrrupt occrus, Interrupt Handler will 
    call the callback function. 
    
    param of pfnCallback
    - pvCBData not used, always 0.
    - ulEvent is the interrupt event..
    - ulMsgParam not used, always 0.
    - pvMsgData not used, always 0.
    .
    
    \return None.
*/

void adc_int_call_back_init (xtEventCallback pfnCallback)
{
    g_pfnADCHandlerCallbacks[0] = pfnCallback;
}

/**
    \brief ADC Interrupt Handler.
    
    If users want to user the ADC Callback feature, Users should promise 
    that the ADCHandle in the vector table is adc_int_handler.
    
    \return None.
*/

void adc_int_handler (void)
{
    uint32_t ulEventFlags = 0;
    g_pfnADCHandlerCallbacks[0](0, ulEventFlags, 0, 0);
}

void ADC_IRQHandler() 
{
    ADC0_SC1A &= ~ADC_SC1_AIEN_MASK;
}

void adc_init (void) 
{
    /* ADC0 Clock Gate Control (27 bit). 
     * This bit controls the clock gate to the ADC0 module.
     */
    SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
}

/**
    \brief Update CFG to select the input clock source and the divide
    ratio used to generate ADCK. This register is also used for 
    selecting sample time and low-power configuration.
    \param ulPowerMode Is Low-Power Configuration Mode that you will set.
    Reference [Power Mode](\ref KL25Z_ADC_Power_Mode).
    \param ulClockDivide Selects the divide ratio used by the ADC
    to generate the internal clock. Reference [Clock Divide Select](\ref KL25Z_ADC_Clock_Divide_Select).
    \param ulSampleTime Selects between different sample
    times based on the conversion mode selected. Reference [Sample Time](\ref KL25Z_ADC_Sample_Time).
    \param ulResolution Select the resolution of conversion of single or
    differencial mode. Reference [single sample resolution](\ref KL25Z_ADC_Single_Sample_Resolution_Select)
    and [differencial sample resolution](\ref KL25Z_ADC_Differential_Sample_Resolution_Select).
    \param ulInputClock Selects the input clock source to generate the internal clock.
    Reference [Input Clock](\ref KL25Z_ADC_Select_Input_Clock).
    
*/
void adc_primary_configuration (
    uint32_t ulPowerMode, 
    uint32_t ulClockDivide,
    uint32_t ulSampleTime,
    uint32_t ulResolution,
    uint32_t ulInputClock
    )
{
    uint32_t transition;
    ASSERT (
        (ulPowerMode == ADC_POWER_MODE_LOW) ||
        (ulPowerMode == ADC_POWER_MODE_NORMAL),
        1
    );
    ASSERT (
        (ulClockDivide == ADC_INPUT_CLOCK_DIV_1) ||
        (ulClockDivide == ADC_INPUT_CLOCK_DIV_2) ||
        (ulClockDivide == ADC_INPUT_CLOCK_DIV_4) ||
        (ulClockDivide == ADC_INPUT_CLOCK_DIV_8),
        2
    );
    ASSERT (
        (ulSampleTime == ADC_SAMPLE_TIME_SHORT) ||
        (ulSampleTime == ADC_SAMPLE_TIME_LONG)  ,
        3
    );
    ASSERT (
        (ulResolution == ADC_SINGLE_RESOLUTION_8 ) ||
        (ulResolution == ADC_SINGLE_RESOLUTION_10) ||
        (ulResolution == ADC_SINGLE_RESOLUTION_12) ||
        (ulResolution == ADC_SINGLE_RESOLUTION_16) ||
        (ulResolution == ADC_DIFF_RESOLUTION_9 )   ||
        (ulResolution == ADC_DIFF_RESOLUTION_13)   ||
        (ulResolution == ADC_DIFF_RESOLUTION_11)   ||
        (ulResolution == ADC_DIFF_RESOLUTION_16)   ,
        4
    );
    ASSERT (
        (ulInputClock == ADC_INPUT_BUS_CLK)        ||
        (ulInputClock == ADC_INPUT_HALF_BUS_CLK)   ||
        (ulInputClock == ADC_INPUT_ALT_CLK)        ||
        (ulInputClock == ADC_INPUT_ASYN_CLK)       ,
        5
    );
    
    transition = ADC0_CFG1;                 // Backup actual parameters
    
    transition &= ~ADC_CFG1_ADLPC_MASK;     // Especific bit reset 
    transition |= ulPowerMode;              // Set the parameter
    
    transition &= ~ADC_CFG1_ADIV_MASK;      // Especific bit reset 
    transition |= ulClockDivide;            // Set the parameter
    
    transition &= ~ADC_CFG1_ADLSMP_MASK;    // Especific bit reset 
    transition |= ulSampleTime;             // Set the parameter
    
    transition &= ~ADC_CFG1_MODE_MASK;      // Especific bit reset 
    transition |= ulResolution;             // Set the parameter
    
    transition &= ~ADC_CFG1_ADICLK_MASK;    // Especific bit reset 
    transition |= ulInputClock;             // Set the parameter

    ADC0_CFG1 = transition;                 // Load the configuration
}

/**
    \brief Hardware/Software trigger select, compare function, and 
    voltage reference select of the ADC module.
    \param ulTrigger Selects the type of trigger used for initiating a conversion.
    Reference [Trigger Source](\ref KL25Z_ADC_Tigger_Source).  
    \param ulCompare Compare function enable. Reference [compare](\ref KL25Z_ADC_Compare_Function).
    \param ulLessGreater Compare function greater than or less than. Reference 
    [greater than or less than](\ref KL25Z_ADC_Less_Greater_Than).
    \param ulCompareRange Compare function range enable. Reference 
    [compare range](\ref KL25Z_ADC_Comp_Function_Range).
    \param ulAdcDMA DMA Enable.
    Reference [DMA](\ref KL25Z_ADC_DMA).
    \param ulVoltageRef Comparator reference voltage. Reference 
    [comparator voltage](\ref KL25Z_ADC_Comp_Ref_Voltage).
*/
void adc_control_1 (
    uint32_t ulTrigger, 
    uint32_t ulCompare,
    uint32_t ulLessGreater,
    uint32_t ulCompareRange,
    uint32_t ulAdcDMA,
    uint32_t ulVoltageRef
)
{
    uint32_t transition;
    ASSERT (
        (ulTrigger == ADC_TRIGGER_SOFTWARE) ||
        (ulTrigger == ADC_TRIGGER_HARDWARE) ,
        1
    );
    ASSERT (
        (ulCompare == ADC_COMPARE_ENABLE ) ||
        (ulCompare == ADC_COMPARE_DISABLE) ,
        2
    );
    ASSERT (
        (ulLessGreater == ADC_LESS_THAN   ) ||
        (ulLessGreater == ADC_GREATER_THAN) ,
        3
    );    
    ASSERT (
        (ulCompareRange == ADC_COMP_RANGE_CV1    ) ||
        (ulCompareRange == ADC_COMP_RANGE_CV1_CV2) ,
        4
    );
    ASSERT (
        (ulAdcDMA == ADC_DMA_ENABLE ) ||
        (ulAdcDMA == ADC_DMA_DISABLE) ,
        5
    );
    ASSERT (
        (ulVoltageRef == ADC_COMP_REF_VOL_DEFAULT  ) ||
        (ulVoltageRef == ADC_COMP_REF_VOL_ALTERNATE) ,
        6
    );
    
    transition = ADC0_SC2;                  // Backup actual parameters
    
    transition &= ~ADC_SC2_ADTRG_MASK;      // Especific bit reset
    transition |= ulTrigger;                // Set the parameter
    
    transition &= ~ADC_SC2_ACFE_MASK;       // Especific bit reset
    transition |= ulCompare;                // Set the parameter

    transition &= ~ADC_SC2_ACFGT_MASK;      // Especific bit reset
    transition |= ulLessGreater;            // Set the parameter
    
    transition &= ~ADC_SC2_ACREN_MASK;      // Especific bit reset
    transition |= ulCompareRange;           // Set the parameter    
    
    transition &= ~ADC_SC2_ACREN_MASK;      // Especific bit reset
    transition |= ulAdcDMA;                 // Set the parameter 
    
    transition &= ~ADC_SC2_REFSEL_MASK;     // Especific bit reset
    transition |= ulVoltageRef;             // Set the parameter 
    
    ADC0_SC2 = transition;                  // Load the configuration
}

/**
    \brief Configure continuous convert, and hardware averaging functions.
    \param ulConversionMode Converter operation mode.
    Reference [operation mode](\ref KL25Z_ADC_Operation_Mode).  
    \param ulAverageEnable Average Enable.
    Reference [average enable](\ref KL25Z_ADC_Average_Enable).
    \param ulAverageSelect Hardware Average Select.
    Reference [average select](\ref KL25Z_ADC_Hardware_Average_Select).
*/
void adc_control_2 (
    uint32_t ulConversionMode,
    uint32_t ulAverageEnable,
    uint32_t ulAverageSelect
)
{
    uint32_t transition = 0;
    ASSERT (
        (ulConversionMode == ADC_OP_SINGLE    ) ||
        (ulConversionMode == ADC_OP_CONTINUOUS) ,
        1
    );
    ASSERT (
        (ulAverageEnable == ADC_AVERAGE_ENABLE ) ||
        (ulAverageEnable == ADC_AVERAGE_DISABLE) ,
        2
    );
    ASSERT (
        (ulAverageSelect == ADC_HARDWARE_AVERAGE_4) ||
        (ulAverageSelect == ADC_HARDWARE_AVERAGE_8) ||
        (ulAverageSelect == ADC_HARDWARE_AVERAGE_16) ||
        (ulAverageSelect == ADC_HARDWARE_AVERAGE_32) ,
        3
    );   

    transition = ADC0_SC3;                  // Backup actual parameters
                          
    transition &= ~ADC_SC3_ADCO_MASK;       // Especific bit reset
    transition |= ulConversionMode;         // Set the parameter
    
    transition &= ~ADC_SC3_AVGE_MASK;       // Especific bit reset
    transition |= ulAverageEnable;          // Set the parameter
    
    transition &= ~ADC_SC3_AVGS_MASK;       // Especific bit reset
    transition |= ulAverageSelect;          // Set the parameter
    
    ADC0_SC3 = transition;                  // Load the configuration
}

/**
    \brief Used for channel selection.
    \param ulInputChannel ADC Input Channel. Reference 
    [input channel](\ref KL25Z_ADC_Input_Channel). 
*/
void adc_channel (
    uint32_t ulInputChannel
)
{
    ASSERT (
        (ulInputChannel == ADC_DADP0   ) ||
        (ulInputChannel == ADC_DADP1   ) ||
        (ulInputChannel == ADC_DADP2   ) ||
        (ulInputChannel == ADC_DADP3   ) ||
        (ulInputChannel == ADC_AD4     ) ||
        (ulInputChannel == ADC_AD5     ) ||
        (ulInputChannel == ADC_AD6     ) ||
        (ulInputChannel == ADC_AD7     ) ||
        (ulInputChannel == ADC_AD8     ) ||
        (ulInputChannel == ADC_AD9     ) ||
        (ulInputChannel == ADC_AD10    ) ||
        (ulInputChannel == ADC_AD11    ) ||
        (ulInputChannel == ADC_AD12    ) ||
        (ulInputChannel == ADC_AD13    ) ||
        (ulInputChannel == ADC_AD14    ) ||
        (ulInputChannel == ADC_AD15    ) ||
        (ulInputChannel == ADC_AD16    ) ||
        (ulInputChannel == ADC_AD17    ) ||
        (ulInputChannel == ADC_AD18    ) ||
        (ulInputChannel == ADC_AD19    ) ||
        (ulInputChannel == ADC_AD20    ) ||
        (ulInputChannel == ADC_AD21    ) ||
        (ulInputChannel == ADC_AD22    ) ||
        (ulInputChannel == ADC_AD23    ) ||
        (ulInputChannel == ADC_TEMP    ) ||
        (ulInputChannel == ADC_BG      ) ||
        (ulInputChannel == ADC_REFSH   ) ||
        (ulInputChannel == ADC_REFSL   ) ||
        (ulInputChannel == ADC_DISABLED) ,
        1
    ); 
    uint32_t transition = 0;
    transition = ADC0_SC1A;                 // Backup actual parameters
    transition &= ~ADC_SC1_ADCH_MASK;       // Especific bit reset
    transition |= ulInputChannel;           // Set the parameter 
    ADC0_SC1A = transition;                 // Load the configuration
}

/**
    \brief Used for both software and hardware trigger modes of operation.
    \param ulDiffMode ADC Input Mode. Reference [input mode](\ref KL25Z_ADC_Input_Mode).
    \param ulInputChannel ADC Input Channel. Reference 
    [input channel](\ref KL25Z_ADC_Input_Channel). 
*/
void adc_control_channel (
    uint32_t ulDiffMode,
    uint32_t ulInputChannel
)
{
    uint32_t transition = 0;
    ASSERT (
        (ulDiffMode == ADC_INPUT_SINGLE ) ||
        (ulDiffMode == ADC_INPUT_DIFF) ,
        1
    ); 
    ASSERT (
        (ulInputChannel == ADC_DADP0   ) ||
        (ulInputChannel == ADC_DADP1   ) ||
        (ulInputChannel == ADC_DADP2   ) ||
        (ulInputChannel == ADC_DADP3   ) ||
        (ulInputChannel == ADC_AD4     ) ||
        (ulInputChannel == ADC_AD5     ) ||
        (ulInputChannel == ADC_AD6     ) ||
        (ulInputChannel == ADC_AD7     ) ||
        (ulInputChannel == ADC_AD8     ) ||
        (ulInputChannel == ADC_AD9     ) ||
        (ulInputChannel == ADC_AD10    ) ||
        (ulInputChannel == ADC_AD11    ) ||
        (ulInputChannel == ADC_AD12    ) ||
        (ulInputChannel == ADC_AD13    ) ||
        (ulInputChannel == ADC_AD14    ) ||
        (ulInputChannel == ADC_AD15    ) ||
        (ulInputChannel == ADC_AD16    ) ||
        (ulInputChannel == ADC_AD17    ) ||
        (ulInputChannel == ADC_AD18    ) ||
        (ulInputChannel == ADC_AD19    ) ||
        (ulInputChannel == ADC_AD20    ) ||
        (ulInputChannel == ADC_AD21    ) ||
        (ulInputChannel == ADC_AD22    ) ||
        (ulInputChannel == ADC_AD23    ) ||
        (ulInputChannel == ADC_TEMP    ) ||
        (ulInputChannel == ADC_BG      ) ||
        (ulInputChannel == ADC_REFSH   ) ||
        (ulInputChannel == ADC_REFSL   ) ||
        (ulInputChannel == ADC_DISABLED) ,
        2
    ); 
    transition = ulDiffMode || ulInputChannel;
    
    transition = ADC0_SC1A;                 // Backup actual parameters
    
    transition &= ~ADC_SC1_DIFF_MASK;       // Especific bit reset
    transition |= ulDiffMode;               // Set the parameter 
    
    transition &= ~ADC_SC1_ADCH_MASK;       // Especific bit reset
    transition |= ulInputChannel;           // Set the parameter 
    
    ADC0_SC1A = transition;                 // Load the configuration
}

void adc_step_configure (uint32_t ulChannel, uint32_t ulStep, uint32_t ulConfig)
{
    //ulChannel = ulConfig & 0x0F;
    
    // Save the Input mode config (set when the step is 0)
    if(ulStep == 0)
    {
        ADC0_SC1A &= ~ADC_SC1_DIFF_MASK;
        if(ulConfig & ADC_CTL_D)
        {
            ADC0_SC1A |= ADC_SC1_DIFF_MASK;
        }   
    }

    // Input is TS
    if(ulConfig & ADC_CTL_TS)
    {
        ADC0_SC1A &= ~ADC_SC1_ADCH_MASK;
        ADC0_SC1A |= ADC_CTL_TS;
    }

    // Configure the comparator 0
    if(ulConfig & ADC_CTL_CMP0)
    {
        ADC0_CV1 &= ADC_CV1_CV_MASK;  
        ADC0_CV1 |= ulConfig;
    }
    
    // Enable the corresponding channle pair in differential mode
    if(ulConfig & ADC_CTL_D)
    {
        ADC0_SC1A |= 1 << (2 * ulChannel); 
    }
    // Enable the corresponding channle pair in single-end mode 
    else
    {
       ADC0_SC1A |= 1 << ulChannel; 
    }
} /* End Of adc_step_configure */

/**
    \brief Configure an ADC digital comparator.
    \param ulCompID is the ID of the comparator to configure.
    \param ulConfig is the configuration of the comparator.
*/

void adc_comp_condition_config (uint32_t ulCompID, uint32_t ulConfig)
{
    //((ulCompID >= 0) && (ulCompID < 2));
    // Write the config to the register
    ADC0_SC2 &= ~(ADC_SC2_ACREN_MASK | ADC_SC2_ACFGT_MASK);
    ADC0_SC2 |= ulConfig;

}

/**
    \brief Get the captured data for a sample sequence.
    \return Returns the conversion result.
*/

uint32_t adc_data_get_block (void)
{
    uint32_t ulValid = 0;
    // Check if ADC Conversion is complete.
    while(!ulValid) {
        ulValid = (ADC0_SC1A & ADC_SC1_COCO_MASK) >> ADC_SC1_COCO_SHIFT;
    }
    // Casting
    return ((uint32_t) ADC0_RA);
}

/**
    \brief Check if data is ready to read.
       
    \return Return TRUE if the data is ready.
*/

boolean adc_data_is_ready (void)
{
    if ((ADC0_SC1A & ADC_SC1_COCO_MASK) >> ADC_SC1_COCO_SHIFT)
    {
        return TRUE;
    } 
    else 
    {
        return FALSE;
    } 
}

/**
    \brief Get data result of conversion.
       
    \return Return the data result.
*/

uint32_t adc_data_get (void)
{
    uint32_t ulRes;
    ulRes = ADC0_RA;
    return ulRes;
}

/**
    \brief This function configures the initiation criteria for the ADC.
    \param ulChannel Input channel select.
    \param ulInputMode is the analog input mode.  
    Refrence \ref KL25Z_ADC_Input_Mode.
    \param ulOpMode is the Operation mode. 
    Refrence \ref KL25Z_ADC_Operation_Mode.
    \param ulTrigger is the trigger source that initiates the sample sequence;
    must be one of the \b ADC_TRIGGER_* values. 
    Refrence \ref KL25Z_ADC_Tigger_Source.
*/

void adc_configure (uint32_t ulChannel, uint32_t ulInputMode, uint32_t ulOpMode, uint32_t ulTrigger)
{
    ASSERT (
        (ulInputMode == ADC_INPUT_SINGLE) || 
        (ulInputMode == ADC_INPUT_DIFF  ) ,
        1
    );
    ASSERT (
        (ulOpMode == ADC_OP_SINGLE    ) || 
        (ulOpMode == ADC_OP_CONTINUOUS) ,
        2
    );
    ASSERT (
        (ulTrigger == ADC_TRIGGER_PIT0       ) ||
        (ulTrigger == ADC_TRIGGER_PIT1       ) ||
        (ulTrigger == ADC_TRIGGER_TPM0       ) ||
        (ulTrigger == ADC_TRIGGER_TPM1       ) ||
        (ulTrigger == ADC_TRIGGER_TPM2       ) ||
        (ulTrigger == ADC_TRIGGER_LPTMR0     ) || 
        (ulTrigger == ADC_TRIGGER_HSCMP0     ) ||
        (ulTrigger == ADC_TRIGGER_EXT_PIN    ) ||
        (ulTrigger == ADC_TRIGGER_RTC_ALARM  ) ||
        (ulTrigger == ADC_TRIGGER_SOFTWARE   ) ||
        (ulTrigger == ADC_TRIGGER_SOFTWARE   ) ||
        (ulTrigger == ADC_TRIGGER_RTC_SECONDS) ,
        3
    );
    // Set channel
    ADC0_SC1A &= ~ADC_SC1_ADCH_MASK;
    ADC0_SC1A |= ulChannel;
    
    // Set Input Mode
    ADC0_SC1A &= ~ADC_SC1_DIFF_MASK;  
    ADC0_SC1A |= ulInputMode;

    // Set Conversions Mode
    ADC0_SC3 &= ~ADC_SC3_ADCO_MASK;  
    ADC0_SC3 |= ulOpMode;

    // Set Trigger Mode
    if(ulTrigger == ADC_TRIGGER_SOFTWARE)
    {
        ADC0_SC3 &= ~ADC_SC2_ADTRG_MASK;   
    }
    else
    {
        SIM_SOPT7 &= ~SIM_SOPT7_ADC0TRGSEL_MASK;
        SIM_SOPT7 |= ulTrigger;
    } 
    
}
  
/**
    \brief Configure ADC Temperature Sensor.
    \param ulInputMode is the analog input mode.  
    Refrence \ref KL25Z_ADC_Input_Mode.
    \return None.
*/

void adc_temperature_sensor_configure (uint32_t ulInputMode)
{
    // Check the arguments
    //((ulInputMode == ADC_INPUT_SINGLE) ||
    //        (ulInputMode == ADC_INPUT_DIFF));

    // Set Input Mode
    ADC0_SC1A &= ~ADC_SC1_DIFF_MASK;  
    ADC0_SC1A |= ulInputMode;

    // Select Temperature Sensor as input.
    ADC0_SC1A &= ~ADC_SC1_ADCH_MASK;  
    ADC0_SC1A |= ADC_CTL_TS;
}

/**
    \brief Configure ADC Bandgap.
    \param ulInputMode is the analog input mode.  
    Refrence \ref KL25Z_ADC_Input_Mode.
    \return None.
    
*/
void adc_bandgap_configure (uint32_t ulInputMode)
{
    ASSERT (
        (ulInputMode == ADC_INPUT_SINGLE) ||
        (ulInputMode == ADC_INPUT_DIFF  ) ,
        1
    );
    // Set Input Mode
    ADC0_SC1A &= ~ADC_SC1_DIFF_MASK;  
    ADC0_SC1A |= ulInputMode;
    // Select Bandgap as input.
    ADC0_SC1A &= ~ADC_SC1_ADCH_MASK;  
    ADC0_SC1A |= ADC_CTL_BD;
}

/**
    \brief ADC Mux Select.
    \param ulMux is the alternate sets of ADC channels. 
    Refrence \ref KL25Z_ADC_Mux_Select.
*/  

void adc_mux_select (uint32_t ulMux)
{
    // Check the arguments
    // ((ulMux == ADC_MUX_A) ||
    //        (ulMux == ADC_MUX_B));

    // ADxxb channels are selected
    if(ulMux == ADC_MUX_B)
    {
        ADC0_CFG2 |= ADC_CFG2_MUXSEL_MASK;        
    }
    else
    {
        ADC0_CFG2 &= ~ADC_CFG2_MUXSEL_MASK;
    }
}

/**
    \brief ADC conversion speed Select.
    \param ulSpeed is Speed Configuration. 
    Refrence \ref KL25Z_ADC_Speed_Select.
*/

void adc_conv_speed_select (uint32_t ulSpeed)
{
    // Check the arguments
    //((ulSpeed == ADC_CONVERSION_SPEED_HIGH) ||
    //        (ulSpeed == ADC_CONVERSION_SPEED_NORMAL));
    
    // High-speed conversion sequence is selected
    if (ulSpeed == ADC_CONVERSION_SPEED_HIGH)
    {
        ADC0_CFG2 |= ADC_CFG2_ADHSC_MASK;        
    }
    else
    {
        ADC0_CFG2 &= ~ADC_CFG2_ADHSC_MASK;
    }
}

/**
    \brief Enable an ADC single-ended channel.
    
    \param ulChannel  is the channel that to enable. It may be from 0 to 23.
    
    This function enables an corresponding analog input channel.
    
    \return None.
*/

void adc_single_ended_channel_enable (uint32_t ulChannel)    
{
    // Enable the corresponding channel
    ADC0_SC1A &= ~ADC_SC1_DIFF_MASK; 
    ADC0_SC1A &= ~ADC_SC1_ADCH_MASK;  
    ADC0_SC1A |= ulChannel;
}

/**
    \brief Enable an ADC differential channel.
    
    \param ulChannel  is the channel that to enable. It may be from 0 to 3.
    
    This function enables an differential analog input channel.
    
    \return None.
*/  

void adc_diff_channel_enable (uint32_t ulChannel)    
{
    // Enable the corresponding channle
    ADC0_SC1A |= ADC_SC1_DIFF_MASK; 
    ADC0_SC1A &= ~ADC_SC1_ADCH_MASK;  
    ADC0_SC1A |= ulChannel;
}

/**
    \brief Enable ADC interrupt.
*/

void adc_int_enable (void)    
{
    // Enable AD conversion complete interrupts.
    ADC0_SC1A |= ADC_SC1_AIEN_MASK;
}

/**
    \brief Disable ADC interrupt.
*/
void adc_int_disable (void)    
{

    // Disable AD conversion complete interrupts.
    ADC0_SC1A &= ~ADC_SC1_AIEN_MASK;
}

/**
    \brief Get to know Calibration is Failed or completed normally.
    
    Get to know Calibration is Failed or completed normally..
    
    \return Returns the TRUE if Calibration is Failed or returns FALSE
    \e boolean.
*/

boolean adc_calibration_is_failed (void)
{
    // Return Calibration is Failed or not.
    return((ADC0_SC3 & ADC_SC3_CALF_MASK) ? TRUE : FALSE);
}

/**
    \brief Clear the ADC Calibration Failed Flag.
    
    Clear the ADC Calibration Failed Flag
    
    \return None.
*/

void adc_calibration_failed_flag_clear (void)   
{   
    // Write 1 to Calibration Failed Flag
    ADC0_SC3 |= ADC_SC3_CALF_MASK;
}

/**
    \brief Cause a processor trigger for a sample sequence.
    
    This function triggers a processor-initiated sample sequence if the sample
    sequence trigger is configured to \b ADC_TRIGGER_PROCESSOR.  
    
    \return None.
*/

void adc_processor_trigger (void)    
{
    // Start Conversion
    ADC0_SC2 &= ~ADC_SC2_ADTRG_MASK;
}

/**
    \brief Processor  stop the sample sequence.
    
    This function stop the AD conversion and enter to idle.  
    
    \return None.
*/

void
adc_convert_stop (void)
{
    // Stop Conversion
    ADC0_SC1A |= ADC_SC1_ADCH_MASK;
}

/**
    \brief Get the Offset data for the ADC sample channel.
    
    This function get the ADC sample  Offset data of the input channel.
    
    \return Returns the Offset data.
*/

uint32_t adc_offset_data_get (void)    
{
    uint32_t ulData = 0;
    
    // Get Offset data from result register
    ulData = ADC0_OFS & ADC_OFS_OFS_MASK;

    return ulData;
}

/**
    \brief Enable ADC Self Calibration.
    
    This function enables the self calibration. It needs 127 ADC clocks 
    to complete the calibration. Users can call adc_calibration_is_failed()
    to check the calibration done status. And the call adc_calibration_disable
    to disable the self Calibration.
    
    \return None.
*/

void
adc_calibration_enable (void)    
{
    // Self Calibration Enable
    ADC0_SC3 |= ADC_SC3_CAL_MASK;
}

/**
    \brief Disable ADC Self Calibration.
*/

void adc_calibration_disable (void)
{
    // Self Calibration Disable
    ADC0_SC3 &= ~ADC_SC3_CAL_MASK;
}

/**
    \brief Enable ADC Asynchronous Clock Output.
    
    This function enables ADC Asynchronous Clock Output.
    
    \return None.
*/

void adc_asyn_clk_output_enable (void)    
{
    // ADC Asynchronous Clock Output Enable
    ADC0_CFG2 |= ADC_CFG2_ADACKEN_MASK;
}

/**
    \brief Disable ADC Asynchronous Clock Output.
    
    This function disables ADC Asynchronous Clock Output.
    
    \return None.
*/

void adc_asyn_clk_output_disable (void)    
{
    // ADC Asynchronous Clock Output Disable
    ADC0_CFG2 &= ~ADC_CFG2_ADACKEN_MASK;
}

/***
    \brief Check ADC Self Calibration Done status.
    
    It needs some ADC clocks to complete the calibration. Users call this 
    function to check the self calibration status.
    
    \return Returns \b TRUE if the ADC has completed the calibration or
    \b FALSE if has not completed.
*/

boolean
adc_calibration_done (void)    
{
    // Check self calibration status
    return ((ADC0_SC3 & ADC_SC3_CALF_MASK) ? FALSE : TRUE);
}

/**
    \brief Set ADC Voltage Reference.
    
    \param ulRefVol is the Voltage Reference that you will set.
    
    Set ADC Voltage Reference.
    
    \return None.
*/

void
adc_ref_voltage_set (uint32_t ulRefVol)
{
    // Check the arguments
    //((ulRefVol == ADC_COMP_REF_VOL_DEFAULT) ||
    //        (ulRefVol == ADC_COMP_REF_VOL_ALTERNATE));
    
    // Set ADC Voltage Reference.
    ADC0_SC2 &= ~ADC_SC2_REFSEL_MASK;
    ADC0_SC2 |= ulRefVol; 
}

/**
    \brief Enable ADC Hardware Average operation.
    
    This function is used to enable ADC Hardware Average function.
    
    \return None.
*/

void
adc_hardware_average_enable (void)    
{
    // Enable ADC Hardware Average function
    ADC0_SC3 |= ADC_SC3_AVGE_MASK;
}

/**
    \brief Disable ADC Hardware Average operation.
    
    This function is used to Disable ADC Hardware Average function.
    
    \return None.
*/

void
adc_hardware_average_disable (void)    
{
    // Disable ADC Hardware Average function
    ADC0_SC3 &= ~ADC_SC3_AVGE_MASK;
}

/**
    \brief Set ADC Hardware Average operation.
    
    \param ulAverage is the Hardware Average which you will set.
    
    This function is used to Set ADC Hardware Average function. 
    Reference \ref KL25Z_ADC_Hardware_Average_Select.
    
    \return None.
*/
void adc_hardware_average_set (uint32_t ulAverage)    
{
    // Set ADC Hardware Average
    ADC0_SC3 &= ~ADC_SC3_AVGS_MASK;
    ADC0_SC3 |= ulAverage;
}

/**
    \brief Configure the compare data of an ADC digital comparator.
    
    \param ulLowRef is the reference point for the low/mid band threshold.
    \param ulHighRef is the reference point for the mid/high band threshold.
    
    \return None.
*/

void adc_comp_data_set (uint32_t ulLowRef, uint32_t ulHighRef)
{
    // Check the arguments
    //(ulLowRef < 0xFFFF);
    //(ulHighRef < 0xFFFF);

    ADC0_SC2 |= ADC_SC2_ACREN_MASK; 
    ADC0_SC2 |= ADC_SC2_ACFGT_MASK;
    
    // Write the comparator to the compare register
    ADC0_CV1 &= ~ADC_CV1_CV_MASK;
    ADC0_CV1 |= ulLowRef;
    ADC0_CV2 &= ~ADC_CV2_CV_MASK;
    ADC0_CV2 |= ulHighRef;  
}

/**
    \brief Enable the ADC digital comparator.
    
    Enables the comparator to monitor the along input.
    
    \return None.
*/

void adc_comp_enable (void)  
{
    // Enables the corresponding comparator
    ADC0_SC2 |= ADC_SC2_ACFE_MASK;
}

/**
    \brief Disable the ADC digital comparator .
    
    Disables the comparator to monitor the along input.
    
    \return None.
*/

void adc_comp_disable (void) 
{
    // Disables the corresponding comparator
    ADC0_SC2 &= ~ADC_SC2_ACFE_MASK;
}

/**
    \brief Set Compare Function Range.
    
    \param ulRange is the range function
    
    Set Compare Function Range. Reference \ref KL25Z_ADC_Comp_Function_Range.
    
    \return None.
*/

void adc_comp_func_range_set ( uint32_t ulRange) 
{
    // Set Compare Function Range
    ADC0_SC2 &= ~ADC_SC2_ACREN_MASK;
    ADC0_SC2 |= ulRange;
}

/**
    \brief The specified ADC DMA features are enabled.  The ADC can be
    configured to use DMA for transmit  AD conversion data.
    
    \return None.
*/

void adc_dma_enable (void)
{
    // Enable DMA for ADC
    ADC0_SC2 |= ADC_SC2_DMAEN_MASK;
}

/**
    \brief This function is used to disable ADC DMA features that were enabled
    by adc_dma_enable().  The specified SSI DMA features are disabled. 
    
    \return None.
*/  

void adc_dma_disable (void)
{
    // Disable DMA for ADC
    ADC0_SC2 &= ~ADC_SC2_DMAEN_MASK;
}

/**
    \brief ADC Low-Power Configure.

    \param ulPowerMode is Low-Power Configuration Mode that you will set. 
    Refrence \ref KL25Z_ADC_Power_Mode.
    
    This function is to Configure ADC Low-Power Mode.
    
    \return None.
*/

void adc_power_mode_set (uint32_t ulPowerMode)
{
    // Low-power configuration is selected
    if(ulPowerMode == ADC_POWER_MODE_LOW)
    {
        ADC0_CFG1 |= ADC_CFG1_ADLPC_MASK;        
    }
    else
    {
        ADC0_CFG1 &= ~ADC_CFG1_ADLPC_MASK;
    }
}

/**
    \brief ADC Low-Power Configure.

    \param ulSampleTime is sample time that you will set. 
    Refrence \ref KL25Z_ADC_Sample_Time.
    
    This function to Set sample time.
    
    \return None.
*/

void adc_sample_time_set (uint32_t ulSampleTime)
{
    // Long sample time is selected
    if(ulSampleTime == ADC_SAMPLE_TIME_LONG)
    {
        ADC0_CFG1 |= ADC_CFG1_ADLSMP_MASK;        
    }
    else
    {
        ADC0_CFG1 &= ~ADC_CFG1_ADLSMP_MASK;
    }
}

/**
    \brief ADC Gain Set.

    \param ulMinusGain is the ADC Minus-Side Gain Value. It can vary from 0 to 65535.
    \param ulPlusGain is the ADC Plus-Side Gain Value. It can vary from 0 to 65535.
    
    This function to Set ADC Gain.
    
    \return None.
*/

void adc_gain_set (uint32_t ulMinusGain, uint32_t ulPlusGain)
{
    // Set ADC Gain
    ADC0_MG &= ~ADC_MG_MG_MASK;
    ADC0_MG |= ulMinusGain;
    ADC0_PG &= ~ADC_PG_PG_MASK;
    ADC0_PG |= ulPlusGain;
}

/**
    \brief Set ADC Long Sample Time.

    \param ulLongSamTime is Long Sample Time which you will set. Reference
    \ref KL25Z_ADC_Long_Sample_Time_Select.
    
    This function is used to set ADC Long Sample Time.
    
    \return None.
*/

void adc_long_sample_time_set (uint32_t ulLongSamTime)    
{
    // Set ADC Long Sample Time
    ADC0_CFG2 &= ~ADC_CFG2_ADLSTS_MASK;
    ADC0_CFG2 |= ulLongSamTime;
}

/**
    \brief Set ADC single-ended resolution mode.
    \param ulSingleRes is the  single-ended resolution mode that you will set.
    Referece \ref KL25Z_ADC_Single_Sample_Resolution_Select.
*/

void adc_single_resolution_set (uint32_t ulSingleRes)    
{
    // Set single-ended resolution mode
    ADC0_SC1A &= ~ADC_SC1_DIFF_MASK; 
    ADC0_CFG1 &= ~ADC_CFG1_MODE_MASK;
    ADC0_CFG1 |= ulSingleRes << ADC_CFG1_MODE_SHIFT;
}

/**
    \brief Set ADC differential resolution mode.
    \param ulDiffRes is the  differentialresolution mode that you will set.
    Reference \ref KL25Z_ADC_Differential_Sample_Resolution_Select.
*/

void adc_differential_resolution_set (uint32_t ulDiffRes)    
{
    // Set differential resolution mode
    ADC0_SC1A |= ADC_SC1_DIFF_MASK; 
    ADC0_CFG1 &= ~ADC_CFG1_MODE_MASK;
    ADC0_CFG1 |= ulDiffRes;
}

/**
    \brief Enable ADC0 alternate trigger.
*/

void adc_alternate_trigger_enable (void)
{
    // Enable ADC0 alternate trigger.
    SIM_SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN_MASK;
}

/**
    \brief Enable TPM1 channel 0(A) and channel 1(B) triggers selected for ADC0.
*/

void adc_tpm_trigger_enable (void)
{
    // Enable ADC0 TPM1 channel 0(A) and channel 1(B) trigger.
    SIM_SOPT7 &= ~SIM_SOPT7_ADC0ALTTRGEN_MASK;
}

/**
    \brief ADC0 pretrigger select.
    
    \param ulPretrigger is pretrigger select that you will set. 
    Refrence \ref KL25Z_ADC_Pretrigger_Mode.
    
    This function to Set ADC0 pretrigger.
    
    \return None.
*/

void adc_pretrigger_set (uint32_t ulPretrigger)
{
    // Pretrigger is selected
    //
    if(ulPretrigger == ADC_PRE_TRIGGER_B)
    {
        SIM_SOPT7 |= SIM_SOPT7_ADC0PRETRGSEL_MASK;        
    }
    else
    {
        SIM_SOPT7 &= ~SIM_SOPT7_ADC0PRETRGSEL_MASK;
    }
}

void adc_input_clk_select (uint32_t ulClock) 
{
    ADC0_CFG1 |= ADC_CFG1_ADICLK_MASK; 
    ADC0_CFG1 &= ulClock;
}

/**
    \brief Indicates that a conversion or hardware averaging is in progress. 
    
    \return TRUE if is set when a conversion is initiated and cleared 
    when a conversion is completed or aborted. Otherwise, FALSE.
*/

boolean adc_conv_in_progress (void)
{
    if ((ADC0_SC2 >> ADC_SC2_ADACT_SHIFT) & 0x00000001) 
    {
        return TRUE;
    } 
    else
    {
        return FALSE;
    }
}
