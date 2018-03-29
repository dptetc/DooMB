/*      MIXSD.H
 *
 * Miscellaneous helper functions common to all mixing sound devices.
 * Technically these functions should be part of each Sound Device's internal
 * code, but are here to save some space and help maintenance.
 *
 * $Id: mixsd.h,v 1.4 1997/07/31 10:56:52 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __MIXSD_H
#define __MIXSD_H


#include "dma.h"


#ifdef __cplusplus
extern "C" {
#endif


extern unsigned GLOBALVAR dmaBufferSize;
extern dmaBuffer GLOBALVAR buffer;
extern unsigned GLOBALVAR dmaPos;


/****************************************************************************\
*
* Function:     int mixsdInit(unsigned mixRate, unsigned mode,
*                   unsigned dmaChNum)
*
* Description:  Common initialization for all mixing Sound Devices.
*               Initializes DMA functions, DSM, start DMA playback and
*               allocates memory for possible post-processing tables
*
* Input:        unsigned mixRate        mixing rate in Hz
*               unsigned mode           output mode
*               int dmaChNum            DMA channel number / -1
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mixsdInit(unsigned mixRate, unsigned mode, int dmaChNum);




/****************************************************************************\
*
* Function:     int CALLING mixsdClose(void)
*
* Description:  Common uninitialization code for all mixing Sound Devices.
*               Uninitializes DMA playback and DSM and deallocates memory.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mixsdClose(void);




/****************************************************************************\
*
* Function:     int mixsdGetMode(unsigned *mode)
*
* Description:  Reads the current output mode
*
* Input:        unsigned *mode          pointer to output mode
*
* Returns:      MIDAS error code. Output mode is written to *mode.
*
\****************************************************************************/

int CALLING mixsdGetMode(unsigned *mode);



/****************************************************************************\
*
* Function:     int mixsdSetUpdRate(unsigned updRate);
*
* Description:  Sets the channel value update rate (depends on song tempo)
*
* Input:        unsigned updRate        update rate in 100*Hz (eg. 50Hz
*                                       becomes 5000).
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mixsdSetUpdRate(unsigned updRate);




/****************************************************************************\
*
* Function:     int mixsdStartPlay(void)
*
* Description:  Prepares for playing - reads DMA playing position. Called
*               once before the Sound Device and music player polling loop.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mixsdStartPlay(void);




/****************************************************************************\
*
* Function:     int mixsdPlay(int *callMP);
*
* Description:  Plays the sound - mixes the correct amount of data with DSM
*               and copies it to DMA buffer with post-processing.
*
* Input:        int *callMP             pointer to music player calling flag
*
* Returns:      MIDAS error code. If enough data was mixed for one updating
*               round and music player should be called, 1 is written to
*               *callMP, otherwise 0 is written there. Note that if music
*               player can be called, mixsdPlay() should be called again
*               with a new check for music playing to ensure the DMA buffer
*               gets filled with new data.
*
\****************************************************************************/

int CALLING mixsdPlay(int *callMP);



#ifdef __cplusplus
}
#endif





/****************************************************************************\
*       enum mixsdFunctIDs
*       ------------------
* Description:  ID numbers for mixing Sound Device common functions
\****************************************************************************/

enum mixsdFunctIDs
{
    ID_mixsdInit = ID_mixsd,
    ID_mixsdClose,
    ID_mixsdGetMode,
    ID_mixsdOpenChannels,
    ID_mixsdSetAmplification,
    ID_mixsdGetAmplification,
    ID_mixsdSetUpdRate,
    ID_mixsdStartPlay,
    ID_mixsdPlay
};



#endif


/*
 * $Log: mixsd.h,v $
 * Revision 1.4  1997/07/31 10:56:52  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.3  1997/06/20 10:08:22  pekangas
 * Fixed to work with new mixing routines
 *
 * Revision 1.2  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.1  1996/05/22 20:49:33  pekangas
 * Initial revision
 *
*/
