
#include "test_tools.h"

#include "ethercat.h"

//init call
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
      switch (ec_slave[i + 1].state)
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

void PrintEepromsPulseLength()
{
   uint16 eepromPulseLength;
   for (int i = 0; i < ec_slavecount; i++)
   {
      eepromPulseLength = (uint16)ec_readeeprom(i, 2, EC_TIMEOUTEEP);
      printf("Slave 100%d, eeprom pulse length : %u\n", i, eepromPulseLength);
   }
   printf("\n");
}

//cyclic process call
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
