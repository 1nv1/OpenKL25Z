/** \brief Q4uC Source file
 **
 ** Source file for Q4uC
 **
 **/

#include "Q4uC.h"

void Q4uC_Init (Q4uC_Queue *Q)
{
    uint8_t i;
    Q->IdxStart = 0;
    Q->IdxEnd = 0;
    Q->IdxLen = 0;
    /* Cleanup messages queue. */
    for (i = 0; i != QUEUE_SIZE; i++) {
        Q->Events[idx] = 0;
    }
    Q->Status = Q4UC_STATUS_RELEASE;
}

int8_t Q4uC_Consume (Q4uC_Queue *Q)
{ 
    uint8_t E;
    if (Q->IdxLen == 0) {
        return (0); /* No messages in the queue. */
    }
    if (Q->IdxEnd != Q->IdxStart)
    {
        E = Q->Events[Q->IdxStart];
        Q->IdxStart++;
        if (Q->IdxStart > QUEUE_SIZE) Q->IdxStart = 0;
        Q->IdxLen--;
        return (E);
    }
    else return (0);
}

int8_t Q4uC_Produce (Q4uC_Queue *Q, uint8_t msg)
{
    /* Valid message? */
    if (msg == 0) return (-1);
    Q->Status = Q4UC_STATUS_IN_USE;
    if (Q->IdxLen != QUEUE_SIZE)
    {
        Q->Events[Q->IdxEnd] = msg;
        Q->IdxEnd++;
        if (Q->IdxEnd > QUEUE_SIZE) Q->IdxEnd = 0;
        Q->IdxLen++;
    }
    else return (-1); /* Failed */
    Q->Status = Q4UC_STATUS_RELEASE;
    return (0); /* Success */
}

int8_t Q4uC_Peek (Q4uC_Queue *Q) {
    return (Q->IdxLen);
}
