#ifndef _EC_INITIALIZE_SLAVES_H
#define _EC_INITIALIZE_SLAVES_H

#include "osal.h"

#ifdef __cplusplus
extern "C"
{
#endif

void InitializeSlavesPoToSo();
void InitializeSlavesSoToOp(OSAL_EVENT_HANDLE * timerEvent);

#ifdef __cplusplus
}
#endif

#endif /* _EC_INITIALIZE_SLAVES_H */