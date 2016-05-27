#ifndef _EC_MEASURE_REG_H
#define _EC_MEASURE_REG_H

#include "ethercattype.h"

#ifdef __cplusplus
extern "C"
{
#endif

    #define REG_LOCAL_TIME_ADDR       0x910
    #define REG_NEXT_SYNC0_ADDR       0x990
    //#define REG_NEXT_SYNC1_ADDR       0x998

    #define EXCEL_FILE_NAME_REG           "C:\\VisualStudioProject\\Reg_Value\\reg_excel_data.csv"
    #define TXT_FILE_NAME_REG             "C:\\VisualStudioProject\\Reg_Value\\reg_txt_data.txt"

    #define SAMPLE_NUMBER_BUFFER_REG  60000

    void MeasureBufferReg(uint16 slave);
    void WriteBufferReg();

#ifdef __cplusplus
}
#endif

#endif /* _EC_ECATDC_H */