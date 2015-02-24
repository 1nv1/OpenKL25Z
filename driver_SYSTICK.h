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

#define CLOCK_SOURCE_REFERENCE 0
#define CLOCK_SOURCE_PROCESSOR 1

// Low Power Timer Control Status Register (SYSTICK_CSR)
// Address: 0x0000_003Ch base + 0h offset = 0x0000_003Ch
/**
    \brief Enables the counter.
    \param enable 0 = counter disabled, 1 = counter enabled
*/
extern inline void systick_enable (const uint32_t enable) {
    SYST_CSR &= ~SysTick_CSR_ENABLE_MASK;
    SYST_CSR |= (SysTick_CSR_ENABLE_MASK & (enable << SysTick_CSR_ENABLE_SHIFT));
};

/**
    \brief Enables SysTick exception request.
    \param enable 0 = counting down to zero does not assert the SysTick
    exception request, 1 = counting down to zero to asserts the SysTick 
    exception request
*/
extern inline void systick_int (const uint32_t enable) {
    SYST_CSR &= ~SysTick_CSR_TICKINT_MASK;
    SYST_CSR |= (SysTick_CSR_TICKINT_MASK & (enable << SysTick_CSR_TICKINT_SHIFT));
}

/**
    \brief Selects the SysTick timer clock source.
    \param clksrc 0 = reference clock, 1 = processor clock.
    If your device does not implement a reference clock, this bit 
    reads-as-one and ignores writes.
*/
extern inline void systick_clock_source (const uint32_t clksrc) {
    SYST_CSR &= ~SysTick_CSR_CLKSOURCE_MASK;
    SYST_CSR |= (SysTick_CSR_CLKSOURCE_MASK & (clksrc << SysTick_CSR_CLKSOURCE_SHIFT));
}

/**
    \brief Returns 1 if timer counted to 0 since the last read of this register.
*/
extern inline uint32_t systick_counter_flag (void) {
    return ((uint32_t)((SYST_CSR & ~SysTick_CSR_COUNTFLAG_MASK) >> SysTick_CSR_COUNTFLAG_SHIFT));
}

// SysTick Reload Value Register
/**
    \brief Value to load into the SYST_CVR when the counter is enabled 
    and when it reaches 0, see Calculating the RELOAD value.
    \param VALUE Value to load
    \note Calculating the RELOAD value
    The RELOAD value can be any value in the range 0x00000001-0x00FFFFFF.
    You can program a value of 0, but this has no effect because the
    SysTick exception request and COUNTFLAG are activated when counting
    from 1 to 0.
    To generate a multi-shot timer with a period of N processor clock
    cycles, use a RELOAD value of N-1. For example, if the SysTick
    interrupt is required every 100 clock pulses, set RELOAD to 99.
*/
extern inline void systick_reload_value (uint32_t value) {
    SysTick_RVR_RELOAD (value);
}

//  SysTick Current Value Register
/**
    \brief The SYST_CVR contains the current value of the SysTick counter.
*/
extern inline void systick_counter_value (uint32_t value) {
    SYST_CVR &= ~SysTick_CVR_CURRENT_MASK;
    SYST_CVR |= (SysTick_CVR_CURRENT_MASK & (value << SysTick_CVR_CURRENT_SHIFT));
}

// SysTick Calibration Value Register
/**
    \brief Indicates whether the device provides a reference clock to
    the processor.
    \return 0 = reference clock provided, 1 = reference clock provided.
    \note If your device does not provide a reference clock, the SYST_CSR.
    CLKSOURCE bit reads-as-one and ignores writes.
*/
extern inline uint32_t systick_no_ref (void) {
    return ((uint32_t)((SYST_CALIB & SysTick_CALIB_NOREF_MASK) >> SysTick_CALIB_NOREF_SHIFT));
}

/**
    \brief Indicates whether the TENMS value is exact.
    \return 0 = TENMS value is exact, 1 = TENMS value is inexact, or not
    given.
    \note An inexact TENMS value can affect the suitability of SysTick
    as a software real time clock.
*/
extern inline uint32_t systick_skew (void) {
    return ((uint32_t)((SYST_CALIB & SysTick_CALIB_SKEW_MASK) >> SysTick_CALIB_SKEW_SHIFT));
}

/** 
    \brief Reload value for 10ms (100Hz) timing, subject to system clock
    skew errors.
    \return Value for 10ms (100Hz).
    \note If the value reads as zero, the calibration value is not known.
*/
extern inline uint32_t systick_tenms (void) {
    return ((uint32_t)((SYST_CALIB & SysTick_CALIB_TENMS_MASK) >> SysTick_CALIB_TENMS_SHIFT));
}

/** @} */ // KL25Z_SYSTICK

#endif  // _DRIVER_SYSTICK_H_
