/*****************************************************************************
*
* FILE NAME:		ec_master.c
*
* DESCRIPTION:		This is the main program module.
*
\*****************************************************************************/

#include <stdio.h>


/*****************************************************************************
*
* FUNCTION:			main
*
* DESCRIPTION:
*  This is the main program module.
*
\*****************************************************************************/

/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : slaveinfo [ifname] [-sdo] [-map]
 * Ifname is NIC interface, f.e. eth0.
 * Optional -sdo to display CoE object dictionary.
 * Optional -map to display slave PDO mapping
 *
 * This shows the configured slave data.
 *
 * (c)Arthur Ketels 2010 - 2011
 */
#include <stdio.h>
#include <string.h>
#include <rt.h>
#include <traceapi.h>

#include "ethercat.h"

#include "measure_input_value.h"
#include "measure_register_dc.h"
#include "slaves_antonpaar.h"

#include "limits.h"

#define   PULSE_LENGTH_EEPROM_ADDRESS             (2)
#define   SLAVE_NUMBER_TO_SLAVE_ADDRESS_PHYSIC    0x1000

uint8 IOmap[4096];

void PrintMasterState()
{
   switch (ec_slave[0].state)
   {
   case EC_STATE_INIT:
      printf("Master state : INIT\n");
      break;
   case EC_STATE_PRE_OP:
      printf("Master state : PRE OP\n");
      break;
   case EC_STATE_SAFE_OP:
      printf("Master state : SAFE OP\n");
      break;
   case EC_STATE_OPERATIONAL:
      printf("Master state : OP\n");
      break;
   case EC_STATE_ERROR:
      printf("Master state : ERROR\n");
      break;
   default:
      printf("Master state : unknown (%d)\n", ec_slave[0].state);
      break;
   }
}

void PrintSlaveState()
{
   switch (ec_slave[1].state)
   {
   case EC_STATE_INIT:
      printf("EK1100 state : INIT\n");
      break;
   case EC_STATE_PRE_OP:
      printf("EK1100 state : PRE OP\n");
      break;
   case EC_STATE_SAFE_OP:
      printf("EK1100 state : SAFE OP\n");
      break;
   case EC_STATE_OPERATIONAL:
      printf("EK1100 state : OP\n");
      break;
   case EC_STATE_ERROR:
      printf("EK1100 state : ERROR\n");
      break;
   default:
      printf("EK1100 state : unknown (%d)\n", ec_slave[1].state);
      break;
   }
}

void PrintSlaveInfo()
{
   for (int i = 0; i < ec_slavecount; i++)
   {
      printf("slave @ position %d ->\tName :    %s \n", i + 1, ec_slave[i + 1].name);
      printf("                      \tMan. id : 0x%x \n", ec_slave[i + 1].eep_man);
      printf("                      \tProd id : 0x%x \n", ec_slave[i + 1].eep_id);
      printf("                      \tAlias :   %d \n", ec_slave[i + 1].aliasadr);
      printf("\n");
   }
}

PrintEepromInfo()
{
   uint16 eepromPulseLength;
   for (int i = 0; i < ec_slavecount; i++)
   {
      eepromPulseLength = (uint16)ec_readeeprom(i, 2, EC_TIMEOUTEEP);
      printf("Slave 100%d, eeprom pulse length : %u\n", i, eepromPulseLength);
   }
   printf("\n");
}


#define INDEX_THERMO_SETTINGS_CHANNEL_1   0x8000
#define INDEX_THERMO_SETTINGS_CHANNEL_2   0x8010
#define INDEX_THERMO_SETTINGS_CHANNEL_3   0x8020
#define INDEX_THERMO_SETTINGS_CHANNEL_4   0x8030

#define SUBINDEX_THERMO_TYPE              0x19
#define SUBINDEX_NB_SIGNIFICANT_DIGIT     0x02

#define PARAM_THERMO_TYPE_K               0
#define PARAM_THERMO_TYPE_N               5

#define PARAM_3_SIGNIFICANT_DIGIT         3
#define PARAM_1_SIGNIFICANT_DIGIT         0

int PO2SO_EL3314(uint16 slave)
{
   /*
   uint16 ThermoType = PARAM_THERMO_TYPE_N;

   ec_SDOwrite(slave, INDEX_THERMO_SETTINGS_CHANNEL_1, SUBINDEX_THERMO_TYPE, 0, sizeof(ThermoType), &ThermoType, INFINITE);
   ec_SDOwrite(slave, INDEX_THERMO_SETTINGS_CHANNEL_2, SUBINDEX_THERMO_TYPE, 0, sizeof(ThermoType), &ThermoType, INFINITE);
   ec_SDOwrite(slave, INDEX_THERMO_SETTINGS_CHANNEL_3, SUBINDEX_THERMO_TYPE, 0, sizeof(ThermoType), &ThermoType, INFINITE);
   ec_SDOwrite(slave, INDEX_THERMO_SETTINGS_CHANNEL_4, SUBINDEX_THERMO_TYPE, 0, sizeof(ThermoType), &ThermoType, INFINITE);
   */

   uint8 Data = PARAM_3_SIGNIFICANT_DIGIT;

   ec_SDOwrite(slave, INDEX_THERMO_SETTINGS_CHANNEL_1, SUBINDEX_NB_SIGNIFICANT_DIGIT, 0, sizeof(Data), &Data, INFINITE);
   ec_SDOwrite(slave, INDEX_THERMO_SETTINGS_CHANNEL_2, SUBINDEX_NB_SIGNIFICANT_DIGIT, 0, sizeof(Data), &Data, INFINITE);
   ec_SDOwrite(slave, INDEX_THERMO_SETTINGS_CHANNEL_3, SUBINDEX_NB_SIGNIFICANT_DIGIT, 0, sizeof(Data), &Data, INFINITE);
   ec_SDOwrite(slave, INDEX_THERMO_SETTINGS_CHANNEL_4, SUBINDEX_NB_SIGNIFICANT_DIGIT, 0, sizeof(Data), &Data, INFINITE);

   return 1;
}

int PO2SO_EL3104(uint16 slave)
{
   /*
   uint8  NumberOfPdoAssignment;
   uint16 PdoAssignment;

   NumberOfPdoAssignment = 0;
   ec_SDOwrite(slave, 0x1C13, 0, 0, sizeof(NumberOfPdoAssignment), &NumberOfPdoAssignment, INFINITE);

   PdoAssignment = 0x1A01;
   ec_SDOwrite(slave, 0x1C13, 1, 0, sizeof(PdoAssignment), &PdoAssignment, INFINITE);

   PdoAssignment = 0x1A03;
   ec_SDOwrite(slave, 0x1C13, 2, 0, sizeof(PdoAssignment), &PdoAssignment, INFINITE);

   PdoAssignment = 0x1A05;
   ec_SDOwrite(slave, 0x1C13, 3, 0, sizeof(PdoAssignment), &PdoAssignment, INFINITE);

   PdoAssignment = 0x1A07;
   ec_SDOwrite(slave, 0x1C13, 4, 0, sizeof(PdoAssignment), &PdoAssignment, INFINITE);

   NumberOfPdoAssignment = 4;
   ec_SDOwrite(slave, 0x1C13, 0, 0, sizeof(NumberOfPdoAssignment), &NumberOfPdoAssignment, INFINITE);
   */

   return 1;
}

#define MANUFACTURER_BECKHOFF 0x02
#define PRODUCTCODE_EL3104    0xC203052
#define PRODUCTCODE_EL3314    0xCF23052
void ConfigPdoMapping()
{
   for (int i = 1; i <= ec_slavecount; i++)
   {
      if (ec_slave[i].eep_man == MANUFACTURER_BECKHOFF && ec_slave[i].eep_id == PRODUCTCODE_EL3104)
         ec_slave[i].PO2SOconfig = PO2SO_EL3104;
      else if (ec_slave[i].eep_man == MANUFACTURER_BECKHOFF && ec_slave[i].eep_id == PRODUCTCODE_EL3314)
         ec_slave[i].PO2SOconfig = PO2SO_EL3314;
   }
}

void PrintInputInt32(int32 data, uint32 cycleUpdate)
{
   static uint32 count;

   count++;
   if (!(count < cycleUpdate))
   {
      count = 0;
      printf("print input : %d\n", data);
   }
}

void MeasureJitterOscillo()
{
   uint16 * pOutput1 = (uint16*)ec_slave[10].outputs;
   uint16 * pOutput2 = (uint16*)ec_slave[11].outputs;

   if (*pOutput1 == 0)
   {
      *pOutput1 = 32767;
      *pOutput2 = 32767;
   }
   else
   {
      *pOutput1 = 0;
      *pOutput2 = 0;
   }
}

void CyclicProcess()
{
   //PrintRegister();
   //PrintInputInt32(*((int32*)(ec_slave[3].inputs + 2)), 3000);

   MeasureBufferRegisterDc(10);
   //MeasureInputValueDoubleBuffer(IOmap + ec_slave[0].Obytes);

   //CommandSlavesAntonPaar();
   MeasureJitterOscillo();
}

#define DC_DIFFERENCE_REGISTER_ADDRESS        (0x92C)
#define DC_DIFFERENCE_REGISTER_LENGTH         (4)

ConfigDc(OSAL_EVENT_HANDLE * timerEvent)
{
   /*
   uint16 pulseLengthSlave0;
   uint16 pulseLengthSlave1;

   pulseLengthSlave0 = (uint16)ec_readeeprom(0, PULSE_LENGTH_EEPROM_ADDRESS, EC_TIMEOUTEEP);
   pulseLengthSlave1 = (uint16)ec_readeeprom(1, PULSE_LENGTH_EEPROM_ADDRESS, EC_TIMEOUTEEP);
   pulseLengthSlave0 = pulseLengthSlave0 * 10;
   pulseLengthSlave1 = pulseLengthSlave1 * 10;
   */

   int wkc, wkc1, wkc2;
   RTHANDLE alarmHandle;
   alarmHandle = CreateRtAlarm(KN_REPEATER, 500);
   uint32 dcDifferenceSlave2 = 0xFFFFFFFF, dcDifferenceSlave3 = 0xFFFFFFFF;

   ec_configdc();

   //inputs analog
   //register shift   : 0
   //register cycle 0 : bus cycle
   //register cycle 1 : 25 us
   //ec_dcsync0(10, TRUE, 500000, 0);
   //ec_dcsync01(10, TRUE, 500000, 500000, 25);

   //outupus analog
   //register shift   : 0
   //register cycle 0 : bus cycle
   //register cycle 1 : 100 us
   //ec_dcsync01(10, TRUE, 1000000, 100000, 300000);

   ec_dcsync01(10, TRUE, 1000000, 200000, 400000);
   ec_dcsync01(11, TRUE, 1000000, 200000, 400000);

   //"fonctionne"
   //ec_dcsync01(10, TRUE, 10000000, 2000000, 4000000);

   while (TRUE)
   {
      osal_event_wait(timerEvent, OSAL_WAIT_INFINITE);

      ec_send_processdata();
      wkc = ec_receive_processdata(EC_TIMEOUTRET);

      wkc1 = ec_FPRD(10 + SLAVE_NUMBER_TO_SLAVE_ADDRESS_PHYSIC, DC_DIFFERENCE_REGISTER_ADDRESS, DC_DIFFERENCE_REGISTER_LENGTH, &dcDifferenceSlave2, EC_TIMEOUTRET3);
      wkc2 = ec_FPRD(11 + SLAVE_NUMBER_TO_SLAVE_ADDRESS_PHYSIC, DC_DIFFERENCE_REGISTER_ADDRESS, DC_DIFFERENCE_REGISTER_LENGTH, &dcDifferenceSlave3, EC_TIMEOUTRET3);
      //wait master digital PLL locked to reference clock (1st slave ?) where is the digitel pll ?
      if (dcDifferenceSlave2 < 5 && dcDifferenceSlave3 < 5 && wkc1 == 1 && wkc2 == 1)
         break;
   }
}

OSAL_THREAD_FUNC serviceThreadRoutine(LPVOID lpParameter)
{
   uint8 coeod[100];

   while (TRUE)
   {
      if (ec_slave[0].state == EC_STATE_OPERATIONAL)
      {
         int size = sizeof(coeod);
         ec_SDOread(2, 0x1A00, 1, FALSE, &size, coeod, EC_TIMEOUTRXM);
      }
      else
         osal_usleep(5000);
   }
}

void PrintRegister()
{
   uint8   register8;
   uint16  register16;
   uint32  register32;
   uint64  register64;

   static uint32 cpt = 0;

   cpt++;
   if (cpt > 2000)
   {
      cpt = 0;

      for (int i = 1; i <= ec_slavecount; i++)
      {
         register8 = 0;
         register16 = 0;
         register32 = 0;
         register64 = 0;

         //add read register 934 et 935

         //ec_FPRD(0x1000 + i, 0x981, 1, &register8, EC_TIMEOUTRET3);
         //printf("Slave 100%d, Register 0x981 : %u (%X)\n", i, register8, register8);

         //ec_FPRD(0x1000 + i, 0x982, 2, &register16, EC_TIMEOUTRET3);
         //printf("Slave 100%d, Register 0x982 : %u (%X)\n", i, register16, register16);

         //ec_FPRD(0x1000 + i, 0x9A4, 4, &register32, EC_TIMEOUTRET3);
         //printf("Slave 100%d, Register 0x9A4 : %u (%X)\n", i, register32, register32);

         //ec_FPRD(0x1000 + i, 0x910, 8, &register64, EC_TIMEOUTRET3);
         //printf("Slave 100%d, Register 0x910 : %llu (%llX)\n", i, register64, register64);
      }

      printf("\n");
   }
}

OSAL_THREAD_FUNC_RT masterThreadRoutine(LPVOID lpParameter)
{
   OSAL_EVENT_HANDLE * timerEvent = (OSAL_EVENT_HANDLE*)lpParameter;

   int wkc = 0, cpt = 0;
   boolean slavesOP = FALSE;

   printf("Thread master : Hello !!\n");

   //Config Master & bus
   ec_init("ie1g0");
   ec_config_init(FALSE);

   printf("\n");
   PrintSlaveInfo();
   //PrintEepromInfo();

   ConfigPdoMapping();

   ec_config_map(&IOmap);

   //Config DC
   ConfigDc(timerEvent);

   //Go OP
   cpt = 0;
   for (int i = 0; i < 10; i++)
   {
      osal_event_wait(timerEvent, OSAL_WAIT_INFINITE);
      ec_send_processdata();
      wkc = ec_receive_processdata(EC_TIMEOUTRET);
   }
   ec_slave[0].state = EC_STATE_OPERATIONAL;
   ec_writestate(0);

   while (slavesOP == FALSE)
   {
      osal_event_wait(timerEvent, OSAL_WAIT_INFINITE);
      ec_send_processdata();
      wkc = ec_receive_processdata(EC_TIMEOUTRET);
      cpt++;
      if (cpt == 4000)
      {
         cpt = 0;

         ec_readstate();
         for (int i = 0; i < ec_slavecount; i++)
         {
            slavesOP = TRUE;
            if (ec_slave[i + 1].state != EC_STATE_OPERATIONAL)
            {
               printf("slave %d (%s) is not OP\n", i + 1, ec_slave[i + 1].name);
               slavesOP = FALSE;
            }
         }
         if (slavesOP == FALSE)
         {
            ec_slave[0].state = EC_STATE_OPERATIONAL;
            ec_writestate(0);
         }
      }
   }

   while (1)
   {
      osal_event_wait(timerEvent, OSAL_WAIT_INFINITE);
      ec_send_processdata();
      wkc = ec_receive_processdata(EC_TIMEOUTRET);

      CyclicProcess();

      if (wkc != (ec_group[0].inputsWKC + 2 * ec_group[0].outputsWKC))
         printf("WKC error. expected : %d and read : %d\n", ec_group[0].inputsWKC + 2 * ec_group[0].outputsWKC, wkc);
   }
}

OSAL_THREAD_FUNC_RT timerThreadRoutine(LPVOID lpParameter)
{
   printf("Thread timer : Hello !!\n");
   
   /*
   while (1)
   {
   osal_usleep(1000);
   osal_event_set((OSAL_EVENT_HANDLE*)lpParameter);
   }
   */

   RTHANDLE alarmHandle;
   alarmHandle = CreateRtAlarm(KN_REPEATER, 1000);

   while (1)
   {
      WaitForRtAlarm(alarmHandle, 1000 * 1000);
      osal_event_set((OSAL_EVENT_HANDLE*)lpParameter);
   }

}

void antonpaar_validation(/*char *ifname*/)
{
   osal_init();

   OSAL_EVENT_HANDLE   timerEvent = NULL;
   OSAL_THREAD_HANDLE  timerThread = NULL;
   OSAL_THREAD_HANDLE  masterThread = NULL;
   OSAL_THREAD_HANDLE  serviceThread = NULL;

   osal_event_create(&timerEvent);
   osal_thread_create_rt(&masterThread, 1024 * 1024, masterThreadRoutine, (LPVOID)&timerEvent);
   osal_thread_create_rt(&timerThread, 4096, timerThreadRoutine, (LPVOID)&timerEvent);
   //osal_thread_create(&serviceThread, 1024 * 1024, serviceThreadRoutine, (LPVOID)&timerEvent);

   while (!osal_thread_is_terminated(&masterThread, OSAL_NO_WAIT) ||
      !osal_thread_is_terminated(&timerThread, OSAL_NO_WAIT))//||
      //!osal_thread_is_terminated(&serviceThread, OSAL_NO_WAIT)  )
   {
      //WriteInputValueDoubleBuffer();
      WriteBufferRegisterDc();
      osal_usleep(10000);
   }

   //osal_thread_delete(&serviceThread);
   osal_thread_delete(&timerThread);
   osal_thread_delete(&masterThread);
   osal_event_delete(&timerEvent);

   // ec_group[0].blockLRW = FALSE;                       //FALSE = Use LRW //TRUE = Use LRD / LRW

   /*
   ec_readstate();
   for (int cpt = 1; cpt <= ec_slavecount; cpt++){
   if (ec_slave[cpt].state != EC_STATE_OPERATIONAL){
   printf("pass slave %s to OP\n", ec_slave[cpt].name);
   ec_slave[cpt].state = EC_STATE_OPERATIONAL;
   ec_writestate(cpt);
   }
   }
   */

   //Go OP
   /*
   ec_send_processdata();
   ec_receive_processdata(EC_TIMEOUTRET);
   ec_slave[0].state = EC_STATE_OPERATIONAL;
   ec_writestate(0);
   while (EC_STATE_OPERATIONAL != ec_statecheck(0, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE))
   {
   ec_readstate();
   for (int i = 0; i < ec_slavecount; i++)
   if (ec_slave[i + 1].state != EC_STATE_OPERATIONAL){
   printf("slave %d (%s) is not OP\n", i + 1, ec_slave[i + 1].name);
   //ec_slave[cpt].state = EC_STATE_OPERATIONAL;
   //ec_writestate(cpt);
   }
   ec_send_processdata();
   ec_receive_processdata(EC_TIMEOUTRET);
   ec_slave[0].state = EC_STATE_OPERATIONAL;
   ec_writestate(0);
   }
   */
}

void main(int argc, char* argv[])
{
   printf("SOEM (Simple Open EtherCAT Master)\nSlaveinfo\n");

   //if (argc > 1)
   //{
   /* start slaveinfo */
   //printf("argv[0] : %s\n", argv[0]);
   //printf("argv[1] : %s\n", argv[1]);
   antonpaar_validation(argv[1]);
   //}
   //else
   //{
   //     printf("Usage: slaveinfo ifname [options]\nifname = eth0 for example\nOptions :\n -sdo : print SDO info\n -map : print mapping\n");
   //}

   printf("End program\n");

}

