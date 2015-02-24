/**
    \file driver_LPTMR.h
    \version 0.1.0
    \date 2015-02-19
    \brief Defines and Macros for LPTMR API.
    \author Nelson Lombardo
    \license This file is released under the MIT License.
    \include LICENSE
 */

#ifndef _DRIVER_SYSTICK_H_
#define _DRIVER_SYSTICK_H_

#include <stdint.h>
#include "types.h"
#include "OpenKL25Z.h"
#include "debug.h"
        
/**
    \addtogroup KL25Z_SYSTICK
    @{
*/

// Low Power Timer Control Status Register (SYSTICK_CSR)
// Address: 0x0000_003Ch base + 0h offset = 0x0000_003Ch

/**
    \brief Enables the counter.
    0 = counter disabled
    1 = counter enabled
*/
#define systick_enable(x)                                               \
    SYST_CSR &= ~SysTick_CSR_ENABLE_MASK;                               \
    SYST_CSR |= (SysTick_CSR_ENABLE_MASK & ((uint32_t)(x) << SysTick_CSR_ENABLE_SHIFT));

/**
    \brief Enables SysTick exception request.
    0 = counting down to zero does not assert the SysTick exception request
    1 = counting down to zero to asserts the SysTick exception request
*/
#define systick_int(x)                                                  \
        SYST_CSR &= ~SysTick_CSR_TICKINT_MASK;                          \
        SYST_CSR |= (SysTick_CSR_TICKINT_MASK & ((uint32_t)(x) << SysTick_CSR_TICKINT_SHIFT));
    
/**
    \brief Selects the SysTick timer clock source.
    0 = reference clock
    1 = processor clock
    If your device does not implement a reference clock, this bit reads-as-one and ignores writes.
*/
#define systick_clock_source(x)                                         \
        SYST_CSR &= ~SysTick_CSR_CLKSOURCE_MASK;                        \
        SYST_CSR |= (SysTick_CSR_CLKSOURCE_MASK & ((uint32_t)(x) << SysTick_CSR_CLKSOURCE_SHIFT));

#define CLOCK_SOURCE_REFERENCE 0
#define CLOCK_SOURCE_PROCESSOR 1

/**
    \brief Returns 1 if timer counted to 0 since the last read of this register.
*/
#define systick_counter_flag()                                          \
        ((uint32_t)((SYST_CSR & ~SysTick_CSR_COUNTFLAG_MASK) >> SysTick_CSR_COUNTFLAG_SHIFT));

// SysTick Reload Value Register
/**
    \brief Value to load into the SYST_CVR when the counter is enabled 
    and when it reaches 0, see Calculating the RELOAD value.
    \note Calculating the RELOAD value.
    The RELOAD value can be any value in the range 0x00000001-0x00FFFFFF.
    You can program a value of 0, but this has no effect because the
    SysTick exception request and COUNTFLAG are activated when counting
    from 1 to 0.
    To generate a multi-shot timer with a period of N processor clock
    cycles, use a RELOAD value of N-1. For example, if the SysTick
    interrupt is required every 100 clock pulses, set RELOAD to 99.
*/
#define systick_reload_value(x)                                         \
        SysTick_RVR_RELOAD(x);

//  SysTick Current Value Register
/**
    \brief The SYST_CVR contains the current value of the SysTick counter.
*/
#define systick_counter_value(x)                                        \
        SYST_CVR &= ~SysTick_CVR_CURRENT_MASK;                          \
        SYST_CVR |= (SysTick_CVR_CURRENT_MASK & ((uint32_t)(x) << SysTick_CVR_CURRENT_SHIFT));

//        ((uint32_t)((SYST_CVR & SysTick_CVR_CURRENT_MASK) >> SysTick_CVR_CURRENT_SHIFT));

// SysTick Calibration Value Register
/**
    \brief Indicates whether the device provides a reference clock to
    the processor.
    0 = reference clock provided
    1 = reference clock provided.
    If your device does not provide a reference clock, the SYST_CSR.
    CLKSOURCE bit reads-as-one and ignores writes.
*/
#define systick_no_ref(x)                                               \
        SYST_CALIB &= ~SysTick_CALIB_NOREF_MASK;                        \
        SYST_CALIB |= (SysTick_CALIB_NOREF_MASK & ((uint32_t)(x) << SysTick_CALIB_NOREF_SHIFT));
        
/**
    \brief Indicates whether the TENMS value is exact.
    0 = TENMS value is exact
    1 = TENMS value is inexact, or not given.
    An inexact TENMS value can affect the suitability of SysTick as a
    software real time clock.
*/
#define systick_skew()                                                  \
    ((uint32_t)((SYST_CALIB & SysTick_CALIB_SKEW_MASK) >> SysTick_CALIB_SKEW_SHIFT))


/** 
    \brief Reload value for 10ms (100Hz) timing, subject to system clock
    skew errors.
    If the value reads as zero, the calibration value is not known.
*/
#define systick_tenms()                                                \
    ((uint32_t)((SYST_CALIB & SysTick_CALIB_TENMS_MASK) >> SysTick_CALIB_TENMS_SHIFT))

/** @} */ // KL25Z_SYSTICK

#endif  // _DRIVER_SYSTICK_H_
