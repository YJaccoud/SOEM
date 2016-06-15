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

#include "ethercat.h"

#include "initialize_slaves.h"
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

void PrintSlavesState()
{
   for (int i = 0; i < ec_slavecount; i++)
   {
      switch (ec_slave[i+1].state)
      {
      case EC_STATE_INIT:
         printf("Slave 100%d state : INIT\n", i + 1);
         break;
      case EC_STATE_PRE_OP:
         printf("Slave 100%d state : PRE OP\n", i + 1);
         break;
      case EC_STATE_SAFE_OP:
         printf("Slave 100%d state : SAFE OP\n", i + 1);
         break;
      case EC_STATE_OPERATIONAL:
         printf("Slave 100%d state : OP\n", i + 1);
         break;
      case EC_STATE_ERROR:
         printf("Slave 100%d state : ERROR\n", i + 1);
         break;
      default:
         printf("Slave 100%d state : unknown (%d)\n", i + i, ec_slave[1].state);
         break;
      }
   }
}

void PrintSlavesIds()
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

PrintEepromsPulseLength()
{
   uint16 eepromPulseLength;
   for (int i = 0; i < ec_slavecount; i++)
   {
      eepromPulseLength = (uint16)ec_readeeprom(i, 2, EC_TIMEOUTEEP);
      printf("Slave 100%d, eeprom pulse length : %u\n", i, eepromPulseLength);
   }
   printf("\n");
}







void PrintCyclicInt32(int32 data, uint32 cycleUpdate)
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
   //PrintCyclicInt32(*((int32*)(ec_slave[3].inputs + 2)), 3000);

   //MeasureBufferRegisterDc(10);
   //MeasureInputValueDoubleBuffer(IOmap + ec_slave[0].Obytes);

   //CommandSlavesAntonPaar();
   MeasureJitterOscillo();
}

//#define DC_DIFFERENCE_REGISTER_ADDRESS        (0x92C)
//#define DC_DIFFERENCE_REGISTER_LENGTH         (4)

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
   PrintSlavesIds();
   //PrintEepromsPulseLength();

   InitializeSlavesPoToSo();

   ec_config_map(&IOmap);

   //Config DC
   InitializeSlavesSoToOp(timerEvent);

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
      //WriteBufferRegisterDc();
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