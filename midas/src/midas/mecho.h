/*      mecho.h
 *
 * MIDAS echo effect engine
 *
 * $Id: mecho.h,v 1.3 1997/07/31 10:56:46 pekangas Exp $
 *
 * Copyright 1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __mecho_h
#define __mecho_h



/****************************************************************************\
*       struct mEchoFilterTypes
*       -----------------------
* Description:  Possible echo filter types
\****************************************************************************/

enum mEchoFilterTypes
{
    mEchoFilterNone = 0,
    mEchoFilterLowPass = 1,
    mEchoFilterHighPass = 2
};



/****************************************************************************\
*       struct mEcho
*       ------------
* Description:  A single echo
\****************************************************************************/

typedef struct
{
    unsigned    delay;                  /* milliseconds, 16.16 fixed point */
    int         gain;                   /* gain, 16.16 fixed point */
    int         reverseChannels;        /* reverse channels? */
    unsigned    filterType;             /* filter type, see enum
                                           mEchoFilterTypes */
} mEcho;



/****************************************************************************\
*       struct mEchoSet
*       ---------------
* Description:  An echo set, defines a single echo/reverb effect
\****************************************************************************/

typedef struct
{
    int         feedback;               /* feedback, 16.16 fixed point */
    int         gain;                   /* total gain, 16.16 fixed point */
    unsigned    numEchoes;              /* number of echoes */
    mEcho       *echoes;                /* the echoes */
} mEchoSet;



typedef void* mEchoHandle;  /* an echo set handle */





#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************\
*
* Function:     int mEchoInit(void)
*
* Description:  Initializes the echo effect engine. MUST be called AFTER
*               dsmInit().
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mEchoInit(void);



/****************************************************************************\
*
* Function:     int mEchoClose(void)
*
* Description:  Uninitializes the echo effect engine
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mEchoClose(void);




/****************************************************************************\
*
* Function:     int mEchoAddEffect(mEchoSet *echoSet, mEchoHandle *handle)
*
* Description:  Adds an echo effect to the output, to the beginning of
*               the post-processor chain.
*
* Input:        mEchoSet *echoSet       echo set that describes the effect
*               mEchoHandle *handle     pointer to destination echo handle
*                                       for the effect
*
* Returns:      MIDAS error code. An echo handle for the effect is written
*               to *handle.
*
\****************************************************************************/

int CALLING mEchoAddEffect(mEchoSet *echoSet, mEchoHandle *handle);




/****************************************************************************\
*
* Function:     int mEchoRemoveEffect(mEchoHandle handle)
*
* Description:  Removes an echo effect from the output
*
* Input:        mEchoHandle handle      echo handle for the effect
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mEchoRemoveEffect(mEchoHandle *handle);


   



#ifdef __cplusplus
}
#endif


/****************************************************************************\
*       enum mEchoFunctIDs
*       ------------------
* Description:  ID numbers for echo effect engine functions
\****************************************************************************/

enum mEchoFunctIDs
{
    ID_mEchoInit = ID_echo,
    ID_mEchoClose,
    ID_mEchoAddEffect,
    ID_mEchoRemoveEffect
};



#endif


/*
 * $Log: mecho.h,v $
 * Revision 1.3  1997/07/31 10:56:46  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.2  1997/07/11 11:05:53  pekangas
 * Added new options to the echoes: Each echo now has its own filter setting
 * (low/high-pass or nothing) and the whole echo set has a common gain.
 *
 * Revision 1.1  1997/07/10 18:39:58  pekangas
 * Initial revision
 *
*/
