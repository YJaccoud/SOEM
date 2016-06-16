#ifndef _EC_TEST_TOOLS_H
#define _EC_TEST_TOOLS_H

#include "ethercat.h"

#ifdef __cplusplus
extern "C"
{
#endif

//init call
void PrintMasterState();
void PrintSlavesState();
void PrintSlavesIds();
void PrintEepromsPulseLength();

//cyclic process call
void PrintRegister();
void PrintCyclicInt32(int32 data, uint32 cycleUpdate);
void MeasureJitterOscillo();

#ifdef __cplusplus
}
#endif

#endif /* _EC_TEST_TOOLS_H */