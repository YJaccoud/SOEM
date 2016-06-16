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
#include "test_tools.h"

#include "limits.h"

#define   PULSE_LENGTH_EEPROM_ADDRESS             (2)
#define   SLAVE_NUMBER_TO_SLAVE_ADDRESS_PHYSIC    0x1000

uint8 IOmap[4096];

void CyclicProcess()
{
   //PrintRegister();
   //PrintCyclicInt32(*((int32*)(ec_slave[3].inputs + 2)), 3000);

   //MeasureBufferRegisterDc(10);
   //MeasureInputValueDoubleBuffer(IOmap + ec_slave[0].Obytes);

   //CommandSlavesAntonPaar();
   MeasureJitterOscillo();
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
   //PrintSlavesIds();
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