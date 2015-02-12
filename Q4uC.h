#include "stdint.h"

#ifndef _Q4UC_H_
#define _Q4UC_H_

/** \brief Q4uC Header file
 **
 ** Header file for Q4uC
 **
 **/

/** \addtogroup _Q4uC Queue for micro-controllers
 ** @{ */

/** \brief Maximum number of elements inside queue (max. msg.) */ 
#define QUEUE_SIZE 20

/** \brief Indicates that any process (produce or consume) is in use */ 
#define Q4UC_STATUS_IN_USE      1

/** \brief Indicates that all process (produce and consume) is free */ 
#define Q4UC_STATUS_RELEASE     0

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

/** \brief The queue object */
typedef struct _Q4uC_Queue {
    uint8_t Events [QUEUE_SIZE];    /** <- Message store */
    uint8_t IdxStart;               /** <- Index starts */
    uint8_t IdxEnd;                 /** <- Index ends */
    uint8_t IdxLen;                 /** <- Index length */ 
    uint8_t Status;                 /** <- Status of queue */
} Q4uC_Queue;

/** \brief Initialize the Q4uC
 **
 **  Performs initialization of parameters
 **
 ** \param[in] Q Queue object
 ** 
 ** \remarks Mandatory for the correct use of Q4uC 
 **/
extern void Q4uC_Init (Q4uC_Queue *Q);

/** \brief Consume message
 **
 **  Performs the consumed of messages inside the queue.
 **
 ** \param[in] Q Queue object
 ** \return The quantity of messages remaining. -1 if failed.
 **/
extern int8_t Q4uC_Consume (Q4uC_Queue *Q);

/** \brief Produce message
 **
 **  Put a message in the queue.
 **
 ** \param[in] Q Queue object
 ** \param[in] msg Message to enqueue
 ** \return If the process was succes return 0, -1 if failed.
 **/
extern int8_t Q4uC_Produce (Q4uC_Queue *Q, uint8_t msg);

/** \brief Peek queue
 **
 **  Peek queue length.
 **
 ** \param[in] Q Queue object
 ** \return The message quantity.
 **/
extern int8_t Q4uC_Peek (Q4uC_Queue *Q);

/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _Q4UC_H_ */
