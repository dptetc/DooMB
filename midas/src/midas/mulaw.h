/*      mulaw.h
 *
 * u-law encoding/decoding routines
 *
 * $Id: mulaw.h,v 1.2 1997/07/31 10:56:53 pekangas Exp $
 *
 * Copyright 1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __mulaw_h
#define __mulaw_h


#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************\
*
* Function:     int mUlawEncode(short *input, unsigned inputLength,
*                   uchar *output)
*
* Description:  Encodes raw 16-bit signed sample data to 8-bit u-law.
*
* Input:        short *input            pointer to input data
*               unsigned inputLenth     length of input data, in bytes
*               uchar *output           pointer to output buffer, length
*                                       inputLength/2.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mUlawEncode(short *input, unsigned inputLength, uchar *output);




/****************************************************************************\
*
* Function:     int mUlawDecode(uchar *input, unsigned inputLength,
*                   short *output)
*
* Description:  Decodes 8-bit u-law data to 16-bit signed raw samples
*
* Input:        uchar *input            pointer to input data
*               unsigned inputLength    amount of input data in bytes
*               short *output           pointer to output buffer, size
*                                       2*inputLength
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mUlawDecode(uchar *input, unsigned inputLength, short *output);


#ifdef __cplusplus
}
#endif



#endif


/*
 * $Log: mulaw.h,v $
 * Revision 1.2  1997/07/31 10:56:53  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.1  1997/02/20 19:48:22  pekangas
 * Initial revision
 *
*/