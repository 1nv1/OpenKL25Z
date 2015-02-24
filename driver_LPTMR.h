/**
    \file driver_LPTMR.h
    \version 0.1.0
    \date 2015-02-19
    \brief Defines and Macros for LPTMR API.
    \author Nelson Lombardo
    \license This file is released under the MIT License.
    \include LICENSE
 */

#ifndef _DRIVER_LPTMR_H_
#define _DRIVER_LPTMR_H_

#include <stdint.h>
#include "types.h"
#include "OpenKL25Z.h"
#include "debug.h"
        
/**
    \addtogroup KL25Z_LPTMR
    @{
*/

#define lptmr_init() \
        SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

#define LPTMR_PULSE_COUNTER_INPUT_0 0
#define LPTMR_PULSE_COUNTER_INPUT_1 1
#define LPTMR_PULSE_COUNTER_INPUT_2 2
#define LPTMR_PULSE_COUNTER_INPUT_3 3

// Low Power Timer Control Status Register (LPTMRx_CSR)
// Address: 4004_0000h base + 0h offset = 4004_0000h

/**
    \brief Timer Compare Flag. 
    TCF is set when the LPTMR is enabled and the CNR equals the CMR and increments.
    TCF is cleared when the LPTMR is disabled or a logic 1 is written to it.
*/
#define lptmr_compare_flag() \
    ((uint32_t)((LPTMR0_CSR & LPTMR_CSR_TCF_MASK) >> LPTMR_CSR_TCF_SHIFT))

/**
    \brief Timer Interrupt Enable. 
    When TIE is set, the LPTMR Interrupt is generated whenever TCF is also set.
*/
#define lptmr_interrupt_enable(x)                                       \
        LPTMR0_CSR &= ~LPTMR_CSR_TIE_MASK;                              \
        LPTMR0_CSR |= (LPTMR_CSR_TIE_MASK & ((uint32_t)(x)<< LPTMR_CSR_TIE_SHIFT));

/**
    \brief Timer Pin Select.
    Configures the input source to be used in Pulse Counter mode. 
    TPS must be altered only when the LPTMR is disabled.
    The input connections vary by device.
    See the chip configuration details for information on the connections to these inputs.
*/
#define lptmr_pin_select(x)                                             \
        LPTMR0_CSR &= ~LPTMR_CSR_TPS_MASK;                              \
        LPTMR0_CSR |= LPTMR_CSR_TPS(x);

/**
    \brief Timer Pin Polarity.
    Configures the polarity of the input source in Pulse Counter mode. 
    TPP must be changed only when the LPTMR is disabled.
*/
#define lptmr_pin_polarity(x)                                           \
        LPTMR0_CSR &= ~LPTMR_CSR_TPP_MASK;                              \
        LPTMR0_CSR |= LPTMR_CSR_TPP_MASK & ((uint32_t)(x) << LPTMR_CSR_TPP_SHIFT);

/**
    \brief Timer Free-Running Counter.
    When clear, TFC configures the CNR to reset whenever TCF is set. 
    When set, TFC configures the CNR to reset on overflow. 
    TFC must be altered only when the LPTMR is disabled.
*/
#define lptmr_free_running_counter(x)                                   \
        LPTMR0_CSR &= ~LPTMR_CSR_TFC_MASK;                              \
        LPTMR0_CSR |= LPTMR_CSR_TFC_MASK & ((uint32_t)(x) << LPTMR_CSR_TFC_SHIFT);

#define LPTMR_COUNTER_RESET_WITH_TCF    0
#define LPTMR_COUNTER_RESET_OVERFLOW    1

/**
    \brief Timer Mode Select.
    Configures the mode of the LPTMR. 
    TMS must be altered only when the LPTMR is disabled.
*/
#define lptmr_mode_select(x)                                            \
        LPTMR0_CSR &= ~LPTMR_CSR_TMS_MASK;                              \
        LPTMR0_CSR |= LPTMR_CSR_TMS_MASK & (x << LPTMR_CSR_TMS_SHIFT);

/**
    \brief Timer Enable.
    When TEN is clear, it resets the LPTMR internal logic, including the CNR and TCF. 
    When TEN is set, the LPTMR is enabled. 
    While writing 1 to this field, CSR[5:1] must not be altered.
*/
#define lptmr_enable(x)                                                 \
        LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;                              \
        LPTMR0_CSR |= LPTMR_CSR_TEN_MASK & ((uint32_t)(x) << LPTMR_CSR_TEN_SHIFT);

#define LPTMR_TIME_COUNTER_MODE     0
#define LPTMR_PULSE_COUNTER_MODE    1

// Low Power Timer Prescale Register (LPTMRx_PSR)
// Address: 4004_0000h base + 4h offset = 4004_0004h

/**
    \brief Prescale Value. 
    Configures the size of the Prescaler in Time Counter mode or width of the glitch filter in Pulse Counter mode. 
    PRESCALE must be altered only when the LPTMR is disabled.
*/
#define lptmr_prescale(x)                                               \
        LPTMR0_PSR &= ~LPTMR_PSR_PRESCALE_MASK;                         \
        LPTMR0_PSR |= LPTMR_PSR_PRESCALE(x);

/**
    \brief Prescaler Bypass. 
    When PBYP is set, the selected prescaler clock in Time Counter mode or selected input source in Pulse
Counter mode directly clocks the CNR. 
    When PBYP is clear, the CNR is clocked by the output of the prescaler/glitch filter.
    PBYP must be altered only when the LPTMR is disabled.
*/
#define lptmr_prescale_bypass(x)                                        \
        LPTMR0_PSR &= ~LPTMR_PSR_PBYP_MASK;                             \
        LPTMR0_PSR |= LPTMR_PSR_PBYP_MASK & ((uint32_t)(x) << LPTMR_PSR_PBYP_SHIFT);

/**
    \brief Prescaler Clock Select. 
    Selects the clock to be used by the LPTMR prescaler/glitch filter. 
    PCS must be altered only when the LPTMR is disabled. 
    The clock connections vary by device.
    \note See the chip configuration details for information on the connections to these inputs.
*/
#define lptmr_prescale_clock_select(x)                                  \
        LPTMR0_PSR &= ~LPTMR_PSR_PCS_MASK;                              \
        LPTMR0_PSR |= LPTMR_PSR_PCS(x);

// Low Power Timer Compare Register (LPTMRx_CMR)
// Address: 4004_0000h base + 8h offset = 4004_0008h

/**
    \brief Compare Value. 
    When the LPTMR is enabled and the CNR equals the value in the CMR and increments, TCF is set and
the hardware trigger asserts until the next time the CNR increments. 
    If the CMR is 0, the hardware trigger will remain asserted until the LPTMR is disabled.
    If the LPTMR is enabled, the CMR must be altered only when TCF is set.
*/
#define lptmr_compare_value(x)                                          \
        LPTMR0_CMR &= ~LPTMR_CMR_COMPARE_MASK;                          \
        LPTMR0_CMR |= LPTMR_CMR_COMPARE(x);


// Low Power Timer Counter Register (LPTMRx_CNR)
// Address: 4004_0000h base + Ch offset = 4004_000Ch

/**
    \brief Counter Value.
*/
#define lptmr_counter_value(x)                                          \
        LPTMR0_CNR &= ~LPTMR_CNR_COUNTER_MASK;                          \
        LPTMR0_CNR |= LPTMR_CNR_COUNTER(x);

/** @} */ // KL25Z_LPTMR

#endif  // _DRIVER_LPTMR_H_
