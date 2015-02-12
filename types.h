/**
    \file types.h
    \version 0.0.2
    \date 2014-07-20
    \brief Some extra data types.
    \author Nelson Lombardo
 */

typedef unsigned char boolean; /**< Define a boolean type, value can be \b xTRUE and \b xFALSE. */


#ifndef TRUE
#define TRUE 1  /**< True state. */
#endif

#ifndef FALSE
#define FALSE 0 /**< False state. */
#endif

/**
    Event callback function.
    
    \param pvCBData is the callback pointer associated with the instance
    generating the callback.  This is a value provided by the client during
    initialization of the instance making the callback.
    \param ulEvent is the identifier of the asynchronous event which is being
    notified to the client.
    \param ulMsgParam is an event-specific parameter.
    \param pvMsgData is an event-specific data pointer.
    
    A function pointer provided to the Cox layer by the application
    which will be called to notify it of all asynchronous events relating to
    data transmission or reception.
    
    \return Returns an event-dependent value.
*/
typedef unsigned long (* xtEventCallback)(
    void *pvCBData,
    unsigned long ulEvent,
    unsigned long ulMsgParam,
    void *pvMsgData
);
