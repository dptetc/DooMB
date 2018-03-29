/*      midasfx.h
 *
 * MIDAS sound effect library
 *
 * $Id: midasfx.h,v 1.6 1997/07/31 10:56:49 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/


#ifndef __midasfx_h
#define __midasfx_h



/****************************************************************************\
*       enum fxChannels
*       ---------------
* Description:  Possible values for effect channel numbers apart from regular
*               channel numbers
\****************************************************************************/

enum fxChannels
{
    fxAutoChannel = 0xFFFF
};



/****************************************************************************\
*       enum fxSamplePlayStatus
*       -----------------------
* Description:  Sample playing status values
\****************************************************************************/

enum fxSamplePlayStatus
{
    fxSampleStopped = 0,
    fxSamplePlaying
};



/****************************************************************************\
*       struct fxChannel
*       ----------------
* Description:  Sound effect channel structure
\****************************************************************************/

typedef struct
{
    unsigned    sdChannel;              /* Sound Device channel number */
    unsigned    sampleHandle;           /* Handle for the sample that is
                                           being played on this channel */
    unsigned    playHandle;             /* Playing handle for the sound on
                                           this channel */
    int         priority;               /* Channel sound priority */
} fxChannel;



#ifdef __cplusplus
extern "C" {
#endif



/****************************************************************************\
*
* Function:     int fxInit(SoundDevice *SD)
*
* Description:  Initializes the sound effect library
*
* Input:        SoundDevice *SD         Pointer to the Sound Device that will
*                                       be used for playing the effects
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING fxInit(SoundDevice *SD);




/****************************************************************************\
*
* Function:     int fxClose(void)
*
* Description:  Uninitializes the sound effect library
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING fxClose(void);




/****************************************************************************\
*
* Function:     int fxLoadRawSample(char *fileName, unsigned sampleType,
*                   int loopSample, unsigned *sampleHandle)
*
* Description:  Loads a raw sample file into memory and adds the sample to
*               the sound device.
*
* Input:        char *fileName          sample file name
*               unsigned sampleType     sample type, see enum sdSampleType
*               int loopSample          1 if sample is looped, 0 if not
*               unsigned *sampleHandle  pointer to sample handle variable
*
* Returns:      MIDAS error code. The sample handle for the sample will be
*               written to *sampleHandle.
*
\****************************************************************************/

int CALLING fxLoadRawSample(char *fileName, unsigned sampleType,
    int loopSample, unsigned *sampleHandle);




/****************************************************************************\
*
* Function:     int fxLoadWaveSample(char *fileName, int loopSample,
*                   unsigned *sampleHandle)
*
* Description:  Loads a raw sample file into memory and adds the sample to
*               the sound device.
*
* Input:        char *fileName          sample file name
*               int loopSample          1 if sample is looped, 0 if not
*               unsigned *sampleHandle  pointer to sample handle variable
*
* Returns:      MIDAS error code. The sample handle for the sample will be
*               written to *sampleHandle.
*
\****************************************************************************/

int CALLING fxLoadWaveSample(char *fileName, int loopSample,
                             unsigned *sampleHandle);




/****************************************************************************\
*
* Function:     int fxFreeSample(unsigned sample)
*
* Description:  Deallocates a sample and frees it from the Sound Device.
*
* Input:        unsigned sample         sample handle for the sample to be
*                                       deallocated.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING fxFreeSample(unsigned sample);




/****************************************************************************\
*
* Function:     int fxSetAutoChannels(int numChannels,
*                   unsigned *channelNumbers)
*
* Description:  Sets the channel numbers that can be used as automatic effect
*               channels.
*
* Input:        int numChannels             number of channels that can be
*                                           used
*               unsigned *channelNumbers    pointer to a table that contains
*                                           the channels that can be used
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING fxSetAutoChannels(int numChannels, unsigned *channelNumbers);




/****************************************************************************\
*
* Function:     int fxPlaySample(unsigned channel, unsigned sample,
*                   int priority, unsigned rate, unsigned volume, int panning,
*                   unsigned *playHandle)
*
* Description:  Starts playing a sound effect sample on a channel
*
* Input:        unsigned channel        channel number, or fxAutoChannel for
*                                       automatic selection
*               unsigned sample         sample handle
*               int priority            effect priority, the higher the value
*                                       the higher the priority
*               unsigned rate           effect initial sample rate
*               unsigned volume         effect initial volume (0-64)
*               int panning             effect initial panning, see enum
*                                       sdPanning
*               unsigned *playHandle    effect playing handle variable
*
* Returns:      MIDAS error code. The playing handle for the effect will be
*               written to *playHandle.
*
\****************************************************************************/

int CALLING fxPlaySample(unsigned channel, unsigned sample, int priority,
    unsigned rate, unsigned volume, int panning, unsigned *playHandle);




/****************************************************************************\
*
* Function:     int fxStopSample(unsigned playHandle)
*
* Description:  Stops playing a sample
*
* Input:        unsigned playHandle     sample playing handle (from
*                                       fxPlaySample)
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING fxStopSample(unsigned playHandle);



/****************************************************************************\
*
* Function:     int fxSetSampleRate(unsigned playHandle, ulong rate)
*
* Description:  Changes the sample rate for a sample that is being played
*
* Input:        unsigned playHandle     sample playing handle (from
*                                       fxPlaySample)
*               ulong rate              new rate
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING fxSetSampleRate(unsigned playHandle, ulong rate);




/****************************************************************************\
*
* Function:     int fxSetSampleVolume(unsigned playHandle, unsigned volume)
*
* Description:  Changes the volume for a sample that is being played
*
* Input:        unsigned playHandle     sample playing handle (from
*                                       fxPlaySample)
*               unsigned volume         new volume
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING fxSetSampleVolume(unsigned playHandle, unsigned volume);




/****************************************************************************\
*
* Function:     int fxSetSamplePanning(unsigned playHandle, int panning)
*
* Description:  Changes the panning position for a sample that is being played
*
* Input:        unsigned playHandle     sample playing handle (from
*                                       fxPlaySample)
*               int panning             new panning position
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING fxSetSamplePanning(unsigned playHandle, int panning);




/****************************************************************************\
*
* Function:     int fxSetSamplePriority(unsigned playHandle, int priority)
*
* Description:  Changes the priority for a sample that is being played
*
* Input:        unsigned playHandle     sample playing handle (from
*                                       fxPlaySample)
*               int priority            new playing priority
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING fxSetSamplePriority(unsigned playHandle, int priority);



/****************************************************************************\
*
* Function:     int fxGetSamplePlayStatus(unsigned playHandle,
*                   unsigned *status)
*
* Description:  Gets sample playing status (playing or stopped)
*
* Input:        unsigned playHandle     sample playing handle
*               unsigned *status        pointer to status variable
*
* Returns:      MIDAS error code. Sample playing status is written to
*               *state.
*
\****************************************************************************/

int CALLING fxGetSamplePlayStatus(unsigned playHandle, unsigned *status);


#ifdef __cplusplus
}
#endif


#endif      /* #ifdef __midasfx_h */


/*
 * $Log: midasfx.h,v $
 * Revision 1.6  1997/07/31 10:56:49  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.5  1997/07/29 16:51:10  pekangas
 * Added sample playing status query
 *
 * Revision 1.4  1997/05/20 20:37:59  pekangas
 * Added WAVE support to both streams and samples
 *
 * Revision 1.3  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.2  1996/09/28 06:39:22  jpaana
 * Converted from CR/LF and fixed some typos
 *
 * Revision 1.1  1996/09/22 23:17:48  pekangas
 * Initial revision
 *
*/
