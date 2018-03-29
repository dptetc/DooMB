/*      waveread.h
 *
 * RIFF WAVE reader routines
 *
 * $Id: waveread.h,v 1.2 1997/07/31 10:56:57 pekangas Exp $
 *
 * Copyright 1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __waveread_h
#define __waveread_h


/****************************************************************************\
*       struct mWaveState
*       -----------------
* Description:  RIFF WAVE reader state structure
\****************************************************************************/

typedef struct
{
    fileHandle file;
    unsigned sampleType;                /* Sound Device sample type */
    unsigned sampleRate;
    unsigned dataLength;                /* sample data length, in bytes */
    unsigned dataLeft;                  /* number of sample data bytes left */
    unsigned dataStart;                 /* sample data start offset */
} mWaveState;



enum mWaveFunctIDs
{
    ID_mWaveOpen = ID_wave,
    ID_mWaveClose,
    ID_mWaveFindChunk
};


typedef struct
{
    char riff[4];                       /* "RIFF" */
    U32 length;
} mWaveRiffChunk;


typedef struct
{
    char name[4];
    U32 length;
} mWaveChunkHeader;


typedef struct
{
    U16 formatTag;
    U16 numChannels;
    U32 samplesPerSec;
    U32 avgBytesPerSec;
    U16 blockSize;
} mWaveFmtData;


typedef struct
{
    U16 bitsPerSample;
} mWavePcmFmtData;



#ifdef __cplusplus
extern "C" {
#endif    


/****************************************************************************\
*
* Function:     int mWaveFindChunk(mFileHandle f, const char *name,
*                   unsigned *length)
*
* Description:  Finds the next chunk with a given name from a RIFF WAVE file,
*               and reads its length.
*
* Input:        mFileHandle f           file handle for the file   
*               const char *name        chunk name to look for
*               unsigned *length        pointer to chunk length destination
*
* Returns:      MIDAS error code. Chunk length in bytes (excluding the name
*               and length) is written to *length. 
*
\****************************************************************************/

int CALLING mWaveFindChunk(fileHandle f, const char *name, unsigned *length);



/****************************************************************************\
*
* Function:     int mWaveOpen(char *fileName, mWaveState **state)
*
* Description:  Opens a RIFF WAVE file for reading, reads its format
*               information, finds the start and length of sample data,
*               and sets the file reading position to the start of the
*               sample data.
*
* Input:        char *fileName          name of file
*               mWaveState **state      pointer to destination state pointer
*
* Returns:      MIDAS error code. Pointer to a WAVE reader state structure
*               is written to *state.
*
\****************************************************************************/

int CALLING mWaveOpen(char *fileName, mWaveState **state);



/****************************************************************************\
*
* Function:     int mWaveClose(mWaveState *state)
*
* Description:  Closes a RIFF WAVE file opened with mWaveOpen() and
*               deallocates its state structure.
*
* Input:        mWaveState *state       the wave file state structure
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mWaveClose(mWaveState *state);


#ifdef __cplusplus
}
#endif


#endif


/*
 * $Log: waveread.h,v $
 * Revision 1.2  1997/07/31 10:56:57  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.1  1997/05/20 20:35:29  pekangas
 * Initial revision
 *
*/
