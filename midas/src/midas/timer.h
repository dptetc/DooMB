/*      timer.h
 *
 * TempoTimer
 *
 * $Id: timer.h,v 1.5 1997/07/31 10:56:56 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/


#ifndef __TIMER_H
#define __TIMER_H

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************\
*
* Function:     int tmrGetScreenRefreshRate(unsigned *refreshRate)
*
* Description:  Tries to determine the screen refresh rate for the current
*               display mode. The refresh rate can be used with
*               tmrSetUserCallbacks() to synchronize the user callbacks to
*               screen refresh.
*
* Input:        unsigned *refreshRate   pointer to target refresh rate
*                                       variable.
*
* Returns:      MIDAS error code. Screen refresh rate is written to
*               *refreshRate. If the refresh rate could not be determined
*               successfully (for example, running under Win95), *refreshRate
*               is set to 0.
*
*               The refresh rate is returned in milliHertz, ie. 1000*Hz.
*               70Hz refresh rate becomes 70000 etc.
*
\****************************************************************************/

int CALLING tmrGetScreenRefreshRate(unsigned *refreshRate);




/****************************************************************************\
*
* Function:     int tmrInit(void);
*
* Description:  Initializes TempoTimer.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING tmrInit(void);




/****************************************************************************\
*
* Function:     int tmrClose(void);
*
* Description:  Uninitializes TempoTimer. MUST be called if and ONLY if
*               tmrInit() has been called.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING tmrClose(void);




/****************************************************************************\
*
* Function:     int tmrPlaySD(SoundDevice *SD);
*
* Description:  Starts playing sound with a Sound Device ie. calling its
*               Play() function in the update rate, which is set to
*               50Hz.
*
* Input:        SoundDevice *SD         Sound Device that will be used
*
* Returns:      MIDAS error code.
*
\****************************************************************************/

int CALLING tmrPlaySD(SoundDevice *SD);




/****************************************************************************\
*
* Function:     int tmrStopSD(void);
*
* Description:  Stops playing sound with the Sound Device.
*
* Returns:      MIDAS error code.
*
\****************************************************************************/

int CALLING tmrStopSD(void);





/****************************************************************************\
*
* Function:     int tmrPlayMusic(void *play, int *playerNum);
*
* Description:  Starts playing music with the timer.
*
* Input:        void *play              Pointer to music playing function,
*                                       must return MIDAS error codes
*               int *playerNum          Pointer to player number, used
*                                       for stopping music
*
* Returns:      MIDAS error code. Player number is written to *playerNum.
*
* Notes:        There can be a maximum of 16 music players active at the
*               same time.
*
\****************************************************************************/

int CALLING tmrPlayMusic(int (CALLING *play)(), int *playerNum);




/****************************************************************************\
*
* Function:     int tmrStopMusic(int playerNum);
*
* Description:  Stops playing music with the timer.
*
* Input:        int playerNum           Number of player to be stopped.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING tmrStopMusic(int playerNum);




/****************************************************************************\
*
* Function:     int tmrSetUserCallbacks(unsigned rate, int syncScreen, void (*preVR)(), void (*immVR)(), void (*inVR)())
*
* Description:  Sets timer user callback functions and the callback rate
*
* Input:        unsigned rate           callback timer rate, in milliHertz
*                                       (1000 * Hz, 50Hz = 50000 etc)
*               int syncScreen          1 if the callbacks should be
*                                       synchronized to screen refresh, 0 if
*                                       not.
*               void (*preVR)()         preVR callback function, called
*                                       immediately BEFORE Vertical Retrace
*                                       if syncScreen is 1
*               void (*immVR)()         immVR callback function, called
*                                       immediately AFTER VR start if
*                                       syncScreen is 1
*               void (*inVR)()          inVR callback function, called later
*                                       DURING VR if syncScreen is 1.
*
* Returns:      MIDAS error code.
*
* Notes:        If any of preVR, immVR or inVR is NULL, that callback is
*               simply ignored. If syncScreen is 1, the user callback timer
*               is synchronized to screen refresh, if possible. In that case,
*               "rate" MUST be the refresh rate for that display mode,
*               returned by tmrGetScreenRefreshRate(). If syncScreen is 0,
*               the callback functions are simply called one after another.
*
*               In any case, the preVR() and immVR() callback functions must
*               be as short as possible, to avoid timing problems. They should
*               not do more than just set a few hardware register and update
*               some counter. inVR() can take somewhat longer time, and can
*               be used, for example, for setting the VGA palette. However,
*               to avoid possible music tempo problems, it should never take
*               more than about one fourth of the total time between two
*               interrupts.
*
*               The callback rate should be between 50Hz and 200Hz.
*
\****************************************************************************/

int CALLING tmrSetUserCallbacks(unsigned rate, int syncScreen,
				void (CALLING *_preVR)(),
				void (CALLING *_immVR)(),
				void (CALLING *_inVR)());




/****************************************************************************\
*
* Function:     int tmrRemoveUserCallbacks(void)
*
* Description:  Removes user timer callbacks and stops the user timer
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING tmrRemoveUserCallbacks(void);




/****************************************************************************\
*
* Function:     int tmrSetUpdRate(unsigned updRate);
*
* Description:  Sets the timer update rate, ie. the rate at which the music
*               playing routines are called
*
* Input:        unsigned updRate        updating rate, in 100*Hz (5000=50Hz)
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING tmrSetUpdRate(unsigned updRate);


#ifdef __cplusplus
}
#endif


/****************************************************************************\
*       enum tmrFunctIDs
*       ----------------
* Description:  ID numbers for TempoTimer functions
\****************************************************************************/

enum tmrFunctIDs
{
    ID_tmrGetScrSync = ID_tmr,
    ID_tmrInit,
    ID_tmrClose,
    ID_tmrPlaySD,
    ID_tmrStopSD,
    ID_tmrPlayMusic,
    ID_tmrStopMusic,
    ID_tmrSyncScr,
    ID_tmrStopScrSync,
    ID_tmrSetUpdRate
};



#endif


/*
 * $Log: timer.h,v $
 * Revision 1.5  1997/07/31 10:56:56  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.4  1997/03/05 16:49:13  pekangas
 * Renamed functions due to dostimer.c rewrite
 *
 * Revision 1.3  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.2  1996/07/13 18:09:09  pekangas
 * Fixed to compile with Visual C
 *
 * Revision 1.1  1996/05/22 20:49:33  pekangas
 * Initial revision
 *
*/
