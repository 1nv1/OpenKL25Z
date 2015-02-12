/**
    \file debug.c
    \brief Drivers for assisting debug of the peripheral library.
    \version V0.0.3
    \date 08/01/2014
    \author Nelson Lombardo
*/

#include "debug.h"

/**
    \brief Error Function to be called when assert runs FALSE.
    
    \param pcFilename is the current file name.
    \param ulLine is the current line number.
    \param ulFunc is the current function were fake error is present.
    
    This is just an default error handler function.
    
    \note This is only used when doing a \b DEBUG build.
    
    \return None.
    
*/
#ifdef DEBUG
void __xerror__(char *pcFilename, unsigned long ulLine, char *ulFunc, unsigned long ulParam)
{
    iprintf("\n**********\n" \
            "This fake error is in file «%s» on line %d at function «%s()».\n" \
            "Confict with parameter %d.\n" \
            "**********\n" \
            , pcFilename, ulLine, ulFunc, ulParam);
    #ifdef PANIC
    iprintf("PANIC!!! Stop execution\n");
    for (;;);
    #endif
}
#endif

