/*      mpoll.h
 *
 * Functions for polling MIDAS Digital Audio System in a thread
 *
 * $Id: mpoll.h,v 1.4 1997/07/31 10:56:52 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __mpoll_h
#define __mpoll_h


#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************\
*
* Function:     void PollMIDAS(void)
*
* Description:  Polls MIDAS Digital Audio System
*
\****************************************************************************/

void PollMIDAS(void);



/****************************************************************************\
*
* Function:     void StartPlayThread(unsigned pollPeriod)
*
* Description:  Starts polling MIDAS in a thread
*
* Input:        unsigned pollPeriod     polling period (delay between two
*                                       polling loops) in milliseconds
*
\****************************************************************************/

void StartPlayThread(unsigned pollPeriod);



/****************************************************************************\
*
* Function:     void StopPlayThread(void)
*
* Description:  Stops polling MIDAS in a thread
*
\****************************************************************************/

void StopPlayThread(void);



/****************************************************************************\
*
* Function:     void InitThreads(void)
*
* Description:  Initializes the MIDAS thread polling system and creates the
*               polling thread critical section object.
*
\****************************************************************************/

void InitThreads(void);



/****************************************************************************\
*
* Function:     void LockPollThread(void)
*
* Description:  Locks the poller thread by entering a critical section. This
*               function is used to ensure that only one thread accesses
*               critical MIDAS parts at a time.
*
\****************************************************************************/

void LockPollThread(void);



/****************************************************************************\
*
* Function:     void UnlockPollThread(void)
*
* Description:  Unlocks the polling thread.
*
\****************************************************************************/

void UnlockPollThread(void);



#ifdef __cplusplus
}
#endif

#endif

/*
 * $Log: mpoll.h,v $
 * Revision 1.4  1997/07/31 10:56:52  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.3  1997/05/07 17:14:53  pekangas
 * Added a lot of new thread synchronization code, mainly to minimize the
 * cases where MIDAS state may be modified when the player thread is active
 * and vice versa. Added MIDASlock() and MIDASunlock() to the API.
 *
 * Revision 1.2  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.1  1996/08/06 20:36:45  pekangas
 * Initial revision
 *
*/

