#ifndef _EC_INITIALIZE_SLAVES_H
#define _EC_INITIALIZE_SLAVES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "osal.h"

void InitializeSlavesPoToSo();
//void InitializeSlavesSoToOp(OSAL_EVENT_HANDLE * timerEvent);
void InitializeSlavesSoToOp(void ** timerEvent);

#ifdef __cplusplus
}
#endif

#endif /* _EC_INITIALIZE_SLAVES_H */