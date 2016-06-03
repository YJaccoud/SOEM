/*
 * Simple Open EtherCAT Master Library
 *
 * File    : osal.c
 * Version : 1.3.1
 * Date    : 11-03-2015
 * Copyright (C) 2005-2015 Speciaal Machinefabriek Ketels v.o.f.
 * Copyright (C) 2005-2015 Arthur Ketels
 * Copyright (C) 2008-2009 TU/e Technische Universiteit Eindhoven
 * Copyright (C) 2012-2015 rt-labs AB , Sweden
 *
 * SOEM is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation.
 *
 * SOEM is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * As a special exception, if other files instantiate templates or use macros
 * or inline functions from this file, or you compile this file and link it
 * with other works to produce a work based on this file, this file does not
 * by itself cause the resulting work to be covered by the GNU General Public
 * License. However the source code for this file must still be made available
 * in accordance with section (3) of the GNU General Public License.
 *
 * This exception does not invalidate any other reasons why a work based on
 * this file might be covered by the GNU General Public License.
 *
 * The EtherCAT Technology, the trade name and logo “EtherCAT” are the intellectual
 * property of, and protected by Beckhoff Automation GmbH. You can use SOEM for
 * the sole purpose of creating, using and/or selling or otherwise distributing
 * an EtherCAT network master provided that an EtherCAT Master License is obtained
 * from Beckhoff Automation GmbH.
 *
 * In case you did not receive a copy of the EtherCAT Master License along with
 * SOEM write to Beckhoff Automation GmbH, Eiserstraße 5, D-33415 Verl, Germany
 * (www.beckhoff.com).
 */
#include <stdlib.h>

#include <windows.h>
#include <iwin32.h>
#include <rt.h>

#include <sys/time.h>
#include <osal.h>

static int64_t sysfrequency;
static double qpc2usec;

#define USECS_PER_SEC     1000000

//Internal
int osal_wait_for_single_object(void **thandle, uint32 timeout_us)
{
  DWORD ret;
  switch (timeout_us)
  {
  case OSAL_NO_WAIT:
    ret = WaitForSingleObject(*thandle, NO_WAIT);
    break;
  case OSAL_WAIT_INFINITE:
    ret = WaitForSingleObject(*thandle, WAIT_FOREVER);
    break;
  default:
    ret = WaitForSingleObject(*thandle, timeout_us / 1000);
    break;
  }

  if (ret == WAIT_OBJECT_0)
    return (int)1;
  else
    return (int)0;
}

int osal_CloseHandle(void **thandle)
{
  return (int)CloseHandle(*thandle);
}

int osal_gettimeofday(struct timeval *tv, void *tz)
{
   return gettimeofday(tv, tz);   //tz not used
}

//Date and Time
ec_timet osal_current_time(void)
{
   struct timeval current_time;
   ec_timet return_value;

   osal_gettimeofday(&current_time, 0);
   return_value.sec = current_time.tv_sec;
   return_value.usec = current_time.tv_usec;
   return return_value;
}

int osal_usleep(uint32 usec)
{
   RtSleepEx (usec / 1000);
   return 1;
}

void osal_time_diff(ec_timet *start, ec_timet *end, ec_timet *diff)
{
   diff->sec = end->sec - start->sec;
   diff->usec = end->usec - start->usec;
   if (diff->usec < 0) {
     --diff->sec;
     diff->usec += 1000000;
   }
}

void osal_timer_start(osal_timert * self, uint32 timeout_usec)
{
  struct timeval start_time;
  struct timeval timeout;
  struct timeval stop_time;

  osal_gettimeofday(&start_time, 0);
  timeout.tv_sec = timeout_usec / USECS_PER_SEC;
  timeout.tv_usec = timeout_usec % USECS_PER_SEC;
  timeradd(&start_time, &timeout, &stop_time);

  self->stop_time.sec = stop_time.tv_sec;
  self->stop_time.usec = stop_time.tv_usec;
}

boolean osal_timer_is_expired(osal_timert * self)
{
  struct timeval current_time;
  struct timeval stop_time;
  int is_not_yet_expired;

  osal_gettimeofday(&current_time, 0);
  stop_time.tv_sec = self->stop_time.sec;
  stop_time.tv_usec = self->stop_time.usec;
  is_not_yet_expired = timercmp(&current_time, &stop_time, <);

  return is_not_yet_expired == FALSE;
}

//Memory
void *osal_malloc(size_t size)
{
   return malloc(size);
}

void osal_free(void *ptr)
{
   free(ptr);
}

//Thread
int osal_thread_create(void **thandle, int stacksize, void *func, void *param)
{
  *thandle = CreateThread(NULL, stacksize, func, param, 0, NULL);
   if(!thandle)
      return 0;
   return 1;
}

int osal_thread_create_rt(void **thandle, int stacksize, void *func, void *param)
{
  int ret;
    ret = osal_thread_create(thandle, stacksize, func, param);
  if (ret)
    ret = SetThreadPriority(*thandle, THREAD_PRIORITY_ABOVE_NORMAL /*THREAD_PRIORITY_ABOVE_NORMAL = 126*/ /*THREAD_PRIORITY_HIGHEST = 117*/ /*THREAD_PRIORITY_TIME_CRITICAL = 0*/);
  return ret;
}

int osal_thread_is_terminated(void **thandle, uint32 timeout_us)
{
  return osal_wait_for_single_object(thandle, timeout_us);
}

int osal_thread_delete(void **thandle)
{
  return osal_CloseHandle(thandle);
}

//Event
int osal_event_create(void **thandle)
{
  HANDLE handle;
  handle = CreateEvent(NULL, FALSE, FALSE, NULL);
  if (handle == NULL)
  {
    *thandle = NULL;
    return 0;
  }
  else
  {
    *thandle = handle;
    return 1;
  }
}

int osal_event_delete(void **thandle)
{
  return osal_CloseHandle(thandle);
}

int osal_event_set(void **thandle)
{
  return (int)SetEvent(*thandle);
}

int osal_event_reset(void **thandle)
{
  return (int)ResetEvent(*thandle);
}

int osal_event_pulse(void **thandle)
{
  return (int)PulseEvent(*thandle);
}

int osal_event_wait(void **thandle, uint32 timeout_us)
{
  return osal_wait_for_single_object(thandle, timeout_us);
}

/* Mutex is not needed when running single threaded */
/*
void osal_mtx_lock(osal_mutex_t * mtx)
{
        //RtWaitForSingleObject((HANDLE)mtx, INFINITE);
}

void osal_mtx_unlock(osal_mutex_t * mtx)
{
        //RtReleaseMutex((HANDLE)mtx);
}

int osal_mtx_lock_timeout(osal_mutex_t * mtx, uint32_t time_ms)
{
        //return RtWaitForSingleObject((HANDLE)mtx, time_ms);
        return 0;
}

osal_mutex_t * osal_mtx_create(void)
{
        //return (void*)RtCreateMutex(NULL, FALSE, NULL);
        return (void *)0;
}
*/