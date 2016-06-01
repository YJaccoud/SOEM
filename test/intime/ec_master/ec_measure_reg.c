
#include "ec_measure_reg.h"

#include <stdio.h>
#include <string.h>
#include "oshw.h"
#include "osal.h"
#include "ethercatbase.h"

static int64   measureBufferSync0[SAMPLE_NUMBER_BUFFER_REG];
static int64   measureBufferSync1[SAMPLE_NUMBER_BUFFER_REG];
static int64   measureBufferLocalTime[SAMPLE_NUMBER_BUFFER_REG];

static boolean  measureBufferSyncFinished = FALSE;

void MeasureBufferReg(uint16 slave)
{
    static unsigned long state = 0;

    static uint32 i = 0;
    static unsigned long flag = 0;

    static int64 registersSync[10];
    static int64 registerTime;

    switch (state)
    {
    case 0: //Wait 5000 cycles in OP before measure
        i++;
        if (i >= 5000)
        {
            i = 0;
            state = 1;
        }
        break;
    case 1:
        ec_FPRD(0x1000 + slave, REG_NEXT_SYNC0_ADDR, 16, registersSync, EC_TIMEOUTRET3);
        ec_FPRD(0x1000 + slave, REG_LOCAL_TIME_ADDR, 8, &registerTime, EC_TIMEOUTRET3);
        
        
        measureBufferSync0[i] = registersSync[0];
        measureBufferSync1[i] = registersSync[1];
        
        measureBufferLocalTime[i] = registerTime;
        
        i++;
        if (i == SAMPLE_NUMBER_BUFFER_REG)
        {
          i = 0;
          state = 100;
        }
        break;
    case 100:
        printf("\ntest yja : Measure reg finished !!\n\n");
        measureBufferSyncFinished = 1;
        state = 101;
        break;
    case 101: //wait
        break;
    }
}

void WriteBufferReg()
{
    static FILE*  txtFile = NULL;
    static FILE*  excelFile = NULL;

    static unsigned long state = 0;
    static unsigned long stateError = 99;

    static uint32 i = 0;

    switch (state)
    {
    case 0:
        excelFile = fopen(EXCEL_FILE_NAME_REG, "w");
        if (excelFile != NULL)
            state = 1;
        else {
            stateError = 0;
            state = 99;
        }
        break;
    case 1:
        txtFile = fopen(TXT_FILE_NAME_REG, "w");
        if (txtFile != NULL)
            state = 2;
        else {
            stateError = 1;
            state = 99;
        }
        break;
    case 2:
        if (measureBufferSyncFinished == FALSE)
          break;
        int64 diffSync0 = measureBufferSync0[0] - measureBufferSync0[0];
        int64 diffSync1 = measureBufferSync1[0] - measureBufferSync1[0];
        int64 diffSync = measureBufferSync1[0] - measureBufferSync0[0];

        int64 diffTimeSync0 = measureBufferSync0[0] - measureBufferLocalTime[0];
        int64 diffTimeSync1 = measureBufferSync1[0] - measureBufferLocalTime[0];

        fprintf(excelFile, "Sample;Local Time;Next Sync0;Diff Sync0;Next Sync1;Diff Sync1;Diff Sync;\n");
        fprintf(txtFile, "Sample ; \t         Local Time ; \tLocal time Next Sy0 ; \tDiff Sy0 ; \tLocal time Next Sy1 ; \tDiff Sy1 ; \t Diff Sy ; \tNext Sy0 ; \tNext Sy1 ;\n");

        fprintf(excelFile, "%d;%lld;%lld;%lld;%lld;%lld;%lld;%lld;%lld;\n", i, measureBufferLocalTime[i], measureBufferSync0[i], diffSync0, measureBufferSync1[i], diffSync1, diffSync, diffTimeSync0, diffTimeSync1);
        fprintf(txtFile, "%6d ; \t%19lld ; \t%19lld ; \t%8lld ; \t%19lld ; \t%8lld ; \t%8lld ; \t%8lld ; \t%8lld ;\n", i, measureBufferLocalTime[i], measureBufferSync0[i], diffSync0, measureBufferSync1[i], diffSync1, diffSync, diffTimeSync0, diffTimeSync1);
        
        state = 3;
        break;
    case 3:
      for (i = 1; i < SAMPLE_NUMBER_BUFFER_REG; i++)
        {
          int64 diffSync0 = measureBufferSync0[i] - measureBufferSync0[i - 1];
          int64 diffSync1 = measureBufferSync1[i] - measureBufferSync1[i - 1];
          int64 diffSync  = measureBufferSync1[i] - measureBufferSync0[i];

          int64 diffTimeSync0 = measureBufferSync0[i] - measureBufferLocalTime[i];
          int64 diffTimeSync1 = measureBufferSync1[i] - measureBufferLocalTime[i];

          fprintf(excelFile, "%d;%lld;%lld;%lld;%lld;%lld;%lld;%lld;%lld;\n", i, measureBufferLocalTime[i], measureBufferSync0[i], diffSync0, measureBufferSync1[i], diffSync1, diffSync, diffTimeSync0, diffTimeSync1);
          fprintf(txtFile, "%6d ; \t%19lld ; \t%19lld ; \t%8lld ; \t%19lld ; \t%8lld ; \t%8lld ; \t%8lld ; \t%8lld ;\n", i, measureBufferLocalTime[i], measureBufferSync0[i], diffSync0, measureBufferSync1[i], diffSync1, diffSync, diffTimeSync0, diffTimeSync1);
        }
        state = 100;
        break;
    case 100:
        fclose(excelFile);
        fclose(txtFile);
        state = 101;
        break;
    case 101:
        printf("\ntest yja : Write jitter finished !!\n\n");
        state = 102;
        break;
    case 99:
        printf("\ntest yja : Write jitter error (case %d) !!\n\n", stateError);
        state = 102;
        break;
    case 102: //wait
        break;
  }
}
