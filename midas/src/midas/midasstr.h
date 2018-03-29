/*      midasstr.h
 *
 * MIDAS stream library
 *
 * $Id: midasstr.h,v 1.11 1997/07/31 10:56:50 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/


#ifndef __midasstr_h
#define __midasstr_h


/* Hack for Unix: (FIXME) */
#ifdef __UNIX__
#include <pthread.h>
#endif

/****************************************************************************\
*       enum strStreamMode
*       ------------------
* Description:  Stream playing mode
\****************************************************************************/

enum strStreamMode
{
    strStreamPlayFile = 1,
    strStreamCallback,
    strStreamPoll
};


/****************************************************************************\
*       struct strStream
*       ----------------
* Description:  Stream playing structure
\****************************************************************************/

typedef struct _strStream
{
    unsigned    sdChannel;              /* channel number for the stream */
    uchar       *buffer;                /* pointer to stream buffer */
    unsigned    bufferSamples;          /* buffer length in samples */
    unsigned    bufferBytes;            /* buffer length in bytes */
    unsigned    bufferWritePos;         /* buffer write position */
    uchar       *fileBuffer;            /* file reading buffer */
    unsigned    fileBufferBytes;        /* file reading buffer size */
    unsigned    sampleSize;             /* size of one sample in bytes */
    int         streamMode;             /* see enum strStreamMode */
    fileHandle  f;                      /* stream file handle */
    ulong       fileOffset;             /* stream data start offset */
    ulong       fileLength;             /* stream file length in bytes */
    ulong       fileLeft;               /* number of bytes left in file */
    int         loop;                   /* 1 if the stream loops */
    volatile int threadExitFlag;        /* thread exit flag */
    unsigned    threadDelay;            /* delay between polling in thread */
    void (CALLING *callback)(uchar *buffer, struct _strStream *stream);
                                            /* stream playing callback */
#ifdef __UNIX__
    pthread_t   playerThread;           /* player thread */
#endif
    mAdpcmState *adpcmState;            /* ADPCM decoder state */
    mWaveState  *waveState;             /* WAVE reader state */
    uchar       adpcm;                  /* ADPCM usage flag */
    uchar       bufferClearVal;         /* the value that should be used to
                                           clear the buffer */
    uchar       filler[2];
} strStream;



#ifdef __cplusplus
extern "C" {
#endif



/****************************************************************************\
*
* Function:     int strInit(SoundDevice *SD)
*
* Description:  Initializes the stream library
*
* Input:        SoundDevice *SD         Pointer to the Sound Device that will
*                                       be used for playing the streams
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING strInit(SoundDevice *SD);




/****************************************************************************\
*
* Function:     int strClose(void)
*
* Description:  Uninitializes the stream library
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING strClose(void);



#ifdef M_HAVE_THREADS


/****************************************************************************\
*
* Function:     int strPlayStreamFile(unsigned channel, char *fileName,
*                   unsigned sampleType, ulong sampleRate,
*                   unsigned bufferLength, int loop, strStream **stream)
*
* Description:  Starts playing a digital sound stream from a file. Creates a
*               new thread that will take care of reading the file and feeding
*               it to the stream buffer
*
* Input:        unsigned channel        channel number for the stream
*               char *fileName          stream file name
*               unsigned sampleType     stream sample type
*               ulong sampleRate        sampling rate
*               unsigned bufferLength   stream buffer length in milliseconds
*               int loop                1 if the stream should be looped,
*                                       0 if not
*               strStream **stream      pointer to stream state pointer
*
* Returns:      MIDAS error code. Pointer to the stream state structure will
*               be written to *stream
*
\****************************************************************************/

int CALLING strPlayStreamFile(unsigned channel, char *fileName,
    unsigned sampleType, ulong sampleRate, unsigned bufferLength, int loop,
    strStream **stream);



/****************************************************************************\
*
* Function:     int strPlayStreamFile(unsigned channel, char *fileName,
*                   unsigned sampleType, ulong sampleRate,
*                   unsigned bufferLength, int loop, strStream **stream)
*
* Description:  Starts playing a digital sound stream from a RIFF WAVE file.
*               Creates a new thread that will take care of reading the file
*               and feeding it to the stream buffer
*
* Input:        unsigned channel        channel number for the stream
*               char *fileName          stream file name
*               unsigned bufferLength   stream buffer length in milliseconds
*               int loop                1 if the stream should be looped,
*                                       0 if not
*               strStream **stream      pointer to stream state pointer
*
* Returns:      MIDAS error code. Pointer to the stream state structure will
*               be written to *stream
*
\****************************************************************************/

int CALLING strPlayStreamWaveFile(unsigned channel, char *fileName,
                                  unsigned bufferLength, int loop,
                                  strStream **stream);



#endif



/****************************************************************************\
*
* Function:     int strPlayStreamPolling(unsigned channel,
*                   unsigned sampleType, ulong sampleRate,
*                   unsigned bufferLength, strStream **stream)
*
* Description:  Starts playing a stream in polling mode. Use
*               strFeedStreamData() to feed the stream data to the player
*
* Input:        unsigned channel        channel number for the stream
*               unsigned sampleType     stream sample type
*               ulong sampleRate        stream sampling rate
*               unsigned bufferLength   stream buffer length in milliseconds
*               strStream **stream      pointer to stream state pointer
*
* Returns:      MIDAS error code. Pointer to the stream state structure will
*               be written to *stream
*
\****************************************************************************/

int CALLING strPlayStreamPolling(unsigned channel, unsigned sampleType,
    ulong sampleRate, unsigned bufferLength, strStream **stream);



/****************************************************************************\
*
* Function:     int strPlayStreamCallback(unsigned sampleType,
*                  ulong sampleRate, unsigned bufferBytes,
*                  void (CALLING *callback)(uchar *buffer, strStream *stream))
*
* Description:  Starts playing a stream with a callback.
*
* Input:        unsigned sampleType     stream sample type
*               ulong sampleRate        stream sampling rate
*               unsigned bufferBytes    stream buffer size _IN BYTES_
*               ... *callback           stream player callback
*               strStream **stream      pointer to stream state pointer
*
* Returns:      MIDAS error code. Pointer to the stream state structure will
*               be written to *stream
*
* Notes:        The callback function will be called each time the whole
*               stream buffer needs to be filled. It receives as an argument
*               a pointer to the buffer, and the stream state pointer.
*
*               The function will be called from inside the mixing routine,
*               so it should return relatively rapidly - do not use this
*               function for, for example, loading data from disc.
*
\****************************************************************************/

int CALLING strPlayStreamCallback(unsigned sampleType, ulong sampleRate,
    unsigned bufferBytes,
    void (CALLING *callback)(uchar *buffer, strStream *stream));




/****************************************************************************\
*
* Function:     int strStopStream(strStream *stream)
*
* Description:  Stops playing a stream. This function will also destroy the
*               playback thread for stream file playback.
*
* Input:        strStream *stream       stream state pointer for the stream
*
* Returns:      MIDAS error code.
*
\****************************************************************************/

int CALLING strStopStream(strStream *stream);




/****************************************************************************\
*
* Function:     int strFeedStreamData(strStream *stream, uchar *data,
*                   unsigned numBytes, int feedAll, unsigned *numFed)
*
* Description:  Feeds sample data to a stream that is being played in polling
*               mode.
*
* Input:        strStream *stream       stream state pointer from
*                                       strPlayStreamPolling()
*               uchar *data             pointer to stream data
*               unsigned numBytes       number of bytes of data to feed. Note!
*                                       This must be a multiple of the stream
*                                       sample size
*               int feedAll             1 if all data should be fed in all
*                                       circumstances. The function will block
*                                       the current thread if this flag is 1
*                                       until all data is fed.
*               unsigned *numFed        pointer to a variable that will
*                                       contain the number of bytes actually
*                                       fed
*
* Returns:      MIDAS error code. The number of bytes of data actually fed is
*               written to *numFed.
*
\****************************************************************************/

int CALLING strFeedStreamData(strStream *stream, uchar *data,
    unsigned numBytes, int feedAll, unsigned *numFed);




/****************************************************************************\
*
* Function:     int strSetStreamRate(strStream *stream, ulong sampleRate)
*
* Description:  Changes the sampling rate for a stream
*
* Input:        strStream *stream       stream state pointer
*               ulong sampleRate        new sampling rate in Hz
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING strSetStreamRate(strStream *stream, ulong sampleRate);




/****************************************************************************\
*
* Function:     int strSetStreamVolume(strStream *stream, unsigned volume)
*
* Description:  Changes the playback volume for a stream (the default is 64).
*
* Input:        strStream *stream       stream state pointer
*               unsigned volume         new volume (0-64)
*
* Returns:      MIDAS error code.
*
\****************************************************************************/

int CALLING strSetStreamVolume(strStream *stream, unsigned volume);




/****************************************************************************\
*
* Function:     int strSetStreamPanning(strStream *stream, int panning)
*
* Description:  Changes the panning for a stream (the default is middle).
*
* Input:        strStream *stream       stream state pointer
*               int panning             new panning position
*
* Returns:      MIDAS error code.
*
\****************************************************************************/

int CALLING strSetStreamPanning(strStream *stream, int panning);



/****************************************************************************\
*
* Function:     int strGetStreamBytesBuffered(strStream *stream,
*                   unsigned *buffered)
*
* Description:  Get the amount of data currently stored in the stream buffer
*
* Input:        strStream *stream       stream
*               unsigned *buffered      pointer to destination variable
*
* Returns:      MIDAS error code. The amount of data (in bytes)
*               currently buffered is written to *buffered.
*
\****************************************************************************/

int CALLING strGetStreamBytesBuffered(strStream *stream, unsigned *buffered);




/****************************************************************************\
*
* Function:     int strIsStreamFinished(strStream *stream, int *finished)
*
* Description:  Checks whether a given stream has reached the end of the
*               stream file or not. Only applies to streams played from a
*               file.
*
* Input:        strStream *stream       stream state pointer
*               int *finished           pointer to result variable
*
* Returns:      MIDAS error code. If the stream is finished, 1 will be written
*               to *finished, otherwise *finished will contain 0.
*
\****************************************************************************/

int CALLING strIsStreamFinished(strStream *stream, int *finished);




/****************************************************************************\
*
* Function:     int strPauseStream(strStream *stream, int pause)
*
* Description:  Pauses/resumes a stream
*
* Input:        strStream *stream       stream to pause/resume  
*               int pause               1 to pause, 0 to resume
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING strPauseStream(strStream *stream, int pause);





#ifdef __cplusplus
}
#endif


#endif      /* #ifdef __midasstr_h */


/*
 * $Log: midasstr.h,v $
 * Revision 1.11  1997/07/31 10:56:50  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.10  1997/06/02 00:54:15  jpaana
 * Changed most __LINUX__ defines to __UNIX__ for generic *nix porting
 *
 * Revision 1.9  1997/05/20 20:38:00  pekangas
 * Added WAVE support to both streams and samples
 *
 * Revision 1.8  1997/05/03 15:10:51  pekangas
 * Added stream support for DOS, removed GUS Sound Device from non-Lite
 * build. M_HAVE_THREADS now defined in threaded environment.
 *
 * Revision 1.7  1997/04/07 21:07:52  pekangas
 * Added the ability to pause/resume streams.
 * Added functions to query the number of stream bytes buffered
 *
 * Revision 1.6  1997/02/12 16:28:34  pekangas
 * Added support for ADPCM streams
 *
 * Revision 1.5  1997/01/16 19:31:53  pekangas
 * Fixed to compile with Linux GCC (but do they work?)
 *
 * Revision 1.4  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.3  1997/01/16 18:25:08  pekangas
 * Implemented strSetStreamRate, strSetStreamVolume and strSetStreamPanning
 *
 * Revision 1.2  1996/09/28 06:39:46  jpaana
 * Converted from CR/LF and fixed some typos
 *
 * Revision 1.1  1996/09/22 23:17:48  pekangas
 * Initial revision
 *
*/








