
#include "measure_io.h"

#include<stdio.h>
#include<stdlib.h>

static uint16   measureBuffer1[SAMPLE_NUMBER_DOUBLE_BUFFER + 1];
static uint16   measureDiffBuffer1[SAMPLE_NUMBER_DOUBLE_BUFFER + 1];
static boolean  fullBuffer1 = 0;

static uint16   measureBuffer2[SAMPLE_NUMBER_DOUBLE_BUFFER + 1];
static uint16   measureDiffBuffer2[SAMPLE_NUMBER_DOUBLE_BUFFER + 1];
static boolean  fullBuffer2 = 0;

static boolean  measureFinished = 0;

//#define INPUT_ANALOG       (*((uint16*)((uint8*)pInput + INPUT_ANALOG_OFFSET)))
#define INPUT_ANALOG         ( (uint16)( (int32)32768 + ((int32)*((int16*)((int8*)pInput + INPUT_ANALOG_OFFSET))) ) )


void MeasureDoubleBuffer(void* pInput)
{
    static unsigned long state = 0;
    static unsigned long stateError = 99;

    static uint32 k = 0;
    static uint32 i = 0;
    static unsigned long flag = 0;

    switch (state)
    {
    case 0: //Wait 5 seconds in OP before measure
        i++;
        if (i > 5000)
        {
            i = 0;
            measureBuffer1[0] = INPUT_ANALOG;
            state = 1;
        }
        break;

    case 1: //research start period (sawtooth signal generator)
        if (INPUT_ANALOG <= measureBuffer1[0])
        {
            measureBuffer1[0] = INPUT_ANALOG;
            measureDiffBuffer1[0] = 0;
            i++;
            flag = 0;
            state = 2;
        }
        else
            measureBuffer1[0] = INPUT_ANALOG;
        break;

    case 2:
        if (fullBuffer1 > 0)
        {
            state = 99;
            stateError = 2;
        }

        measureBuffer1[i] = INPUT_ANALOG;

        if (measureBuffer1[i] > measureBuffer1[i - 1])
        {
            if (flag > 0 && flag <= MAXIMUM_RIGHT_VALUE)
                measureDiffBuffer1[i] = measureBuffer1[i] - measureBuffer1[i - 1];
            else
                measureDiffBuffer1[i] = 0;
            flag++;
        }
        else
        {
            flag = 0;
            measureDiffBuffer1[i] = 0;
        }

        if (i == SAMPLE_NUMBER_DOUBLE_BUFFER)
        {
            i = 0;
            measureBuffer2[0] = measureBuffer1[SAMPLE_NUMBER_DOUBLE_BUFFER];
            measureDiffBuffer2[0] = measureDiffBuffer1[SAMPLE_NUMBER_DOUBLE_BUFFER];
            fullBuffer1 = 1;
            k++;
            if (k == NUMBER_DOUBLE_BUFFER)
                state = 100;
            else
                state = 3;
        }
        i++;
        break;

    case 3:
        if (fullBuffer2 > 0)
        {
            state = 99;
            stateError = 3;
        }

        measureBuffer2[i] = INPUT_ANALOG;

        if (measureBuffer2[i] > measureBuffer2[i - 1])
        {
            if (flag > 0 && flag <= MAXIMUM_RIGHT_VALUE)
                measureDiffBuffer2[i] = measureBuffer2[i] - measureBuffer2[i - 1];
            else
                measureDiffBuffer2[i] = 0;
            flag++;
        }
        else
        {
            flag = 0;
            measureDiffBuffer2[i] = 0;
        }

        if (i == SAMPLE_NUMBER_DOUBLE_BUFFER)
        {
            i = 0;
            measureBuffer1[0] = measureBuffer2[SAMPLE_NUMBER_DOUBLE_BUFFER];
            measureDiffBuffer1[0] = measureDiffBuffer2[SAMPLE_NUMBER_DOUBLE_BUFFER];
            fullBuffer2 = 1;
            k++;
            if (k == NUMBER_DOUBLE_BUFFER)
                state = 100;
            else
                state = 2;
        }
        i++;
        break;

    case 99:
        printf("\ntest yja : Measure jitter error (case %d) !!\n\n", stateError);
        measureFinished = 1;
        state = 101;
        break;

    case 100:
        printf("\ntest yja : Measure jitter finished !!\n\n");
        measureFinished = 1;
        state = 101;
        break;

    case 101: //wait
        break;
    }
}

void WriteDoubleBuffer()
{
    static FILE*  txtFile = NULL;
    static FILE*  excelFile = NULL;

    static unsigned long state = 0;
    static unsigned long stateError = 99;

    static uint32 i = 0;
    static uint32 j = 0;

    switch (state)
    {
    case 0:
        excelFile = fopen(EXCEL_FILE_NAME_IO, "w");
        if (excelFile != NULL)
            state = 1;
        else {
            stateError = 0;
            state = 99;
        }
        break;

    case 1:
        txtFile = fopen(TXT_FILE_NAME_IO, "w");
        if (txtFile != NULL)
            state = 2;
        else {
            stateError = 1;
            state = 99;
        }
        break;

    case 2:
        fprintf(excelFile, "Sample;Values int;Values flo;Diff int;Diff flo;\n");
        fprintf(txtFile, "Sample   ;Values int   ;Values flo        ;Diff int    ;Diff flo         ;\n");
        state = 3;
        break;

    case 3:
        if (fullBuffer1 == 0)
        {
            if (fullBuffer2 == 0 && measureFinished > 0)
            {
                state = 100;
                break;
            }
            else
                break;
        }

        for (i = 0; i < SAMPLE_NUMBER_DOUBLE_BUFFER; i++)
        {
            fprintf(excelFile, "%d;%d;%.6f;%d;%.6f;\n", i + j, measureBuffer1[i], measureBuffer1[i] / CONVERSION_COEF, measureDiffBuffer1[i], measureDiffBuffer1[i] / CONVERSION_COEF);
            fprintf(txtFile, "%8d ; \t%+6d ; \t%+.6f ; \t%6d ; \t%.6f ;\n", i + j, measureBuffer1[i], measureBuffer1[i] / CONVERSION_COEF, measureDiffBuffer1[i], measureDiffBuffer1[i] / CONVERSION_COEF);
            //fprintf(txtFile, "%8d ; \t%6d ; \t%.6f ;\n", i + j, measureDiffBuffer1[i], measureDiffBuffer1[i] / CONVERSION_COEF);
        }
        j += SAMPLE_NUMBER_DOUBLE_BUFFER;
        fullBuffer1 = 0;
        state = 4;
        break;

    case 4:
        if (fullBuffer2 == 0)
        {
            if (fullBuffer1 == 0 && measureFinished > 0)
            {
                state = 100;
                break;
            }
            else
                break;
        }

        for (i = 0; i < SAMPLE_NUMBER_DOUBLE_BUFFER; i++)
        {
            fprintf(excelFile, "%d;%d;%.6f;%d;%.6f;\n", i + j, measureBuffer2[i], measureBuffer2[i] / CONVERSION_COEF, measureDiffBuffer2[i], measureDiffBuffer2[i] / CONVERSION_COEF);
            fprintf(txtFile, "%8d ; \t%+6d ; \t%+.6f ; \t%6d ; \t%.6f ;\n", i + j, measureBuffer2[i], measureBuffer2[i] / CONVERSION_COEF, measureDiffBuffer2[i], measureDiffBuffer2[i] / CONVERSION_COEF);
            //fprintf(txtFile, "%8d ; \t%6d ; \t%.6f ;\n", i + j, measureDiffBuffer2[i], measureDiffBuffer2[i] / CONVERSION_COEF);
        }
        j += SAMPLE_NUMBER_DOUBLE_BUFFER;
        fullBuffer2 = 0;
        state = 3;
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

void MeasureAndWriteFile(void* pInput)
{
    static short  measure[SAMPLE_NUMBER];
    static short  measureDiff[SAMPLE_NUMBER];

    static FILE*  txtFile = NULL;
    static FILE*  excelFile = NULL;

    static unsigned long state = 0;
    static unsigned long caseError = 99;

    static uint32 i = 0;
    static uint32 flag = 0;

 
    switch (state)
    {
    case 0: //Wait 5 seconds in OP before measure
        i++;
        if (i > 5000)
        {
            i = 0;
            measure[0] = INPUT_ANALOG;
            state = 1;
        }
        break;

    case 1://research start period (sawtooth signal generator)
        if (INPUT_ANALOG < measure[0])
        {
            measure[0] = INPUT_ANALOG;
            measureDiff[0] = 0;
            i = 1;
            flag = 0;
            state = 2;
        }
        else
            measure[0] = INPUT_ANALOG;
        break;

    case 2:
        measure[i] = INPUT_ANALOG;

        if (measure[i] > measure[i - 1])
        {
            if (flag > 0 && flag <= MAXIMUM_RIGHT_VALUE)
                measureDiff[i] = measure[i] - measure[i - 1];
            else
                measureDiff[i] = 0;
            flag++;
        }
        else
        flag = 0;

        i++;
        if (i == SAMPLE_NUMBER)
        {
            i = 0;
            state = 10;
            printf("\ntest yja : Measure jitter sampled\n\n");
        }
        break;

    case 10:
        excelFile = fopen(EXCEL_FILE_NAME_IO, "w");
        if (excelFile != NULL)
            state = 11;
        else {
            caseError = 10;
            state = 99;
        }
        break;

    case 11:
        txtFile = fopen(TXT_FILE_NAME_IO, "w");
        if (txtFile != NULL)
            state = 12;
        else {
            caseError = 11;
            state = 99;
        }
        break;

    case 12:
        fprintf(excelFile, "Sample;Values int;Values flo;Diff int;Diff flo;\n");
        fprintf(txtFile, "Sample   ;Values int   ;Values flo        ;Diff int    ;Diff flo         ;\n");
        state = 13;
        break;

    case 13:
        for (int j = 0; j < 100; j++)
        {
            fprintf(excelFile, "%d;%d;%.6f;%d;%.6f;\n", i + j, measure[i + j], measure[i + j] / CONVERSION_COEF, measureDiff[i + j], measureDiff[i + j] / CONVERSION_COEF);
            fprintf(txtFile, "%8d ; \t%+6d ; \t%+.6f ; \t%6d ; \t%.6f ;\n", i + j, measure[i + j], measure[i + j] / CONVERSION_COEF, measureDiff[i + j], measureDiff[i + j] / CONVERSION_COEF);
        }
        i += 100;
        if (i >= SAMPLE_NUMBER)
        {
            i = 0;
            state = 14;
        }
        break;

    case 14:
        fclose(excelFile);
        fclose(txtFile);
        state = 15;
        break;

    case 15:
        printf("\ntest yja : Measure jitter finished !!\n\n");
        state = 100;
        break;

    case 99:
        printf("\ntest yja : Measure jitter error (case %d) !!\n\n", caseError);
        state = 100;
        break;

    case 100: //wait
        break;
    }
}