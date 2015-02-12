
/* Copyright 2014, Nelson Lombardo
 *
 * This file is part of  Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief  POSIX source file
 **
 ** This file contains the pseudo-POSIX implementation
 **
 **/

/** \addtogroup _Firmware  Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */
/*
 * Initials     Name
 * ---------------------------
 * EzEs         Ezequiel Esposito
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140528 v0.0.2 MaCe implement printf
 * 20140420 v0.0.1 EzEs initial version
 */

/*==================[inclusions]=============================================*/
#include "k.h"            /* <=  kernel header */
#include "POSIX_stdio.h"
#include "POSIX_string.h"
#include "POSIX_stdlib.h"
#include "POSIX_assert.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
/** \brief Filedescriptor type */
typedef struct {
   Devices_deviceType * device;
} POSIX_stdio_fildesType;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
/** \brief List of files descriptors */
POSIX_stdio_fildesType POSIX_stdio_fildes[POSIX_stdio_MAXFILDES];

/** \brief device prefix */
char const * const POSIX_stdio_devPrefix = "/dev";

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void POSIX_init(void)
{
   uint32_t loopi;

   /* init all posix devices */
   for (loopi = 0; loopi < POSIX_stdio_MAXFILDES; loopi++) {
      POSIX_stdio_fildes[loopi].device = NULL;
   }
}

extern int32_t POSIX_open(char const * path, uint8_t oflag)
{
   Devices_deviceType * device;
   Devices_deviceType * rewriteDevice;
   int32_t ret = -1;
   int8_t loopi;

   /* check if device */
   if (POSIX_strncmp(path,
            POSIX_stdio_devPrefix,
            POSIX_strlen(POSIX_stdio_devPrefix)) == 0)
   {
      /* get device */
      device = Devices_getDevice(path);

      /* if a device has been found */
      if (NULL != device)
      {
         /* search a file descriptor */
         for(loopi = 0; (loopi < POSIX_stdio_MAXFILDES) && (-1 == ret); loopi++)
         {
            /* enter critical section */
#ifdef POSIXR
            GetResource(POSIXR);
#else /* #ifdef POSIXR */
            SuspendOSInterrupts();
#endif /* #ifdef POSIXR */

            /* if file descriptor not used, use it */
            if (NULL == POSIX_stdio_fildes[loopi].device)
            {
               /* load device in descriptor */
               POSIX_stdio_fildes[loopi].device = device;

               /* return file descriptor */
               ret = loopi;
            }

            /* exit critical section */
#ifdef POSIXR
            ReleaseResource(POSIXR);
#else /* #ifdef POSIXR */
            ResumeOSInterrupts();
#endif /* #ifdef POSIXR */
         }

         /* if a file descriptor has been found */
         if (-1 != ret)
         {
            /* open device */
            rewriteDevice = POSIX_stdio_fildes[ret].device->open(path,
                  POSIX_stdio_fildes[ret].device,
                  oflag);
            if (NULL != rewriteDevice)
            {
               /* open device successfull */
               POSIX_stdio_fildes[loopi].device = rewriteDevice;
            }
            else
            {
               /* device could not be opened */

               /* enter critical section */
#ifdef POSIXR
               GetResource(POSIXR);
#else /* #ifdef POSIXR */
               SuspendOSInterrupts();
#endif /* #ifdef POSIXR */

               /* remove device from file descriptor */
               POSIX_stdio_fildes[ret].device = NULL;

               /* exit critical section */
#ifdef POSIXR
               ReleaseResource(POSIXR);
#else /* #ifdef POSIXR */
               ResumeOSInterrupts();
#endif /* #ifdef POSIXR */

               /* return an error */
               ret = -1;
            }
         }

      }
   }
   else
   {
      POSIX_assert(0);
      /* TODO implement file handler */
   }

   return ret;
} /* end POSIX_open */

int32_t POSIX_close(int32_t fildes)
{
   int32_t ret = -1;

   if ( (fildes >= 0) && (fildes < POSIX_stdio_MAXFILDES) )
   {
      if (NULL != POSIX_stdio_fildes[fildes].device)
      {
         ret = POSIX_stdio_fildes[fildes].device->close(POSIX_stdio_fildes[fildes].device);
         if (0 == ret)
         {
            /* free file descriptor, file has been closed */
            POSIX_stdio_fildes[fildes].device = NULL;
         }
         else
         {
            /* allowed return values are -1 and 0, if failed force -1 */
            ret = -1;
         }
      }
   }

   return ret;
}

int32_t POSIX_ioctl (int32_t fildes, int32_t request, void * param)
{
   int32_t ret = -1;

   /* check that file descriptor is on range */
   if ( (fildes >= 0) && (fildes < POSIX_stdio_MAXFILDES) )
   {
      /* check that file descriptor is beeing used */
      if (NULL != POSIX_stdio_fildes[fildes].device)
      {
         switch(request)
         {
            case POSIX_IOCTL_RXINDICATION:
               /* store callback */
               /* TODO continue here */
               break;

            default:
               /* nothing to be processed */
               /* call ioctl function */
               ret = POSIX_stdio_fildes[fildes].device->ioctl(
                           POSIX_stdio_fildes[fildes].device,
                           request,
                           param);
               break;
         }
      }
   }

   return ret;
}

ssize_t POSIX_read(int32_t fildes, void * buf, ssize_t nbyte)
{
   int32_t ret = -1;

   /* check that file descriptor is on range */
   if ( (fildes >= 0) && (fildes < POSIX_stdio_MAXFILDES) )
   {
      /* check that file descriptor is beeing used */
      if (NULL != POSIX_stdio_fildes[fildes].device)
      {
         /* call read function */
         ret = POSIX_stdio_fildes[fildes].device->read(
               POSIX_stdio_fildes[fildes].device,
               buf,
               nbyte);
      }
   }

   return ret;
}

extern ssize_t POSIX_write (int32_t fildes, void const * buf, size_t nbyte)
{
   int32_t ret = -1;

   /* check that file descriptor is on range */
   if ( (fildes >= 0) && (fildes < POSIX_stdio_MAXFILDES) )
   {
      /* check that file descriptor is beeing used */
      if (NULL != POSIX_stdio_fildes[fildes].device)
      {
         /* call write function */
         ret = POSIX_stdio_fildes[fildes].device->write(
               POSIX_stdio_fildes[fildes].device,
               buf,
               nbyte);
      }
   }

   return ret;
}

extern int32_t POSIX_printf(const char * format, ...)
{
   int32_t ret;
   /* parameter format is not used in no win nor posix arch, casted to void to
    * avoid compiler warning */
   (void)format;
   /* this interface is not supported in no windows nor posix system */
   ret = -1;
#endif

   return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

