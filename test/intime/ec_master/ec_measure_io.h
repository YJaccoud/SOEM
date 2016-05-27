#ifndef _EC_MEASURE_IO_H
#define _EC_MEASURE_IO_H

#include "osal.h"

#ifdef __cplusplus
extern "C"
{
#endif

    //for measures and writes(global)
    #define INPUT_ANALOG_OFFSET 2   //2: EL3104 only

    #define MAXIMUM_RIGHT_VALUE 7

    #define CONVERSION_COEF     3276.7

    #define EXCEL_FILE_NAME_IO     "C:\\VisualStudioProject\\NIC_jitter\\jitter_excel_data.csv"
    #define TXT_FILE_NAME_IO       "C:\\VisualStudioProject\\NIC_jitter\\jitter_txt_data.txt"

    //for measure and write (MeasureAndWriteFile())
    #define SAMPLE_NUMBER                  1400000  // 1'400'000

    //for measure and write double buffer (MeasureDoubleBuffer() & WriteDoubleBuffer())
    #define NUMBER_DOUBLE_BUFFER             576       //2 for fast test  //28 * 50'000 = 1'400'000  //576 * 50'000 = 28'800'000 = 8h
    #define SAMPLE_NUMBER_DOUBLE_BUFFER    50000

    void MeasureDoubleBuffer(void* pInput);
    void WriteDoubleBuffer();

    void MeasureAndWriteFile(void* pInput);

#ifdef __cplusplus
}
#endif

#endif /* _EC_ECATDC_H */