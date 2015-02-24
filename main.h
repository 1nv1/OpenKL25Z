/**
    \file main.h
    \version 0.0.1
    \date 2014-07-20
    \brief Simple demonstration program. Based on demo.c of 
    Andrew Payne <andy@payne.org>. The spirit of demo is show 
    functionality and create a base for yours projects.
    \author Nelson Lombardo
    \license This file is released under the MIT License.
    \include LICENSE
*/
#ifndef _MAIN_H_
#define _MAIN_H_

/** \brief Main buffer of the system. */
enum sysBufferIdx
{
    idxADC0 = 0,
    idxADC1,
    idxADC2,
    idxADC3,
    idxADC4,
    idxADC5,
    idxAccelX,
    idxAccelY,
    idxAccelZ,
    idxTouchDataX,
    idxTouchDataY
};

#define SYS_BUFFER_LAST 10
extern uint32_t sysBuffer[11]; 
#endif
