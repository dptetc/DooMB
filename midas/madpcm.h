/*      madpcm.h
 *
 * IMA ADPCM coding/decoding routines
 *
 * $Id: madpcm.h,v 1.4.2.1 1997/08/21 18:02:50 pekangas Exp $
 *
 * Copyright 1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __madpcm_h
#define __madpcm_h


/****************************************************************************\
*       struct mStreamFrameHeader
*       -------------------------
* Description:  MIDAS stream frame header
\****************************************************************************/

typedef struct
{
    U8          magic17, magic42;       /* 0x17, 0x42 */
    U16         frameLength;            /* frame length, including header */
    U8          sum;                    /* sum mod 256 of entire header */
    U8          sampleType;             /* sample type for stream */
    /* sample type specific data - ADPCM predictor values for each channel
       etc */
} mStreamFrameHeader;




/****************************************************************************\
*       struct mAdpcmFrameChannelHeader
*       -------------------------------
* Description:  ADPCM-specific stream frame header data for one channel
\****************************************************************************/

typedef struct
{
    S16         predValue;              /* current predictor value */
    S8          stepSizeIndex;          /* index to step size table */
} mAdpcmFrameChannelHeader;




/****************************************************************************\
*       struct mAdpcmChannelState
*       -------------------------
* Description:  ADPCM coder/decoder channel state
\****************************************************************************/

typedef struct
{
    int         predValue;              /* current predictor value */
    int         stepSizeIndex;          /* index to step size table */
} mAdpcmChannelState;




/****************************************************************************\
*       struct mAdpcmState
*       ------------------
* Description:  ADPCM coder/decoder state
\****************************************************************************/

typedef struct
{
    unsigned    numChannels;            /* number of channels used */
    mStreamFrameHeader *header;         /* stream frame header */
    mAdpcmFrameChannelHeader *chanHeaders;  /* channel headers */
    U8          *headerPtr;             /* header write pointer */
    unsigned    headerSize;             /* frame header total size */
    unsigned    headerBytes;            /* current number of header bytes */
    unsigned    frameBytes;             /* number of frame bytes left */
    unsigned    frameLength;            /* total frame length (for encoding)*/
    mAdpcmChannelState channels[EMPTYARRAY];
} mAdpcmState;




#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************\
*
* Function:     int mAdpcmInitState(unsigned numChannels,
*                   unsigned frameLength, mAdpcmState **state)
*
* Description:  Allocates and initializes the ADPCM state structure.
*
* Input:        unsigned numChannels    number of channels that will be used
*               unsigned frameLength    length of one encoded stream frame,
*                                       _including header_, in bytes. Only
*                                       used for encoding.
*               mAdpcmState **state     pointer to destination state pointer
*
* Returns:      MIDAS error code. The initialized ADPCM state structure will
*               be written to *state.
*
\****************************************************************************/

int CALLING mAdpcmInitState(unsigned numChannels, unsigned frameLength,
    mAdpcmState **state);




/****************************************************************************\
*
* Function:     int mAdpcmFreeState(mAdpcmState *state)
*
* Description:  Deallocates an ADPCM state structure
*
* Input:        mAdpcmState *state      the state structure
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mAdpcmFreeState(mAdpcmState *state);




/****************************************************************************\
*
* Function:     int mAdpcmEncode(short *input, unsigned inputLength,
*                   uchar *output, unsigned outputBufferLength,
*                   unsigned *inputUsed, unsigned *outputWritten,
*                   mAdpcmState *state)
*
* Description:  Encodes raw 16-bit signed audio data to 4-bit ADPCM
*
* Input:        short *input            pointer to input data
*               unsigned inputLength    length of input data, in bytes. MUST
*                                       contain an even number of samples
*                                       unless encoding the final block of a
*                                       stream.
*               uchar *output           pointer to destination data
*               unsigned outputBufferLength  output buffer length, in bytes
*               unsigned *inputUsed     pointer to the variable that will get
*                                       the number of input bytes used
*               unsigned *outputWritten pointer to the variable that will get
*                                       the number of output bytes written
*               mAdpcmState *state      ADPCM coder state
*
* Returns:      MIDAS error code. Coded data is written to *output, the number
*               of input bytes used is written to *inputUsed, and the number
*               of output bytes written is written to *outputWritten.
*
\****************************************************************************/

int CALLING mAdpcmEncode(short *input, unsigned inputLength, uchar *output,
    unsigned outputBufferLength, unsigned *inputUsed, unsigned *outputWritten,
    mAdpcmState *state);




/****************************************************************************\
*
* Function:     int mAdpcmDecode(uchar *input, unsigned inputLength,
*                   short *output, unsigned outputBufferLength,
*                   unsigned *inputUsed, unsigned *outputWritten,
*                   mAdpcmState *state)
*
* Description:  Encodes 4-bit ADPCM audio data into raw 16-bit signed data
*
* Input:        uchar *input            pointer to input data
*               unsigned inputLength    length of input data, in bytes
*               ushort*output           pointer to destination data
*               unsigned outputBufferLength  output buffer length, in bytes.
*               unsigned *inputUsed     pointer to the variable that will get
*                                       the number of input bytes used
*               unsigned *outputWritten pointer to the variable that will get
*                                       the number of output bytes written
*               mAdpcmState *state      ADPCM coder state
*
* Returns:      MIDAS error code. Decoded data is written to *output,
*               the number of input bytes used is written to *inputUsed, and
*               the number of output bytes written is written to
*               *outputWritten.
*
* Notes:        Decoding will be done for numChannels (mAdpcmInitState)
*               samples at a time, so both the input and output buffer must
*               contain data for an integral number of n-channel samples.
*               In addition, the output buffer must have space for an even
*               number of samples for mono streams.
*
\****************************************************************************/

int CALLING mAdpcmDecode(uchar *input, unsigned inputLength, short *output,
    unsigned outputBufferLength, unsigned *inputUsed, unsigned *outputWritten,
    mAdpcmState *state);






#ifdef __cplusplus
}
#endif


/****************************************************************************\
*       enum mAdpcmFunctIDs
*       -------------------
* Description:  ID numbers for ADPCM function
\****************************************************************************/

enum mAdpcmFunctIDs
{
    ID_mAdpcmInitState = ID_adpcm,
    ID_mAdpcmFreeState,
    ID_mAdpcmEncode,
    ID_mAdpcmDecode
};



/****************************************************************************\
*       enum mAdpcmProfIDs
*       ------------------
* Description:  Profiling IDs for ADPCM functions
\****************************************************************************/

#ifdef __mprof_h

enum mAdpcmProfIDs
{
    M_ADPCM_PROF_ENCODE = M_PROF_ADPCM,
    M_ADPCM_PROF_DECODE
};

#endif


#endif


/*
 * $Log: madpcm.h,v $
 * Revision 1.4.2.1  1997/08/21 18:02:50  pekangas
 * Greatly optimized the ADPCM decoding functions
 *
 * Revision 1.4  1997/07/31 10:56:45  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.3  1997/05/23 15:05:10  jpaana
 * Fixed a warning with -pedantic
 *
 * Revision 1.2  1997/02/18 20:21:35  pekangas
 * New ADPCM encoding/decoding code: ADPCM streams now consist of frames,
 * and playback can be started at the beginning of any frame.
 *
 * Revision 1.1  1997/02/11 18:14:21  pekangas
 * Initial revision
 *
*/
