/*      midas.h
 *
 * Simplified MIDAS Digital Audio System API
 *
 * $Id: midas.h,v 1.28.2.1 1997/08/21 22:32:50 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __MIDAS_H
#define __MIDAS_H



/****************************************************************************\
*      MIDAS version defines
\****************************************************************************/

#define MVERSION 1.1.2
#define MVERSTR "1.1.2"
#define MVERNUM 0x1102


/****************************************************************************\
*      MIDAS header files:
\****************************************************************************/

#include "lang.h"

/* *nix hack: #include this here to get rid of type name conflicts: */
#ifdef __UNIX__
#include <sys/types.h>
#endif

#include "mtypes.h"
#include "errors.h"
#include "mglobals.h"
#include "mmem.h"
#include "file.h"
#include "sdevice.h"
#include "gmplayer.h"
#ifndef __UNIX__
#include "timer.h"
#include "dma.h"
#endif
#include "mutils.h"
#ifndef NOEMS
#include "ems.h"
#endif
#if defined(__WIN32__)||defined(__UNIX__)
#include "mpoll.h"
#endif
#include "madpcm.h"
#include "waveread.h"
#include "midasfx.h"
#include "midasstr.h"



#ifdef __cplusplus
extern "C" {
#endif



/****************************************************************************\
*      MIDAS global variables:
\****************************************************************************/

#ifdef __WIN32__

extern SoundDevice GLOBALVAR WinWave;   /* Windows Wave Sound Device */
extern SoundDevice GLOBALVAR MixNoSound; /* Mixing NoSound SD */
#ifndef NODSOUND
extern SoundDevice GLOBALVAR DSound;    /* DirectSound Sound Device */
#endif

#ifdef NODSOUND
#define NUMSDEVICES 2                   /* total number of Sound Devices */
#else
#define NUMSDEVICES 3                   /* total number of Sound Devices */
#endif

#else /* __WIN32__ */

#ifdef __UNIX__

extern SoundDevice GLOBALVAR OSS;   /* OSS Sound Device */
extern SoundDevice GLOBALVAR MixNoSound; /* Mixing NoSound SD */
#define NUMSDEVICES 2

#else /* __UNIX__ */

#ifdef M_LITE
extern SoundDevice GLOBALVAR GUS;       /* Gravis UltraSound Sound Device */
#endif
extern SoundDevice GLOBALVAR mGusMix;   /* Gravis UltraSound Mixing Device */
extern SoundDevice GLOBALVAR PAS;       /* Pro Audio Spectrum Sound Device */
extern SoundDevice GLOBALVAR WSS;       /* Windows Sound System Sound Device*/
extern SoundDevice GLOBALVAR SB;        /* Sound Blaster Sound Device */
extern SoundDevice GLOBALVAR MixNoSound; /* Mixing NoSound SD */

#ifdef M_LITE
#define NUMSDEVICES 6                   /* total number of Sound Devices */
#else
#define NUMSDEVICES 5
#endif
#endif
#endif

extern SoundDevice GLOBALVAR *midasSD;  /* current Sound Device */

    /* array to pointers to all Sound Devices, in numbering and detection
       order - GUS is SD #1: */
extern SoundDevice * GLOBALVAR midasSoundDevices[NUMSDEVICES];

extern gmpPlayHandle GLOBALVAR midasPlayHandle;  /* Generic Module Player
                                                    playing handle */
extern int GLOBALVAR midasDisableEMS;   /* 1 if EMS usage is disabled
                                           (default 0) */
extern int GLOBALVAR midasSDNumber;     /* Sound Device number (-1 for
                                           autodetect, default -1) */
extern int GLOBALVAR midasSDPort;       /* Sound Device I/O port number
                                           (-1 for autodetect or SD default,
                                           default -1) */
extern int GLOBALVAR midasSDIRQ;        /* Sound Device IRQ number (-1 for
                                           autodetect or SD default,
                                           default -1) */
extern int GLOBALVAR midasSDDMA;        /* Sound Device DMA channel number
                                           (-1 for autodetect or SD default,
                                           default -1) */
extern int GLOBALVAR midasSDCard;       /* Sound Device sound card type
                                           (-1 for autodetect or SD default,
                                           default -1) */
extern unsigned GLOBALVAR midasMixRate; /* Sound Device mixing rate */
extern unsigned GLOBALVAR midasOutputMode; /* Sound Device output mode force
                                           bits, default 0 (SD default) */
extern int GLOBALVAR midasAmplification;  /* Forced amplification level or -1
                                             for SD default (default -1) */
extern int GLOBALVAR midasChannels;     /* number of channels open or 0 if no
                                           channels have been opened using
                                           midasOpenChannels() */
extern int GLOBALVAR midasPlayerNum;    /* timer music player number */


extern int GLOBALVAR midasEMSInit;      /* is EMS heap manager initialized? */
extern int GLOBALVAR midasTMRInit;      /* is TempoTimer initialized? */
extern int GLOBALVAR midasTMRPlay;      /* is sound beind player with timer?*/
extern int GLOBALVAR midasSDInit;       /* is Sound Device initialized? */
extern int GLOBALVAR midasSDChans;      /* are Sound Device channels open? */
extern int GLOBALVAR midasGMPInit;      /* is GMP initialized? */
extern int GLOBALVAR midasGMPPlay;      /* is GMP playing? */
extern int GLOBALVAR midasTMRMusic;     /* is music being player with timer?*/

extern void (CALLING *midasErrorExit)(char *msg);   /* error exit function */




/****************************************************************************\
*      MIDAS constants:
\****************************************************************************/



/****************************************************************************\
*      MIDAS global functions from MIDAS.C:
\****************************************************************************/


/****************************************************************************\
*
* Function:     void midasError(int errNum)
*
* Description:  Prints a MIDAS error message to stderr and exits to DOS
*
* Input:        int errNum              MIDAS error code
*
\****************************************************************************/

void CALLING midasError(int errNum);




/****************************************************************************\
*
* Function:     void midasUninitError(int errNum)
*
* Description:  Prints an error message to stderr and exits to DOS without
*               uninitializing MIDAS. This function should only be used
*               from midasClose();
*
* Input:        int errNum              MIDAS error code
*
\****************************************************************************/

void CALLING midasUninitError(int errNum);



/****************************************************************************\
*
* Function:     void midasDetectSD(void)
*
* Description:  Attempts to detect a Sound Device. Sets the global variable
*               midasSD to point to the detected Sound Device or NULL if no
*               Sound Device was detected
*
\****************************************************************************/

void CALLING midasDetectSD(void);



/****************************************************************************\
*
* Function:     void midasInit(void)
*
* Description:  Initializes MIDAS Digital Audio System
*
\****************************************************************************/

void CALLING midasInit(void);



/****************************************************************************\
*
* Function:     void midasClose(void)
*
* Description:  Uninitializes MIDAS Digital Audio System
*
\****************************************************************************/

void CALLING midasClose(void);



/****************************************************************************\
*
* Function:     void midasSetDefaults(void)
*
* Description:  Initializes MIDAS Digital Audio System variables to their default
*               states. MUST be the first MIDAS function to be called.
*
\****************************************************************************/

void CALLING midasSetDefaults(void);




/****************************************************************************\
*
* Function:     void midasOpenChannels(int numChans);
*
* Description:  Opens Sound Device channels for sound and music output.
*
* Input:        int numChans            Number of channels to open
*
* Notes:        Channels opened with this function can be used for sound
*               playing, and modules played with midasPlayModule() will be
*               played through the last of these channels. This function is
*               provided so that the same number of channels can be open
*               the whole time throughout the execution of the program,
*               keeping the volume level constant. Note that you must ensure
*               that you open enough channels for all modules, otherwise
*               midasPlayModule() will fail.
*
\****************************************************************************/

void CALLING midasOpenChannels(int numChans);




/****************************************************************************\
*
* Function:     void midasCloseChannels(void);
*
* Description:  Closes Sound Device channels opened with midasOpenChannels().
*               Do NOT call this function unless you have opened the sound
*               channels used yourself with midasOpenChannels().
*
\****************************************************************************/

void CALLING midasCloseChannels(void);




/****************************************************************************\
*
* Function:     midasPlayModule(gmpModule *module, int numEffectChns)
*
* Description:  Starts playing a Generic Module Player module loaded to memory
*
* Input:        gmpModule *module       Pointer to loaded module structure
*               int numEffectChns       Number of channels to open for sound
*                                       effects. Ignored if sound channels
*                                       have already been opened with
*                                       midasOpenChannels().
*
* Returns:      Pointer to module structure. This function can not fail,
*               as it will call midasError() to handle all error cases.
*
* Notes:        The Sound Device channels available for sound effects are the
*               _first_ numEffectChns channels. So, for example, if you use
*               midasPlayModule(module, 3), you can use channels 0-2 for sound
*               effects. If you already have opened channels with
*               midasOpenChannels(), the module will be played with the last
*               possible channels, so that the first channels will be
*               available for sound effects. Note that if not enough channels
*               are open this function will fail.
*
\****************************************************************************/

void CALLING midasPlayModule(gmpModule *module, int numEffectChns);




/****************************************************************************\
*
* Function:     void midasStopModule(gmpModule *module)
*
* Input:        gmpModule *module       the module which is being played
*
* Description:  Stops playing a module and uninitializes the Module Player.
*               If sound channels were NOT opened through midasOpenChannels(),
*               but by letting midasPlayModule() open them, they will be
*               closed. Sound channels opened with midasOpenChannels() are NOT
*               closed and must be closed separately.
*
\****************************************************************************/

void CALLING midasStopModule(gmpModule *module);




/****************************************************************************\
*
* Function:     void midasSetErrorExit(void (CALLING *errorExit)(char *msg))
*
* Description:  Sets error exit function.
*
* Input:        void (CALLING *errorExit)() Pointer to the function that will
*                                           be called to exit the program with
*                                           an error message.
*
\****************************************************************************/

void midasSetErrorExit(void (CALLING *midasErrorExit)(char *msg));




#ifdef __cplusplus
}
#endif


#endif


/*
 * $Log: midas.h,v $
 * Revision 1.28.2.1  1997/08/21 22:32:50  pekangas
 * The Mixing No Sound Sound Device is now used in the Linux version too
 *
 * Revision 1.28  1997/08/18 14:01:17  pekangas
 * Removed dsm.h from midas.h
 *
 * Revision 1.27  1997/08/01 18:02:29  jpaana
 * Changed version from 1.1 alpha to beta
 *
 * Revision 1.26  1997/07/31 15:54:13  pekangas
 * Changed DOS to use mixing No Sound Sound Device
 *
 * Revision 1.25  1997/07/31 10:56:47  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.24  1997/07/11 15:39:49  pekangas
 * Boosted version to 1.1 alpha
 *
 * Revision 1.23  1997/06/02 00:54:14  jpaana
 * Changed most __LINUX__ defines to __UNIX__ for generic *nix porting
 *
 * Revision 1.22  1997/05/20 20:37:10  pekangas
 * Added RIFF WAVE reader
 *
 * Revision 1.21  1997/05/03 15:10:50  pekangas
 * Added stream support for DOS, removed GUS Sound Device from non-Lite
 * build. M_HAVE_THREADS now defined in threaded environment.
 *
 * Revision 1.20  1997/04/08 15:47:34  pekangas
 * Fixed to compile with Linux without warnings
 *
 * Revision 1.19  1997/02/27 16:03:18  pekangas
 * Changed GUSDAC to mGusMix
 *
 * Revision 1.18  1997/02/12 16:27:33  pekangas
 * Now includes madpcm.h
 *
 * Revision 1.17  1997/02/06 20:58:19  pekangas
 * Added DirectSound support - new files, errors, and global flags
 *
 * Revision 1.16  1997/02/05 22:55:04  pekangas
 * Small fixes to get new build system work better. Changed MIDAS version
 * to 0.6.1
 *
 * Revision 1.15  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.14  1996/10/14 14:57:24  pekangas
 * Changed version to 0.5.0
 *
 * Revision 1.13  1996/09/22 23:02:52  pekangas
 * Now #include midasfx.h and midasstr.h
 *
 * Revision 1.12  1996/09/21 17:18:01  jpaana
 * mpoll.h is included also in Linux
 *
 * Revision 1.11  1996/09/21 16:38:00  jpaana
 * Changed uss to oss
 *
 * Revision 1.10  1996/09/08 20:21:20  pekangas
 * Boosted version to RC2
 *
 * Revision 1.9  1996/08/06 20:35:51  pekangas
 * Now #includes mpoll.h for WIN32 targets
 *
 * Revision 1.8  1996/08/06 18:45:25  pekangas
 * Changed version number to 0.5.rc1
 *
 * Revision 1.7  1996/07/29 19:36:08  pekangas
 * Added MixNoSound Sound Device for Win32
 *
 * Revision 1.6  1996/07/13 20:32:19  pekangas
 * Fixed midasSoundDevices[] with Visual C
 *
 * Revision 1.5  1996/07/13 18:18:24  pekangas
 * Fixed to compile with Visual C
 *
 * Revision 1.4  1996/06/05 19:40:35  jpaana
 * Changed usswave to uss
 *
 * Revision 1.3  1996/05/25 15:49:57  jpaana
 * Small fix
 *
 * Revision 1.2  1996/05/24 16:20:36  jpaana
 * Added USSWave device and fixed Linux support
 *
 * Revision 1.1  1996/05/22 20:49:33  pekangas
 * Initial revision
 *
*/
