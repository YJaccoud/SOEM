#ifndef _EC_SLAVESAP_H
#define _EC_SLAVESAP_H

#ifdef __cplusplus
extern "C"
{
#endif

	#pragma pack(push, 1)
	
	typedef struct
	{
	  unsigned short pwmMicroscope : 12;
	  unsigned short motorOn : 1;
	  unsigned short inverseDirection : 1;
	  unsigned short : 1;
	  unsigned short watchdog : 1;
	} HeadOutputStruct;

	typedef struct
	{
	  unsigned short : 2;
	  unsigned short cam_contact : 1;
	  unsigned short high_position : 1;
	  unsigned short low_position : 1;
	  unsigned short compass : 2;
	  unsigned short : 5;
	  unsigned short moduleIndex : 5;
	} HeadInputStruct;

	typedef struct
	{
	  unsigned char watchdog : 1;
	  unsigned char sync_ttl : 1;
	  unsigned char : 6;
	} AxisGeneralCommandRegister;

	typedef struct
	{
	  unsigned char emergency_stop : 1;
	  unsigned char head_collision : 1;
	  unsigned char watchdog : 1;
	  unsigned char : 5;
	} AxisGeneralControlRegister;

	typedef struct
	{
	  unsigned char sttoper_home : 1;
	  unsigned char sttoper_up : 1;
	  unsigned char trigger_state : 1;
	  unsigned char : 5;
	} AxisControlRegister;

	typedef struct
	{
	  unsigned char motor_on : 1;
	  unsigned char reset_position : 1;
	  unsigned char trigger_reset : 1;
	  unsigned char trigger_active : 1;
	  unsigned char : 4;
	} AxisCommandRegister;

	typedef struct
	{
	  signed long         TriggerPosition;
	  signed short        CommandValue;
	  AxisCommandRegister CommandRegister;
	} AxisOutputStruct;

	typedef struct
	{
	  signed long         ActualPosition;
	  signed long         LastIndexPosition;
	  signed short        ActualCurrent;
	  signed long         ActualSpeed;
	  AxisControlRegister StatusRegister;
	} AxisInputStruct;

	typedef struct
	{
	  AxisGeneralCommandRegister ConfigRegister;
	  AxisOutputStruct           AxisX;
	  AxisOutputStruct           AxisY;
	  AxisOutputStruct           AxisZ;
	} RtMotorOutputStruct;

	typedef struct
	{
	  AxisGeneralControlRegister StatusRegister;
	  AxisInputStruct            AxisX;
	  AxisInputStruct            AxisY;
	  AxisInputStruct            AxisZ;
	} RtMotorInputStruct;
	
	#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* _EC_ECATDC_H */