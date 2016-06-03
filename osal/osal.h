/******************************************************************************
 *                *          ***                    ***
 *              ***          ***                    ***
 * ***  ****  **********     ***        *****       ***  ****          *****
 * *********  **********     ***      *********     ************     *********
 * ****         ***          ***              ***   ***       ****   ***
 * ***          ***  ******  ***      ***********   ***        ****   *****
 * ***          ***  ******  ***    *************   ***        ****      *****
 * ***          ****         ****   ***       ***   ***       ****          ***
 * ***           *******      ***** **************  *************    *********
 * ***             *****        ***   *******   **  **  ******         *****
 *                           t h e  r e a l t i m e  t a r g e t  e x p e r t s
 *
 * http://www.rt-labs.com
 * Copyright (C) 2009. rt-labs AB, Sweden. All rights reserved.
 *------------------------------------------------------------------------------
 * $Id: osal.h 473 2013-04-08 11:43:02Z rtlaka $
 *------------------------------------------------------------------------------
 */

#ifndef _osal_
#define _osal_

#ifdef __cplusplus
extern "C"
{
#endif

#include "osal_defs.h"
#include <stdint.h>

/* General types */
typedef uint8_t             boolean;
#define TRUE                1
#define FALSE               0
typedef int8_t              int8;
typedef int16_t             int16;
typedef int32_t             int32;
typedef uint8_t             uint8;
typedef uint16_t            uint16;
typedef uint32_t            uint32;
typedef int64_t             int64;
typedef uint64_t            uint64;
typedef float               float32;
typedef double              float64;

typedef struct
{
    uint32 sec;     /*< Seconds elapsed since the Epoch (Jan 1, 1970) */
    uint32 usec;    /*< Microseconds elapsed since last second boundary */
} ec_timet;

typedef struct osal_timer
{
    ec_timet stop_time;
} osal_timert;

#define OSAL_NO_WAIT                0
#define OSAL_WAIT_INFINITE 4294967295

ec_timet osal_current_time(void);
int osal_usleep(uint32 usec);
void osal_time_diff(ec_timet *start, ec_timet *end, ec_timet *diff);
void osal_timer_start(osal_timert * self, uint32 timeout_us);
boolean osal_timer_is_expired(osal_timert * self);
int osal_thread_create(void *thandle, int stacksize, void *func, void *param);
int osal_thread_create_rt(void *thandle, int stacksize, void *func, void *param);
int osal_thread_is_terminated(void **thandle, uint32 timeout_us);
int osal_thread_delete(void **thandle);
int osal_event_create(void **thandle);
int osal_event_delete(void **thandle);
int osal_event_set(void **thandle);
int osal_event_reset(void **thandle);
int osal_event_pulse(void **thandle);
int osal_event_wait(void **thandle, uint32 timeout_us);

#ifdef __cplusplus
}
#endif

#endif
