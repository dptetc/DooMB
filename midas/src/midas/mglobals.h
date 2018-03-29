/*      MGLOBALS.H
 *
 * MIDAS Digital Audio System global variables
 *
 * $Id: mglobals.h,v 1.14 1997/07/31 14:30:16 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __MGLOBALS_H
#define __MGLOBALS_H


#ifdef __cplusplus
extern "C" {
#endif

/* Possible DirectSound modes: */
enum mDSoundModes
{
    M_DSOUND_DISABLED = 0,              /* DirectSound disabled */
    M_DSOUND_STREAM,                    /* DirectSound stream mode */
    M_DSOUND_PRIMARY,                   /* DirectSound primary buffer mode */
    M_DSOUND_FORCE_STREAM               /* DirectSound stream force mode.
                                           Causes DirectSound to be always
                                           used, in steream mode, even through
                                           emulation */
};


/* Preferred mixing mode: */
enum mPreferredMixModes
{
    M_MIX_AUTODETECT = 0,
    M_MIX_INTEGER,
    M_MIX_FLOAT
};


extern int GLOBALVAR mUseEMS;           /* 1 if EMS should be used */
extern int GLOBALVAR mEnableSurround;   /* 1 if surround sound is enabled */
extern int GLOBALVAR mBufferLength;     /* buffer length in milliseconds */
extern int GLOBALVAR mBufferBlocks;     /* number of buffer blocks
                                           (not applicable to all SDs) */
extern int GLOBALVAR mDefaultFramerate; /* default framerate (in 100*Hz), used
                                           when screen sync is not available*/
extern int GLOBALVAR mSyncScreen;       /* 1 if timer can be synchronized to
                                           screen */
extern int GLOBALVAR mDSoundMode;       /* DirectSound mode, see enum
                                           mDSoundModes above */
extern void* GLOBALVAR mDSoundObject;   /* the DirectSound object */
extern void* GLOBALVAR mDSoundHwnd;     /* HWND for DirectSound */
extern int GLOBALVAR mDSoundBufferLength; /* Buffer length for DirectSound,
                                            not used with emulation */
extern int GLOBALVAR m16bitUlawAutoConvert;  /* Automatically convert 16-bit
                                                samples to u-law for SW
                                                mixing */
extern int GLOBALVAR mOutputFilterMode; /* Output filter mode */
extern int GLOBALVAR mOversampling;     /* Oversampling */
extern int GLOBALVAR mDefaultStereoSeparation; /* Default stereo separation */
extern int GLOBALVAR mLoopModule;       /* Whether modules are looped by
                                           default */
extern int GLOBALVAR mDisableVolumeRamping;    /* Disable volume ramping from
                                                  mixing routines */
extern int GLOBALVAR mPreferredMixingMode;     /* Preferred mixing mode */
extern int GLOBALVAR mDisableDSound;           /* Disable DirectSound */





#ifdef __cplusplus
}
#endif




#endif


/*
 * $Log: mglobals.h,v $
 * Revision 1.14  1997/07/31 14:30:16  pekangas
 * Added mDisableDSound
 *
 * Revision 1.13  1997/07/31 10:56:46  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.12  1997/07/23 17:27:58  pekangas
 * Added profiling support
 *
 * Revision 1.11  1997/07/17 09:54:46  pekangas
 * Added option for disabling volume ramping
 *
 * Revision 1.10  1997/07/15 18:24:36  jpaana
 * Added mLoopModule
 *
 * Revision 1.9  1997/07/09 08:57:59  pekangas
 * Added default stereo separation
 *
 * Revision 1.8  1997/06/05 20:18:47  pekangas
 * Added preliminary support for interpolating mixing (mono only at the
 * moment)
 *
 * Revision 1.7  1997/05/03 17:53:26  pekangas
 * Added some optional simple output filtering
 *
 * Revision 1.6  1997/03/05 19:35:03  pekangas
 * Added flag m16bitUlawAutoConvert - controls whether 16-bit samples are
 * automatically converted to u-law or not.
 *
 * Revision 1.5  1997/02/06 20:58:19  pekangas
 * Added DirectSound support - new files, errors, and global flags
 *
 * Revision 1.4  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.3  1996/05/31 21:40:35  pekangas
 * Added mSyncScreen and mDefaultFramerate
 *
 * Revision 1.2  1996/05/25 09:32:26  pekangas
 * Added mBufferLength and mBufferBlocks
 *
 * Revision 1.1  1996/05/22 20:49:33  pekangas
 * Initial revision
 *
*/
