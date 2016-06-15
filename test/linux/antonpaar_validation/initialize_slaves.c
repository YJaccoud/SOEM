
#include "initialize_slaves.h"

//#include "limits.h"

#include "oshw.h"

#include "ethercattype.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"

//EL3314
int EL3314_InitCommandCoeConfigurePdoMapping(uint16 slave)
{
   return 1;
}

#define EL3314_INDEX_THERMO_SETTINGS_CHANNEL_1   0x8000
#define EL3314_INDEX_THERMO_SETTINGS_CHANNEL_2   0x8010
#define EL3314_INDEX_THERMO_SETTINGS_CHANNEL_3   0x8020
#define EL3314_INDEX_THERMO_SETTINGS_CHANNEL_4   0x8030

#define EL3314_INDEX_THERMO_SETTINGS_SUBINDEX_THERMO_TYPE            0x19
#define EL3314_INDEX_THERMO_SETTINGS_SUBINDEX_NB_SIGNIFICANT_DIGIT   0x02

#define EL3314_SUBINDEX_THERMO_TYPE_PARAM_THERMO_TYPE_K                    0
#define EL3314_SUBINDEX_THERMO_TYPE_PARAM_THERMO_TYPE_N                    5

#define EL3314_SUBINDEX_NB_SIGNIFICANT_DIGIT_PARAM_3_SIGNIFICANT_DIGIT     3
#define EL3314_SUBINDEX_NB_SIGNIFICANT_DIGIT_PARAM_1_SIGNIFICANT_DIGIT     0

int EL3314_InitCommandCoeConfigureProcess(uint16 slave)
{
   /*
   uint16 ThermoType = EL3314_SUBINDEX_THERMO_TYPE_PARAM_THERMO_TYPE_N;

   ec_SDOwrite(slave, EL3314_INDEX_THERMO_SETTINGS_CHANNEL_1, EL3314_INDEX_THERMO_SETTINGS_SUBINDEX_THERMO_TYPE, 0, sizeof(ThermoType), &ThermoType, INFINITE);
   ec_SDOwrite(slave, EL3314_INDEX_THERMO_SETTINGS_CHANNEL_2, EL3314_INDEX_THERMO_SETTINGS_SUBINDEX_THERMO_TYPE, 0, sizeof(ThermoType), &ThermoType, INFINITE);
   ec_SDOwrite(slave, EL3314_INDEX_THERMO_SETTINGS_CHANNEL_3, EL3314_INDEX_THERMO_SETTINGS_SUBINDEX_THERMO_TYPE, 0, sizeof(ThermoType), &ThermoType, INFINITE);
   ec_SDOwrite(slave, EL3314_INDEX_THERMO_SETTINGS_CHANNEL_4, EL3314_INDEX_THERMO_SETTINGS_SUBINDEX_THERMO_TYPE, 0, sizeof(ThermoType), &ThermoType, INFINITE);
   */

   uint8 Data = EL3314_SUBINDEX_NB_SIGNIFICANT_DIGIT_PARAM_3_SIGNIFICANT_DIGIT;

   ec_SDOwrite(slave, EL3314_INDEX_THERMO_SETTINGS_CHANNEL_1, EL3314_INDEX_THERMO_SETTINGS_SUBINDEX_NB_SIGNIFICANT_DIGIT, 0, sizeof(Data), &Data, INFINITE);
   ec_SDOwrite(slave, EL3314_INDEX_THERMO_SETTINGS_CHANNEL_2, EL3314_INDEX_THERMO_SETTINGS_SUBINDEX_NB_SIGNIFICANT_DIGIT, 0, sizeof(Data), &Data, INFINITE);
   ec_SDOwrite(slave, EL3314_INDEX_THERMO_SETTINGS_CHANNEL_3, EL3314_INDEX_THERMO_SETTINGS_SUBINDEX_NB_SIGNIFICANT_DIGIT, 0, sizeof(Data), &Data, INFINITE);
   ec_SDOwrite(slave, EL3314_INDEX_THERMO_SETTINGS_CHANNEL_4, EL3314_INDEX_THERMO_SETTINGS_SUBINDEX_NB_SIGNIFICANT_DIGIT, 0, sizeof(Data), &Data, INFINITE);

   return 1;
}

int EL3314_InitCommand(uint16 slave)
{
   EL3314_InitCommandCoeConfigurePdoMapping(slave);
   EL3314_InitCommandCoeConfigureProcess(slave);

   return 1;
}

boolean EL3314_InitDc(uint16 slave)
{
   return FALSE;
}

//EL3104
#define EL3104_PDOASSIGNMENT_OUTPUT                      0x1C12
#define EL3104_PDOASSIGNMENT_INPUT                       0x1C13

#define EL3104_PDOMAPPING_CHANNEL1_STANDARD              0x1A00
#define EL3104_PDOMAPPING_CHANNEL1_COMPACT               0x1A01
#define EL3104_PDOMAPPING_CHANNEL2_STANDARD              0x1A02
#define EL3104_PDOMAPPING_CHANNEL2_COMPACT               0x1A03
#define EL3104_PDOMAPPING_CHANNEL3_STANDARD              0x1A04
#define EL3104_PDOMAPPING_CHANNEL3_COMPACT               0x1A05
#define EL3104_PDOMAPPING_CHANNEL4_STANDARD              0x1A06
#define EL3104_PDOMAPPING_CHANNEL4_COMPACT               0x1A07

int EL3104_InitCommandCoeConfigurePdoMapping(uint16 slave)
{
   uint8  NumberOfPdoAssignment;
   uint16 PdoMapping;

   NumberOfPdoAssignment = 0;
   ec_SDOwrite(slave, EL3104_PDOASSIGNMENT_INPUT, 0, 0, sizeof(NumberOfPdoAssignment), &NumberOfPdoAssignment, INFINITE);

   PdoMapping = EL3104_PDOMAPPING_CHANNEL1_COMPACT;
   ec_SDOwrite(slave, EL3104_PDOASSIGNMENT_INPUT, 1, 0, sizeof(PdoMapping), &PdoMapping, INFINITE);

   PdoMapping = EL3104_PDOMAPPING_CHANNEL2_COMPACT;
   ec_SDOwrite(slave, EL3104_PDOASSIGNMENT_INPUT, 2, 0, sizeof(PdoMapping), &PdoMapping, INFINITE);

   PdoMapping = EL3104_PDOMAPPING_CHANNEL3_COMPACT;
   ec_SDOwrite(slave, EL3104_PDOASSIGNMENT_INPUT, 3, 0, sizeof(PdoMapping), &PdoMapping, INFINITE);

   PdoMapping = EL3104_PDOMAPPING_CHANNEL4_COMPACT;
   ec_SDOwrite(slave, EL3104_PDOASSIGNMENT_INPUT, 4, 0, sizeof(PdoMapping), &PdoMapping, INFINITE);

   NumberOfPdoAssignment = 4;
   ec_SDOwrite(slave, EL3104_PDOASSIGNMENT_INPUT, 0, 0, sizeof(NumberOfPdoAssignment), &NumberOfPdoAssignment, INFINITE);

   return 1;
}

int EL3104_InitCommandCoeConfigureProcess(uint16 slave)
{
   return 1;
}

int EL3104_InitCommand(uint16 slave)
{
   EL3104_InitCommandCoeConfigurePdoMapping(slave);
   EL3104_InitCommandCoeConfigureProcess(slave);

   return 1;
}

boolean EL3104_InitDc(uint16 slave)
{
   //inputs analog
   //register shift   : 0
   //register cycle 0 : bus cycle
   //register cycle 1 : 25 us
   //ec_dcsync0(10, TRUE, 500000, 0);
   //ec_dcsync01(10, TRUE, 500000, 500000, 25);

   //ec_dcsync01(11, TRUE, 1000000, 200000, 400000);
   return FALSE;
}

//EL4104
int EL4104_InitCommandCoeConfigurePdoMapping(uint16 slave)
{
   return 1;
}

int EL4104_InitCommandCoeConfigureProcess(uint16 slave)
{
   return 1;
}

int EL4104_InitCommand(uint16 slave)
{
   EL4104_InitCommandCoeConfigurePdoMapping(slave);
   EL4104_InitCommandCoeConfigureProcess(slave);

   return 1;
}

boolean EL4104_InitDc(uint16 slave)
{
   //outupus analog
   //register shift   : 0
   //register cycle 0 : bus cycle
   //register cycle 1 : 100 us
   //ec_dcsync01(10, TRUE, 1000000, 100000, 300000);

   ec_dcsync01(10, TRUE, 1000000, 200000, 400000);
   return TRUE;
}

//Initialize slaves
#define MANUFACTURER_BECKHOFF 0x02
#define PRODUCTCODE_EL3104    0x0C203052
#define PRODUCTCODE_EL3314    0x0CF23052
#define PRODUCTCODE_EL4104    0x10083052

void InitializeSlavesPoToSo()
{
   for (int i = 1; i <= ec_slavecount; i++)
   {
      if (ec_slave[i].eep_man == MANUFACTURER_BECKHOFF && ec_slave[i].eep_id == PRODUCTCODE_EL3104)
         ec_slave[i].PO2SOconfig = EL3104_InitCommand;
      else if (ec_slave[i].eep_man == MANUFACTURER_BECKHOFF && ec_slave[i].eep_id == PRODUCTCODE_EL3314)
         ec_slave[i].PO2SOconfig = EL3314_InitCommand;
      else if (ec_slave[i].eep_man == MANUFACTURER_BECKHOFF && ec_slave[i].eep_id == PRODUCTCODE_EL4104)
         ec_slave[i].PO2SOconfig = EL4104_InitCommand;
   }
}

#define MAX_DC_SLAVES_NUMBER                    (10)
#define SLAVE_NUMBER_TO_SLAVE_ADDRESS_PHYSIC    (0x1000)

#define DC_DIFFERENCE_REGISTER_ADDRESS          (0x92C)
#define DC_DIFFERENCE_REGISTER_LENGTH           (4)

void InitializeSlavesSoToOp(OSAL_EVENT_HANDLE*  timerEvent)
{
   int re = 0, i = 0, j = 0;
   int slaveDcId[MAX_DC_SLAVES_NUMBER] = {0};
   uint32 dcDifference = 0xFFFFFFFF;
   boolean notSynchro = FALSE;
   ec_configdc();

   for (i = 1 ; i <= ec_slavecount ; i++)
   {
      if (ec_slave[i].eep_man == MANUFACTURER_BECKHOFF && ec_slave[i].eep_id == PRODUCTCODE_EL3104)
         re = EL3104_InitDc(i);
      else if (ec_slave[i].eep_man == MANUFACTURER_BECKHOFF && ec_slave[i].eep_id == PRODUCTCODE_EL3314)
         re = EL3314_InitDc(i);
      else if (ec_slave[i].eep_man == MANUFACTURER_BECKHOFF && ec_slave[i].eep_id == PRODUCTCODE_EL4104)
         re = EL4104_InitDc(i);
      else
         re = 0;

      if (re)
      {
         slaveDcId[j] = i;
         j++;
      }
   }

   do
   {
      re = osal_event_wait(timerEvent, OSAL_WAIT_INFINITE);

      ec_send_processdata();
      ec_receive_processdata(EC_TIMEOUTRET);

      for (i = 0; i < j; i++)
      {
         ec_FPRD(10 + SLAVE_NUMBER_TO_SLAVE_ADDRESS_PHYSIC, DC_DIFFERENCE_REGISTER_ADDRESS, DC_DIFFERENCE_REGISTER_LENGTH, &dcDifference, EC_TIMEOUTRET3);
         if (dcDifference > 4)
            break;
      }
   }
   while (dcDifference > 4);
}
