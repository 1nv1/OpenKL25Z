/**
 * \file freedom.h
 * \brief Definitions for Freescale Freedom development board
 * \date 2012-2013
 * \author Andrew Payne <andy@payne.org>
 * \license This file is released under the MIT License.
 * \include LICENSE
 */
#ifndef _FREEDOM_H_
#define _FREEDOM_H_

#include "OpenKL25Z.h"                    // CPU definitions

#define CORE_CLOCK          48000000    /**< Core clock speed. */

/**
 * \brief Control of RGB LED on board.
 * \param red Intensity of red colour.
 * \param green Intensity of green colour.
 * \param blue Intensity of blue colour.
 */
static inline void RGB_LED(int red, int green, int blue) {
    TPM2_C0V  = red;
    TPM2_C1V  = green;
    TPM0_C1V  = blue;
}
#endif
