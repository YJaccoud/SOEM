
#include "slavesap.h"

#include "limits.h"

#include "osal.h"
#include "oshw.h"
#include "ethercattype.h"
#include "ethercatbase.h"
#include "ethercatmain.h"

#define RTMOTOR_ACCESS_OUTPUT (*((RtMotorOutputStruct*)ec_slave[rtmotorPosition].outputs))
#define RTMOTOR_ACCESS_INPUT  (*((RtMotorInputStruct*)ec_slave[rtmotorPosition].inputs))

#define HEAD1_ACCESS_OUTPUT   (*((HeadOutputStruct*)ec_slave[head1Position].outputs))
#define HEAD1_ACCESS_INPUT    (*((HeadInputStruct*)ec_slave[head1Position].inputs))

#define HEAD2_ACCESS_OUTPUT   (*((HeadOutputStruct*)ec_slave[head2Position].outputs))
#define HEAD2_ACCESS_INPUT    (*((HeadInputStruct*)ec_slave[head2Position].inputs))

#define STEP          10

CommandSlavesAP()
{
  static boolean  firstCycle = 1;
  static boolean  reverse = 0;
  static int16    command;
  static uint32   state = 0;
  static uint32   tempo_head = 0;
  static uint32   tempo_rtmotor = 0;
  static uint32   rtmotorPosition = 0;
  static uint32   head1Position = 0;
  static uint32   head2Position = 0;

  if (firstCycle > 0)
  {
    for (int i = 1 ; i <= ec_slavecount ; i++)
    {
      switch (ec_slave[i].aliasadr)
      {
      case 100:
        rtmotorPosition = i;
        break;
      case 10400:
        head1Position = i;
        break;
      case 20400:
        head2Position = i;
        break;
      }
    }
    firstCycle = 0;
  }
  else
  { 
    //Command Ethercat RTMotor
    if (rtmotorPosition > 0)
    {
      RTMOTOR_ACCESS_OUTPUT.ConfigRegister.watchdog = !RTMOTOR_ACCESS_OUTPUT.ConfigRegister.watchdog;

      if (reverse > 0)
      {
        if (((int32)command) - ((int32)STEP) <= SHRT_MIN)
          reverse = 0;
      }
      else
      {
        if (((int32)command) + ((int32)STEP) >= SHRT_MAX)
          reverse = 1;
      }

      if (reverse > 0)
        command = command - STEP;
      else
        command = command + STEP;
    }

    RTMOTOR_ACCESS_OUTPUT.AxisX.CommandRegister.motor_on = TRUE;
    RTMOTOR_ACCESS_OUTPUT.AxisY.CommandRegister.motor_on = TRUE;
    RTMOTOR_ACCESS_OUTPUT.AxisZ.CommandRegister.motor_on = TRUE;

    RTMOTOR_ACCESS_OUTPUT.AxisX.CommandValue = command;
    RTMOTOR_ACCESS_OUTPUT.AxisY.CommandValue = command;
    RTMOTOR_ACCESS_OUTPUT.AxisZ.CommandValue = command;

    //Command Ethercat Head Module 2
    if (head2Position > 0)
    {
      HEAD2_ACCESS_OUTPUT.watchdog = !HEAD2_ACCESS_OUTPUT.watchdog;

      if (tempo_head < 3999)
        tempo_head++;
      else
      {
        tempo_head = 0;
        HEAD2_ACCESS_OUTPUT.motorOn = !HEAD2_ACCESS_OUTPUT.motorOn;
      }
    }
    else
    {
      tempo_head = 0;
    }

    //Command Ethercat Head Module 1
    if (head1Position > 0)
    {
      HEAD1_ACCESS_OUTPUT.watchdog = !HEAD1_ACCESS_OUTPUT.watchdog;

      switch (state)
      {
      case 0://init
        HEAD1_ACCESS_OUTPUT.inverseDirection = FALSE;
        HEAD1_ACCESS_OUTPUT.motorOn = FALSE;

        if (HEAD1_ACCESS_INPUT.cam_contact == FALSE)
          state = 10;
        else
        {
          if (HEAD1_ACCESS_INPUT.low_position == TRUE)
            state = 2;
          else
            state = 1;
        }
        break;

      case 1://descente
        if (HEAD1_ACCESS_INPUT.cam_contact = FALSE)
          state = 99;
        else
        {
          HEAD1_ACCESS_OUTPUT.inverseDirection = TRUE;
          HEAD1_ACCESS_OUTPUT.motorOn = TRUE;

          if (HEAD1_ACCESS_INPUT.low_position == TRUE)
          {
            state = 2;
            HEAD1_ACCESS_OUTPUT.motorOn = FALSE;
          }
        }
        break;
      case 2://montee
        if (HEAD1_ACCESS_INPUT.cam_contact = FALSE)
          state = 99;
        else
        {
          HEAD1_ACCESS_OUTPUT.inverseDirection = FALSE;
          HEAD1_ACCESS_OUTPUT.motorOn = TRUE;

          if (HEAD1_ACCESS_INPUT.high_position == TRUE)
          {
            state = 3;
            HEAD1_ACCESS_OUTPUT.motorOn = FALSE;
          }
        }
        break;
      case 3://temp
        HEAD1_ACCESS_OUTPUT.motorOn = FALSE;
        if (++tempo_rtmotor >= 5000)
        {
          tempo_rtmotor = 0;
          state = 1;
        }
        break;
      case 10://research working range
        HEAD1_ACCESS_OUTPUT.inverseDirection = FALSE;
        HEAD1_ACCESS_OUTPUT.motorOn = TRUE;
        if (HEAD1_ACCESS_INPUT.cam_contact == TRUE)
        {
          state = 2;
          HEAD1_ACCESS_OUTPUT.motorOn = FALSE;
        }
        break;
      case 99://error
        HEAD1_ACCESS_OUTPUT.inverseDirection = FALSE;
        HEAD1_ACCESS_OUTPUT.motorOn = FALSE;
        break;
      }
    }
    else
    {
      state = 0;
      tempo_rtmotor = 0;
    }
  }
}
