/**
    \file driver_ADC.h
    \version 0.1.0
    \date 2014-07-31
    \brief Defines and Macros for ADC API.
    \author Nelson Lombardo
    \license This file is released under the MIT License.
    \include LICENSE
 */

#ifndef _DRIVER_ADC_H_
#define _DRIVER_ADC_H_

#include <stdint.h>
#include "types.h"
#include "OpenKL25Z.h"
#include "debug.h"

#define ADC ADC_BASE_PTR 
        
/**
    \addtogroup KL25Z_ADC
    @{
*/

/**
    \addtogroup KL25Z_ADC_Input_Channel KL25Z ADC Input Channel
    \brief ADC Channel ID/index
    @{
*/

#define ADC_DADP0       0x00000000   /**< Analog Input Channel 0.    */
#define ADC_DADP1       0x00000001   /**< Analog Input Channel 1.    */
#define ADC_DADP2       0x00000002   /**< Analog Input Channel 2.    */
#define ADC_DADP3       0x00000003   /**< Analog Input Channel 3.    */
#define ADC_AD4         0x00000004   /**< Analog Input Channel 4.    */
#define ADC_AD5         0x00000005   /**< Analog Input Channel 5.    */
#define ADC_AD6         0x00000006   /**< Analog Input Channel 6.    */
#define ADC_AD7         0x00000007   /**< Analog Input Channel 7.    */
#define ADC_AD8         0x00000008   /**< Analog Input Channel 8.    */
#define ADC_AD9         0x00000009   /**< Analog Input Channel 9.    */
#define ADC_AD10        0x0000000A   /**< Analog Input Channel 10.   */
#define ADC_AD11        0x0000000B   /**< Analog Input Channel 11.   */
#define ADC_AD12        0x0000000C   /**< Analog Input Channel 12.   */
#define ADC_AD13        0x0000000D   /**< Analog Input Channel 13.   */
#define ADC_AD14        0x0000000E   /**< Analog Input Channel 14.   */
#define ADC_AD15        0x0000000F   /**< Analog Input Channel 15.   */
#define ADC_AD16        0x00000010   /**< Analog Input Channel 16.   */
#define ADC_AD17        0x00000011   /**< Analog Input Channel 17.   */
#define ADC_AD18        0x00000012   /**< Analog Input Channel 18.   */
#define ADC_AD19        0x00000013   /**< Analog Input Channel 19.   */
#define ADC_AD20        0x00000014   /**< Analog Input Channel 20.   */
#define ADC_AD21        0x00000015   /**< Analog Input Channel 21.   */
#define ADC_AD22        0x00000016   /**< Analog Input Channel 22.   */
#define ADC_AD23        0x00000017   /**< Analog Input Channel 23.   */
#define ADC_TEMP        0x0000001A   /**< Temp sensor select.        */
#define ADC_BG          0x0000001B   /**< Bandgap select.            */
#define ADC_REFSH       0x0000001D   /**< V REFSH select.            */
#define ADC_REFSL       0x0000001E   /**< V REFSL select.            */
#define ADC_DISABLED    0x0000001F   /**< Module disabled.           */

/** @} */

/**
    \addtogroup KL25Z_ADC_Step_Config KL25Z ADC Step Config
    \brief ADC step config values, such as single-end / diffrence, channel source.
    
    They are can be passed to the adc_step_configure() as the ulConfig parameter.
    
    \section ADC_Step_Config_Sec_Divide ADC Step Config Divide
    The Step Config value is an logic OR type of the following config:
    - Channel source select, like \b ADC_CTL_$CHx$, such as \ref ADC_CTL_CH0.  
    Or \ref ADC_CTL_TS to select the temperature sensor.
    - Comparator select to monitor the convertion value, like 
    \b ADC_CTL_$CMPx$, such as \ref ADC_CTL_CMP0.
    - single-end / diffrence input mode. ADC_CTL_D shows diffrence input mode,
    and 0 shows single-end mode.
    - \ref ADC_CTL_END shows this is the end step.
    
    @{
*/

#define ADC_CTL_TS      0x0000001A  /**< Temperature sensor select. */
#define ADC_CTL_BD      0x0000001B  /**< Bandgap select.            */
#define ADC_CTL_END     0x40000000  /**< Sequence end select.       */
#define ADC_CTL_D       0x20000000  /**< Differential select.       */
#define ADC_CTL_CH0     0x00000000  /**< Input channel 0.           */  
#define ADC_CTL_CH1     0x00000001  /**< Input channel 1.           */    
#define ADC_CTL_CH2     0x00000002  /**< Input channel 2.           */                     
#define ADC_CTL_CH3     0x00000003  /**< Input channel 3.           */                    
#define ADC_CTL_CH4     0x00000004  /**< Input channel 4.           */
#define ADC_CTL_CH5     0x00000005  /**< Input channel 5.           */
#define ADC_CTL_CH6     0x00000006  /**< Input channel 6.           */
#define ADC_CTL_CH7     0x00000007  /**< Input channel 7.           */
#define ADC_CTL_CMP0    0x00000800  /**< Select Comparator 0.       */
#define ADC_CTL_CMP1    0x00000900  /**< Select Comparator 1.       */

/** @} */

/**
    \addtogroup KL25Z_ADC_Comp_Function_Range KL25Z ADC Comparator Function Range
    \brief ADC digital comparator Function Range
    @{
*/

/** 
    Range function disabled. Only CV1 is compared.
*/
#define ADC_COMP_RANGE_CV1      0x00000000

/** 
    Range function enabled. Both CV1 and CV2 are compared.
*/
#define ADC_COMP_RANGE_CV1_CV2  0x00000080

/** @} */

/**
    \addtogroup KL25Z_ADC_Mux_Select KL25Z ADC Mux Select
    \brief ADC Mux Select
    The following are values that can be passed to the adc_mux_select()
    API as the ulMux parameter. 
    @{
*/

//! ADxxa channels are selected
#define ADC_MUX_A           0x00000000

//! ADxxb channels are selected.
#define ADC_MUX_B           0x00000001

/** @} */

/**
    \addtogroup KL25Z_ADC_Speed_Select KL25Z ADC conversion speed Select
    \brief ADC conversion speed Select
    The following are values that can be passed to the adc_conv_speed_select()
    API as the ulSpeed parameter. 
    @{
*/

//! Normal conversion sequence selected.
#define ADC_CONVERSION_SPEED_NORMAL                                           \
                                0x00000000

//! High-speed conversion sequence selected with 2 additional ADCK cycles to 
//! total conversion time.
#define ADC_CONVERSION_SPEED_HIGH                                             \
                                0x00000001

/** @} */

/**
    \addtogroup KL25Z_ADC_Comp_IDs KL25Z ADC Comparator ID
    \brief IDs/index of the ADC digital comparator.
    
    They can be passed to the flowing function as ulComp parameter:
    - adc_comp_condition_config(), ADCCompRegionSet() to configure the comparator.
    - adc_comp_enable(), adc_comp_disable() to enable/disable the comparator.
    
    @{
*/

#define ADC_COMP_0              0 /**< Analog Comparator 0. */
#define ADC_COMP_1              1 /**< Analog Comparator 1. */

/** @} */

/**
    \addtogroup KL25Z_ADC_Power_Mode KL25Z ADC Low-Power Configuration
    \brief ADC Low-Power Configuration
    The following are values that can be passed to the adc_power_mode_set()
    API as the ulPowerMode parameter. 
    @{
*/

//! Normal power configuration.
#define ADC_POWER_MODE_NORMAL   0x00000000

//! Low-power configuration.The power is reduced at the expense of maximum clock speed.
#define ADC_POWER_MODE_LOW      0x00000080

/** @} */

/**
    \addtogroup KL25Z_ADC_Sample_Time KL25Z ADC Sample Time Set.
    \brief ADC Sample Time Set.
    The following are values that can be passed to the \ref adc_primary_configuration()
    API as the ulSampleTime parameter. 
    @{
*/

#define ADC_SAMPLE_TIME_SHORT   0x00000000 /**< Short sample time. */
#define ADC_SAMPLE_TIME_LONG    0x00000010 /**< Long sample time.  */

/** @} */

/**
    \addtogroup KL25Z_ADC_Pretrigger_Mode KL25Z ADC Pretrigger Mode Set.
    \brief ADC pretrigger select Set.
    The following are values that can be passed to the adc_pretrigger_set()
    API as the ulPretrigger parameter. 
    @{
*/

#define ADC_PRE_TRIGGER_A       0x00000000 /**< Pre-trigger A. */
#define ADC_PRE_TRIGGER_B       0x00000010 /**< Pre-trigger B. */

/** @} */

/**
    \addtogroup KL25Z_ADC_Long_Sample_Time_Select KL25Z ADC Long Sample Time Select
    \brief ADC Long Sample Time Select.
    The following are values that can be passed to the adc_long_sample_time_set()
    API as the ulLongSamTime parameter. 
    @{
*/

//! Default longest sample time; 20 extra ADCK cycles; 24 ADCK cycles total.
#define ADC_LONG_SAMPLE_TIME_24 0x00000000

//! 12 extra ADCK cycles; 16 ADCK cycles total sample time.
#define ADC_LONG_SAMPLE_TIME_16 0x00000001

//! 6 extra ADCK cycles; 10 ADCK cycles total sample time.
#define ADC_LONG_SAMPLE_TIME_10 0x00000002

//! 2 extra ADCK cycles; 6 ADCK cycles total sample time.
#define ADC_LONG_SAMPLE_TIME_6  0x00000003

/** @} */

/**
    \addtogroup KL25Z_ADC_Single_Sample_Resolution_Select KL25Z ADC Single-Ended Resolution Select
    \brief ADC single-ended resolution mode Select.
    The following are values that can be passed to the adc_single_resolution_set()
    API as the ulSingleRes parameter. 
    @{
*/

#define ADC_SINGLE_RESOLUTION_8  0x00000000 /**< Single-ended 8-bit conversion.  */
#define ADC_SINGLE_RESOLUTION_10 0x00000004 /**< Single-ended 10-bit conversion. */
#define ADC_SINGLE_RESOLUTION_12 0x00000008 /**< Single-ended 12-bit conversion. */
#define ADC_SINGLE_RESOLUTION_16 0x0000000C /**< Single-ended 16-bit conversion. */
/** @} */

/**
    \addtogroup KL25Z_ADC_Differential_Sample_Resolution_Select KL25Z ADC Differential Resolution Select
    \brief ADC Differential resolution mode Select.
    The following are values that can be passed to the adc_differential_resolution_set()
    API as the ulDiffRes parameter. 
    @{
*/

#define ADC_DIFF_RESOLUTION_9   0x00000000 /**< Differential 9-bit conversion.  */
#define ADC_DIFF_RESOLUTION_13  0x00000004 /**< Differential 13-bit conversion. */
#define ADC_DIFF_RESOLUTION_11  0x00000008 /**< Differential 11-bit conversion. */
#define ADC_DIFF_RESOLUTION_16  0x0000000C /**< Differential 16-bit conversion. */                     

/** @} */

/**
    \addtogroup KL25Z_ADC_Input_Mode KL25Z ADC Input Mode
    \brief ADC analog input mode, single-end / differenctial.
    @{
*/
#define ADC_INPUT_SINGLE    0x00000000  /**< Single-end Input Mode.   */
#define ADC_INPUT_DIFF      0x00000020  /**< Differential Input Mode. */
/** @} */

/**
    \addtogroup KL25Z_ADC_Operation_Mode KL25Z ADC Operation Mode
    \brief ADC Converter Operation Mode.
    
    They can be passed to 
    adc_configure() as the ulMode parameter to configure the ADC.
    
    @{
*/

/**
    \brief Single cycle scan mode.
    
    A/D conversion will sample and convert the specified steps once in the
    sequence, from the first step to the end. Then the A/D converter 
    enters idle state.
*/
#define ADC_OP_SINGLE               0x00000000

/**
    \brief Continuous scan mode.
    
    A/D conversion will sample and convert the
    specified steps in the sequence, form the frist step to the end. and then 
    The A/D converter repeat sample and convert from the first step, 
    continuous.
    
    when the mode is set as continuous scan mode, the ADC will begin sample 
    after enable the ADC.
*/
#define ADC_OP_CONTINUOUS           0x00000008

/** @} */

/**
    \addtogroup KL25Z_ADC_Tigger_Source KL25Z ADC Tigger Source
    \brief ADC trigger Source that can start the convertion.
    
    Can be passed to 
    adc_configure() as the ulTrigger parameter to configure the ADC.
    
    \section ADC_Trigger_Source_Sec_Ext 1. al Pin Event In BMA Standard
    BMA defines macros like \b ADC_TRIGGER_EXT_$ShortPin$, show a trigger   
    source on an al pin. \b $ShortPin$ is the GPIO short pin Name like 
    \b PB8.
    
    @{
*/

#define ADC_TRIGGER_SOFTWARE        0x00000000  /**< Processor event (soft).                            */
#define ADC_TRIGGER_HARDWARE        0x00000020  /**< Processor event (hard).                            */
#define ADC_TRIGGER_EXT_PIN         0x00000000  /**< al Pin Event (such as Rising, Falling...) on PB8   */
#define ADC_TRIGGER_HSCMP0          0x00000001  /**< HSCMP0 output.                                     */
#define ADC_TRIGGER_PIT0            0x00000004  /**< PIT trigger 0.                                     */
#define ADC_TRIGGER_PIT1            0x00000005  /**< PIT trigger 1.                                     */
#define ADC_TRIGGER_TPM0            0x00000008  /**< TPM0 overflow.                                     */
#define ADC_TRIGGER_TPM1            0x00000009  /**< TPM1 overflow.                                     */
#define ADC_TRIGGER_TPM2            0x0000000A  /**< TPM2 overflow.                                     */
#define ADC_TRIGGER_RTC_ALARM       0x0000000C  /**< RTC alarm.                                         */
#define ADC_TRIGGER_RTC_SECONDS     0x0000000D  /**< RTC second.                                        */
#define ADC_TRIGGER_LPTMR0          0x0000000E  /**< LPTMR0 trigger.                                    */

/** @} */

/**
    \addtogroup KL25Z_ADC_EXT_Trigger_Mode KL25Z ADC al Tigger Mode
    \brief ADC al trigger Mode when the trigger source is al pin.
    Can be passed to the adc_configure() as the ulTrigger parameter,     
    @{
*/
/** EXT Tigger is low level. */
#define ADC_TRIGGER_EXT_LOW_LEVEL           0x00000000
/** EXT Tigger is high level. */
#define ADC_TRIGGER_EXT_HIGH_LEVEL          0x00000040
/** EXT Tigger is falling edge. */
#define ADC_TRIGGER_EXT_FALLING_EDGE        0x00000080
/** EXT Tigger is rising edge. */
#define ADC_TRIGGER_EXT_RISING_EDGE         0x000000C0                       
/** @} */

/**
    \addtogroup KL25Z_ADC_Channel7_Source KL25Z_ADC Channel 7 Source
    \brief ADC channel 7 analog input source config.
    @{
*/

//! Analog Input Channel 7 source is al analog input
#define ADC_CH7_EXT             0x00000000

//! Analog Input Channel 7 source is Internal bandgap voltage
#define ADC_CH7_INT_BV          0x00000100

//! Analog Input Channel 7 source is Internal temperature sensor
#define ADC_CH7_INT_TS          0x00000200

/** @} */

/**
    \addtogroup KL25Z_ADC_Ints KL25Z ADC Interrupt
    \brief Values that show the ADC Interrupt source.
    @{
*/

//! Interrupt after single cycle conversion
#define ADC_INT_END_CONVERSION  0x00000001

//!  Interrupt when the comparator 0 match
#define ADC_INT_COMP0           0x00000001

/** @} */

/**
    \addtogroup KL25Z_ADC_Data_Status KL25Z ADC Data Status
    \brief Check the convertion data if valid or overrun.
    @{
*/

#define ADC_DATA_VALID      0x00000001 /**< Channel n Data is valid.    */
#define ADC_DATA_OVERRUN    0x00000002 /**< Channel n Data is overrun.  */

/** @} */

/**
    \addtogroup KL25Z_ADC_Data_Resolution KL25Z ADC Data Resolution
    \brief Values that show the ADC Data Resolution (Data Mask, Data Length).
    @{
*/

#define ADC_DATA_MASK       0x00000FFF  /**< Channel n Data mask.       */
#define ADC_DATA_BIT_SIZE   12          /**< Channel n Data bit length. */

/** @} */

/**
    \addtogroup KL25Z_ADC_Comp_Conditions KL25Z ADC Comparator Interrupt Condition
    \brief Conditions when the digital comparator generate and interrupt.
    
    They can be passed to adc_comp_condition_config() as the ulConfig parameter to
    configure the comparator interrupt condition.
    
    @{
*/

//! Compare condition is less than
#define ADC_COMP_LESS_THAN      0x00000000

//! Compare condition is greater or equal
#define ADC_COMP_GREATER_EQUAL  0x00000010

/** @} */

/**
    \addtogroup KL25Z_ADC_Comp_Ref_Voltage KL25Z ADC Comparator Reference Voltage
    \brief ADC Comparator Reference Voltage.
    @{
*/
/**
    Default voltage reference pin pair, that is, al pins VREFH and VREFL.
*/
#define ADC_COMP_REF_VOL_DEFAULT   0x00000000

/**
    Alternate reference pair, that is, VALTH and VALTL.
*/
#define ADC_COMP_REF_VOL_ALTERNATE 0x00000001

/** @} */

/**
    \addtogroup KL25Z_ADC_Hardware_Average_Select KL25Z ADC Hardware Average Select
    \brief ADC Hardware Average Select.
    The following are values that can be passed to the adc_hardware_average_set()
    API as the ulAverage parameter. 
    @{
*/

#define ADC_HARDWARE_AVERAGE_4  0x00000000 /**< 4 samples averaged.  */
#define ADC_HARDWARE_AVERAGE_8  0x00000001 /**< 8 samples averaged.  */
#define ADC_HARDWARE_AVERAGE_16 0x00000002 /**< 16 samples averaged. */
#define ADC_HARDWARE_AVERAGE_32 0x00000003 /**< 32 samples averaged. */

/** @} */

/**
    \addtogroup KL25Z_ADC_Select_Input_Clock KL25Z ADC Select Input Clock
    \brief ADC Select Input Clock.
    The following are values that can be passed to the \ref adc_primary_configuration()
    API as the ulInputClock parameter.
    @{
*/
#define ADC_INPUT_BUS_CLK       0x00000000
#define ADC_INPUT_HALF_BUS_CLK  0x00000001
#define ADC_INPUT_ALT_CLK       0x00000002
#define ADC_INPUT_ASYN_CLK      0x00000003
/** @} */

/**
    \addtogroup KL25Z_ADC_Clock_Divide_Select KL25Z ADC Clock Divide Select
    \brief ADC Clock Divide Select.
    The following are values that can be passed to the \ref adc_primary_configuration().
    API as the ulClockDivide parameter. 
    @{
*/
#define ADC_INPUT_CLOCK_DIV_1       0x00000000
#define ADC_INPUT_CLOCK_DIV_2       0x00000020
#define ADC_INPUT_CLOCK_DIV_4       0x00000040
#define ADC_INPUT_CLOCK_DIV_8       0x00000060
/** @} */

/**
    \addtogroup KL25Z_ADC_Compare_Function KL25Z ADC Compare Function
    \brief ADC Compare Function.
    The following are values that can be passed to the adc_control1().
    API as the ulCompare parameter. 
    @{
*/
#define ADC_COMPARE_ENABLE          0x00000020
#define ADC_COMPARE_DISABLE         0x00000000
/** @} */

/**
    \addtogroup KL25Z_ADC_Less_Greater_Than KL25Z ADC Compare Less/Greater Than
    \brief ADC Compare Less/Greater Than.
    The following are values that can be passed to the adc_control1().
    API as the ulLessGreater parameter. 
    @{
*/
#define ADC_LESS_THAN               0x00000000
#define ADC_GREATER_THAN            0x00000010
/** @} */


/**
    \addtogroup KL25Z_ADC_DMA KL25Z ADC DMA
    \brief ADC DMA.
    The following are values that can be passed to the adc_control1().
    API as the ulAdcDMA parameter. 
    @{
*/
#define ADC_DMA_ENABLE             0x00000040
#define ADC_DMA_DISABLE            0x00000000
/** @} */

/**
    \addtogroup KL25Z_ADC_Average_Enable KL25Z ADC Average Enable
    \brief ADC Average Enable.
    The following are values that can be passed to the adc_control2().
    API as the ulAverageEnable parameter. 
    @{
*/
#define ADC_AVERAGE_ENABLE             0x00000040
#define ADC_AVERAGE_DISABLE            0x00000000
/** @} */

/**
    \addtogroup KL25Z_ADC_Exported_APIs KL25Z ADC API
    \brief KL25Z ADC API Reference
    @{
*/  
void        adc_int_call_back_init              (xtEventCallback pfnCallback);
void        adc_init                            (void);
void        adc_primary_configuration           (uint32_t ulPowerMode, uint32_t ulClockDivide, uint32_t ulSampleTime, uint32_t ulResolution, uint32_t ulInputClock);
void        adc_channel                         (uint32_t ulInputChannel);
void        adc_control_1                       (uint32_t ulTrigger, uint32_t ulCompare, uint32_t ulLessGreater, uint32_t ulCompareRange, uint32_t ulAdcDMA, uint32_t ulVoltageRef);
void        adc_control_2                       (uint32_t ulConversionMode, uint32_t ulAverageEnable, uint32_t ulAverageSelect);
void        adc_control_channel                 (uint32_t ulDiffMode, uint32_t ulInputChannel);
void        adc_configure                       (uint32_t ulChannel, uint32_t ulInputMode, uint32_t ulOpMode, uint32_t ulTrigger);
void        adc_step_configure                  (uint32_t ulChannel, uint32_t ulStep, uint32_t ulConfig); 
void        adc_comp_condition_config           (uint32_t ulCompID, uint32_t ulConfig);
void        adc_diff_channel_enable             (uint32_t ulChannel);
void        adc_single_ended_channel_enable     (uint32_t ulChannel);
void        adc_int_enable                      (void);
void        adc_int_disable                     (void);
boolean     adc_calibration_is_failed           (void);
void        adc_calibration_failed_flag_clear   (void);
void        adc_processor_trigger               (void);
void        adc_convert_stop                    (void);
uint32_t    adc_data_get_block                  (void);
boolean     adc_data_is_ready                   (void);
uint32_t    adc_data_get                        (void);
uint32_t    adc_offset_data_get                 (void);
void        adc_calibration_enable              (void);
void        adc_calibration_disable             (void);
void        adc_asyn_clk_output_enable          (void);
void        adc_asyn_clk_output_disable         (void);
boolean     adc_calibration_done                (void);
void        adc_ref_voltage_set                 (uint32_t ulRefVol);
void        adc_hardware_average_enable         (void);
void        adc_hardware_average_disable        (void);
void        adc_hardware_average_set            (uint32_t ulAverage);
void        adc_comp_data_set                   (uint32_t ulLowRef, uint32_t ulHighRef);
void        adc_comp_enable                     (void);
void        adc_comp_disable                    (void);
void        adc_comp_func_range_set             (uint32_t ulRange);
void        adc_dma_enable                      (void);
void        adc_dma_disable                     (void);
void        adc_power_mode_set                  (uint32_t ulPowerMode);
void        adc_sample_time_set                 (uint32_t ulSampleTime);
void        adc_gain_set                        (uint32_t ulMinusGain, uint32_t ulPlusGain);
void        adc_long_sample_time_set            (uint32_t ulLongSamTime);
void        adc_single_resolution_set           (uint32_t ulSingleRes);
void        adc_differential_resolution_set     (uint32_t ulDiffRes);
void        adc_conv_speed_select               (uint32_t ulSpeed);
void        adc_mux_select                      (uint32_t ulMux);
void        adc_bandgap_configure               (uint32_t ulInputMode);
void        adc_temperature_sensor_configure    (uint32_t ulInputMode);
void        adc_alternate_trigger_enable        (void);
void        adc_tpm_trigger_enable              (void);
void        adc_pretrigger_set                  (uint32_t ulPretrigger);
void        adc_input_clk_select                (uint32_t ulClock);
boolean     adc_conv_in_progress                (void);
/** @} */

/** @} */

#endif // __ADC_H__
