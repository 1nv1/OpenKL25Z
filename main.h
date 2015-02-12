#ifndef _MAIN_H_
#define _MAIN_H_
enum sysBufferIdx
{
    idxADC0,
    idxADC1,
    idxAccelX,
    idxAccelY,
    idxAccelZ,
    idxTouchDataX,
    idxTouchDataY
};

extern uint32_t sysBuffer[7]; 
#endif
