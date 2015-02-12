/**
    \file debug.h
    \brief Macros for assisting debug of the peripheral library.
    \version V0.0.3
    \date 08/01/2014
    \author Nelson Lombardo
*/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

/**
    \addtogroup Development_Tools
    \brief      Some APIs for make _easy_ the development with the
    framework.
    @{
    
*/

/**
    \addtogroup Debug
    \brief      Provided some assert macros to help debug.
                This module provides a macro called ASSERT, Used to assert
                some conditions if is correct.
    \section    Debug_When When User the Debug feature?
                - Verify the legitimacy of the parameters
                - Judge execution of the accuracy of the results
                - where you want to determine if actual == expected ?
    \section    Debug_How How to use the Debug Feature?
                - Enable the debug feature by doing a DEBUG build.
                - add the ASSERT where you want.
    @{
*/

/**
    \addtogroup Debug_Exported_APIs Debug API
    \brief      Debug API Reference.
    @{
*/

/**
    \brief  Error Handler function.
    This function can be used to print error file name and error line
    number. 
    \param pcFilename is a string pointer point to error filename.
    \param ulLine is the error line number.
    \param ulFunc Function from error born.
    \param ulParam Parameter with conflict (const).
    \return None.
*/

extern void __xerror__(char *pcFilename, unsigned long ulLine, char *ulFunc, unsigned long ulParam);

/**
    \brief  The ASSERT macro.
    It does the actual assertion checking. Typically, this will be for
    procedure arguments.
    \param  [in] expr is the expression to be check and parameter with conflict.
    \param param Is the number of conflict parameter.
*/
#ifdef DEBUG
#define ASSERT(expr, param)                                             \
        {                                                               \
            if(!(expr))                                                 \
            {                                                           \
                __xerror__(__FILE__, __LINE__, __FUNCTION__, param);    \
            }                                                           \
        }
#else
// If wasn't define, then nothing to do here...
#define ASSERT(expr, param) 
#endif

/** @} */
/** @} */
/** @} */

#endif // __DEBUG_H__

