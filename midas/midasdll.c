/*      midasdll.c
 *
 * MIDAS DLL programming interface
 *
 * $Id: midasdll.c,v 1.42.2.4 1998/01/21 21:57:25 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

/*#define DEBUGMESSAGES*/

#if defined(__NT__) || defined(__WINDOWS__) || defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef DEBUGMESSAGES
#include <stdio.h>
#endif

#include "midas.h"
#include "dsm.h"
#include "mecho.h"
#include "mprof.h"

/* This is a kluge, but necessary as Watcom C sucks: */
#if defined(M_DLL)
#define EXPORT_IN_MIDASDLL_H
#endif

#include "midasdll.h"


RCSID(const char *midasdll_rcsid = "$Id: midasdll.c,v 1.42.2.4 1998/01/21 21:57:25 pekangas Exp $";)


#ifdef DEBUGMESSAGES
#  define DEBUGMSG(x) x
#else
#  define DEBUGMSG(x)
#endif

int             mLastError;
static int      MIDASthread = 0;
#ifdef __WIN32__
static volatile int DLLinUse = 0;
#endif
static int      midasFxInit, midasStrInit;
static unsigned amplification;
static unsigned channelAmplification;
static int      *chanAllocated = NULL;
static int      playModuleOpenChannels;   /* did MIDASplayModule open? */

static int      timerMusicCount = 0;
static int      forceNoSound = 0;

static unsigned *autoEffectChannels = NULL;
static unsigned numAutoEffectChannels = 0;
static unsigned numEchoEffects = 0;


#define MAXPLAYHANDLES 16

typedef struct _ModulePlayHandle
{
    unsigned     numChannels;
    gmpPlayHandle playHandle;
    unsigned     channels[EMPTYARRAY];
} ModulePlayHandle;


ModulePlayHandle *mPlayHandles[MAXPLAYHANDLES];


#ifdef M_HAVE_THREADS
#  define M_LOCK LockPollThread();
#  define M_UNLOCK UnlockPollThread();
#else
#  define M_LOCK {}
#  define M_UNLOCK {}
#endif


_FUNC(int) MIDASgetLastError(void)
{
    return mLastError;
}


_FUNC(char*) MIDASgetErrorMessage(int errorCode)
{
    return errorMsg[errorCode];
}



#ifdef __DOS__


/****************************************************************************\
*
* Function:     DWORD MIDASgetDisplayRefreshRate(void)
*
* Description:  Gets the current display refresh rate
*
* Returns:      Returns the current display refresh rate in milliHertz
*               (ie. 1000*hz, 70Hz becomes 70000 etc.), or 0 if the refresh
*               rate couldn't be determined (running under Windows)
*
\****************************************************************************/

_FUNC(DWORD) MIDASgetDisplayRefreshRate(void)
{
    static unsigned rate;
    int error;

    if ( (error = tmrGetScreenRefreshRate(&rate)) != OK )
    {
	mLastError = error;
	return 0;
    }

    mLastError = OK;
    return rate;
}



#endif /* #ifdef __DOS__ */




/****************************************************************************\
*
* Function:     BOOL MIDASstartup(void)
*
* Description:  Sets all configuration variables to default values and
*               prepares MIDAS for use. This function must be called before
*               ANY other MIDAS function, including MIDASinit and
*               MIDASsetOption. After this function has been called,
*               MIDASclose can be called at any point, regarless of whether
*               MIDAS has been initialized or not.
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASstartup(void)
{
    unsigned i;
    
    MIDASthread = 0;
    midasFxInit = 0;
    midasStrInit = 0;
    amplification = 100;
    channelAmplification = 0;
    midasSetDefaults();
    chanAllocated = NULL;
    playModuleOpenChannels = 0;
    timerMusicCount = 0;
    autoEffectChannels = NULL;
    numAutoEffectChannels = 0;
    numEchoEffects = 0;
    forceNoSound = 0;

    for ( i = 0; i < MAXPLAYHANDLES; i++ )
        mPlayHandles[i] = NULL;

    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASdetectSD(void)
*
* Description:  Attempts to detect a Sound Device. Sets the global variable
*               midasSD to point to the detected Sound Device or NULL if no
*               Sound Device was detected
*
* Returns:      TRUE if detected, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASdetectSD(void)
{
    int         dsd;
    int         error;
    static int  dResult;

    midasSD = NULL;                     /* no Sound Device detected yet */
    midasSDNumber = -1;
    dsd = 0;                            /* start from first Sound Device */

    /* search through Sound Devices until a Sound Device is detected: */
    while ( (midasSD == NULL) && (dsd < NUMSDEVICES) )
    {
        /* attempt to detect current SD: */
        if ( (error = (*midasSoundDevices[dsd]->Detect)(&dResult)) != OK )
        {
            mLastError = error;
            return FALSE;
        }
        if ( dResult == 1 )
        {
            midasSDNumber = dsd;        /* Sound Device detected */
            /* point midasSD to this Sound Device: */
            midasSD = midasSoundDevices[dsd];
        }
        dsd++;                          /* try next Sound Device */
    }

    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASdetectSoundCard(void)
*
* Description:  Attempts to autodetect the sound card used
*
* Returns:      TRUE if a sound card was detected, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASdetectSoundCard(void)
{
    if ( !MIDASdetectSD() )
        return FALSE;

    /* Did we detect No Sound? */
    if ( midasSDNumber == 5 )
        return FALSE;

    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL midasInit(void);
*
* Description:  Initializes MIDAS Digital Audio System
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASinit(void)
{
    int         error;
    static int  dResult;

    midasEMSInit = 0;
    mUseEMS = 0;

#ifdef M_PROF
    mProfInit();
#endif

#ifdef M_HAVE_THREADS    
    InitThreads();
#endif

    if ( forceNoSound )
    {
#ifdef __WIN32__
#  ifdef NODSOUND
        midasSDNumber = 1;
#  else
        midasSDNumber = 2;
#  endif
#else
#  ifdef __UNIX__
        midasSDNumber = 1;
#  else
#    ifdef M_LITE
        midasSDNumber = 5;
#    else
        midasSDNumber = 4;
#    endif
#  endif
#endif
    }

    if ( midasSDNumber == -1 )      /* has a Sound Device been selected? */
    {
        /* attempt to detect Sound Device */
        if ( !MIDASdetectSD() )
        {
            return FALSE;
        }

        if ( midasSD == NULL )
        {
            mLastError = errSDFailure;
            return FALSE;
        }
    }
    else
    {
        /* use selected Sound Device: */
        midasSD = midasSoundDevices[midasSDNumber];

        /* Sound Device number was forced, but if no I/O port, IRQ, DMA or
           sound card type has been set, try to autodetect the values for this
           Sound Device. If detection fails, use default values: */

        if ( (midasSDPort == -1) && (midasSDIRQ == -1) &&
            (midasSDDMA == -1) && (midasSDCard == -1) )
            if ( (error = midasSD->Detect(&dResult)) != OK )
            {
                mLastError = error;
                return FALSE;
            }
    }

    if ( midasSDPort != -1 )            /* has an I/O port been selected? */
        midasSD->port = midasSDPort;    /* if yes, set it to Sound Device */
    if ( midasSDIRQ != -1 )             /* SD IRQ number? */
        midasSD->IRQ = midasSDIRQ;      /* if yes, set it to Sound Device */
    if ( midasSDDMA != -1 )             /* SD DMA channel number? */
        midasSD->DMA = midasSDDMA;
    if ( midasSDCard != -1 )            /* sound card type? */
        midasSD->cardType = midasSDCard;

#if defined (__DOS__) && (!defined(NOTIMER))
    /* initialize TempoTimer: */
    if ( (error = tmrInit()) != OK )
    {
        mLastError = error;
        return FALSE;
    }

    midasTMRInit = 1;                 /* TempoTimer initialized */
#endif

    /* initialize Sound Device: */
    if ( (error = midasSD->Init(midasMixRate, midasOutputMode)) != OK )
    {
        mLastError = error;
        return FALSE;
    }

    midasSDInit = 1;                  /* Sound Device initialized */


#if defined (__DOS__) && (!defined(NOTIMER))
    /* start playing sound using the timer: */
    if ( (error = tmrPlaySD(midasSD)) != OK )
    {
        mLastError = error;
        return FALSE;
    }
    midasTMRPlay = 1;
#endif

    /* Initialize Generic Module Player: */
    if ( (error = gmpInit(midasSD)) != OK )
    {
        mLastError = error;
        return FALSE;
    }
    midasGMPInit = 1;

    /* Initialize Sound Effects library: */
    if ( (error = fxInit(midasSD)) != OK )
    {
        mLastError = error;
        return FALSE;
    }
    midasFxInit = 1;

#ifdef SUPPORTSTREAMS
    /* Initialize stream player library: */
    if ( (error = strInit(midasSD)) != OK )
    {
        mLastError = error;
        return FALSE;
    }
    midasStrInit = 1;
#endif

    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASclose(void)
*
* Description:  Uninitializes MIDAS Digital Audio System
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASclose(void)
{
    unsigned    i;
    int         error;

    DEBUGMSG(puts("MIDASclose: MIDASstopBackgroundPlay"));

#if defined(__WIN32__) || defined(__UNIX__)
    /* Stop playing thread: */
    if ( MIDASthread )
        MIDASstopBackgroundPlay();
#endif

    DEBUGMSG(puts("MIDASclose: mEchoClose"));

    /* Remove any echo effects left and uninitialize echo effect engine: */
    if ( numEchoEffects > 0 )
    {
        if ( (error = mEchoClose()) != OK )
        {
            mLastError = error;
            return FALSE;
        }
    }

    DEBUGMSG(puts("MIDASclose: MIDASfreeAutoEffectChannels"));

    /* Remove any automatic sound effect channels: */
    if ( numAutoEffectChannels > 0 )
    {
        if ( !MIDASfreeAutoEffectChannels() )
            return FALSE;
    }

#if defined (__DOS__) && (!defined(NOTIMER))
    /* if music is being played with timer, stop it: */
    if ( midasTMRMusic )
    {
        if ( (error = gmpSetUpdRateFunct(NULL)) != OK )
        {
            mLastError = error;
            return FALSE;
        }
        if ( (error = tmrStopMusic(midasPlayerNum)) != OK )
        {
            mLastError = error;
            return FALSE;
        }
        midasTMRMusic = 0;
    }
#endif

    DEBUGMSG(puts("MIDASclose: strClose"));
    
#ifdef SUPPORTSTREAMS
    /* Uninitialize stream player if initialized: */
    if ( midasStrInit )
    {
        if ( (error = strClose()) != OK )
        {
            mLastError = error;
            return FALSE;
        }
        midasStrInit = 0;
    }
#endif

    DEBUGMSG(puts("MIDASclose: fxClose"));
    
    /* Uninitialize sound effect library if initialized: */
    if ( midasFxInit )
    {
        if ( (error = fxClose()) != OK )
        {
            mLastError = error;
            return FALSE;
        }
        midasFxInit = 0;
    }

    DEBUGMSG(puts("MIDASclose: MIDASstopModule()s"));
    
    /* Stop any modules that are being played: */
    for ( i = 0; i < MAXPLAYHANDLES; i++ )
    {
        if ( mPlayHandles[i] != NULL )
        {
            if ( !MIDASstopModule(i+1) )
                return FALSE;
        }
    }
    
    DEBUGMSG(puts("MIDASclose: gmpClose"));

    /* If Generic Module Player has been initialized, uninitialize it: */
    if ( midasGMPInit )
    {
        if ( (error = gmpClose()) != OK )
        {
            mLastError = error;
            return FALSE;
        }
        midasGMPInit = 0;
    }

    DEBUGMSG(puts("MIDASclose: midasSD->CloseChannels"));
    
    /* if Sound Device channels are open, close them: */
    if ( midasSDChans )
    {
        if ( (error = midasSD->CloseChannels()) != OK )
        {
            mLastError = error;
            return FALSE;
        }
        midasSDChans = 0;
        midasChannels = 0;
    }

#if defined (__DOS__) && (!defined(NOTIMER))
    /* if sound is being played, stop it: */
    if ( midasTMRPlay )
    {
        if ( (error = tmrStopSD()) != OK )
        {
            mLastError = error;
            return FALSE;
        }
        midasTMRPlay = 0;
    }
#endif

    DEBUGMSG(puts("MIDASclose: midasSD->Close"));
    
    /* if Sound Device is initialized, uninitialize it: */
    if ( midasSDInit )
    {
        if ( (error = midasSD->Close()) != OK )
        {
            mLastError = error;
            return FALSE;
        }
        midasSDInit = 0;
        midasSD = NULL;
    }

#if defined (__DOS__) && (!defined(NOTIMER))
    /* if TempoTimer is initialized, uninitialize it: */
    if ( midasTMRInit )
    {
        if ( (error = tmrClose()) != OK )
        {
            mLastError = error;
            return FALSE;
        }
        midasTMRInit = 0;
    }
#endif

    DEBUGMSG(puts("MIDASclose: mProfClose"));
    
#ifdef M_PROF
    mProfClose();
#endif

    DEBUGMSG(puts("MIDASclose: Done."));    

    return TRUE;
}



#ifdef M_SD_HAVE_SUSPEND
/****************************************************************************\
*
* Function:     BOOL MIDASsuspend(void)
*
* Description:  Suspends all MIDAS playback and releases the output device
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsuspend(void)
{
    int         error;

    M_LOCK

    if ( (error = midasSD->Suspend()) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASresume(void)
*
* Description:  Resumes MIDAS playback after MIDASsuspend()
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASresume(void)
{
    int         error;

    M_LOCK

    if ( (error = midasSD->Resume()) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}

#endif


/****************************************************************************\
*
* Function:     char* MIDASgetVersionString(void)
*
* Description:  Queries the MIDAS version
*
* Returns:      String containing MIDAS version number
*
\****************************************************************************/

_FUNC(char*) MIDASgetVersionString(void)
{
    return MVERSTR;
}




#ifdef __DOS__



/****************************************************************************\
*
* Function:     BOOL MIDASsetTimerCallbacks(DWORD rate, BOOL displaySync,
*                   void MIDAS_CALL (*preVR)(), void MIDAS_CALL (*immVR)(),
*                   void MIDAS_CALL (*inVR)())
*
* Description:  Sets timer user callback
*
* Input:        DWORD rate              timer rate
*               BOOL displaySync        synchronize to screen refresh?
*               void MIDAS_CALL (*preVR)()  preVR callback or NULL
*               void MIDAS_CALL (*immVR)()  immVR callback or NULL
*               void MIDAS_CALL (*inVR)()   inVR callback or NULL
*
* Returns:      TRUE if successful, FALSE if not.
*
* Notes:        See tmrSetUserCallbacks() description in dostimer.c for more
*               information
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetTimerCallbacks(DWORD rate, BOOL displaySync,
				   void MIDAS_CALL (*preVR)(),
				   void MIDAS_CALL (*immVR)(),
				   void MIDAS_CALL (*inVR)())
{
    int error;

    if ( (error = tmrSetUserCallbacks(rate, (int) displaySync, preVR, immVR,
				      inVR)) != OK )
    {
	mLastError = error;
	return FALSE;
    }

    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASremoveTimerCallbacks(void)
*
* Description:  Removes timer user callbacks
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASremoveTimerCallbacks(void)
{
    int error;

    if ( (error = tmrRemoveUserCallbacks()) != OK )
    {
	mLastError = error;
	return FALSE;
    }

    return TRUE;
}



#endif /* #ifdef __DOS__ */


/****************************************************************************\
*
* Function:     BOOL MIDASopenChannels(int numChans);
*
* Description:  Opens Sound Device channels for sound and music output.
*
* Input:        int numChans            Number of channels to open
*
* Notes:        Channels opened with this function can be used for sound
*               playing, and modules played with midasPlayModule() will be
*               played through the last of these channels. This function is
*               provided so that the same number of channels can be open
*               the whole time throughout the execution of the program,
*               keeping the volume level constant. Note that you must ensure
*               that you open enough channels for all modules, otherwise
*               midasPlayModule() will fail.
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASopenChannels(int numChans)
{
    int         error;
    int         i;
    static unsigned amp;

    M_LOCK

    midasChannels = numChans;

    /* open Sound Device channels: */
    if ( (error = midasSD->OpenChannels(numChans)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }
    midasSDChans = 1;

    /* Allocate channel allocation table and mark all channels free: */
    if ( (error = memAlloc(midasChannels * sizeof(int), (void**)
                           &chanAllocated)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }
    
    for ( i = 0; i < midasChannels; i++ )
        chanAllocated[i] = 0;

    /* set amplification level if forced: */
    if ( midasAmplification != -1 )
    {
        if ( (error = midasSD->SetAmplification(midasAmplification)) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return FALSE;
        }
    }

    /* Get channel default amplification: */
    if ( (error = midasSD->GetAmplification(&channelAmplification)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    /* Set the user amplification level if not 100: */
    if ( amplification != 100 )
    {
        amp = (channelAmplification * amplification) / 100;

        if ( (error = midasSD->SetAmplification(amp)) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return FALSE;
        }
    }

    M_UNLOCK
    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDAScloseChannels(void);
*
* Description:  Closes Sound Device channels opened with midasOpenChannels().
*               Do NOT call this function unless you have opened the sound
*               channels used yourself with midasOpenChannels().
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDAScloseChannels(void)
{
    int         error;

    M_LOCK

    /* Close Sound Device channels: */
    if ( (error = midasSD->CloseChannels()) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }
    midasSDChans = 0;
    midasChannels = 0;

    /* Deallocate channel allocation table: */
    if ( (error = memFree(chanAllocated)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }
    chanAllocated = NULL;

    M_UNLOCK
    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASsetAmplification(DWORD amplification)
*
* Description:  Sets output amplification level. 100 is no amplification,
*               200 double volume, 50 half volume etc.
*
* Input:        DWORD amplification     new amplification level
*
* Returns:      TRUE if successful, FALSE if not.
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetAmplification(DWORD _amplification)
{
    int error;
    static unsigned amp;

    M_LOCK
    
    amplification = _amplification;    

    /* If channels are already open, set the amplification level: */
    if ( midasChannels != 0 )
    {
        amp = (channelAmplification * amplification) / 100;

        if ( (error = midasSD->SetAmplification(amp)) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return FALSE;
        }
    }

    M_UNLOCK

    return TRUE;
}




/****************************************************************************\
*
* Function:     DWORD MIDASallocateChannel(void)
*
* Description:  Allocates a sound channel
*
* Returns:      Channel number or MIDAS_ILLEGAL_CHANNEL if failed
*
\****************************************************************************/

_FUNC(DWORD) MIDASallocateChannel(void)
{
    int         i;

    M_LOCK

    if ( midasChannels == 0 )
    {
        mLastError = errNoChannels;
        M_UNLOCK
        return MIDAS_ILLEGAL_CHANNEL;
    }

    for ( i = 0; i < midasChannels; i++ )
    {
        if ( !chanAllocated[i] )
        {
            chanAllocated[i] = 1;
            M_UNLOCK
            return i;
        }
    }

    mLastError = errOutOfChannels;

    M_UNLOCK
    return MIDAS_ILLEGAL_CHANNEL;
}




/****************************************************************************\
*
* Function:     BOOL MIDASfreeChannel(DWORD channel)
*
* Description:  Deallocates an allocated sound channel
*
* Input:        DWORD channel           channel number
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASfreeChannel(DWORD channel)
{
    M_LOCK
    
    if ( midasChannels == 0 )
    {
        mLastError = errNoChannels;
        M_UNLOCK
        return FALSE;
    }

    chanAllocated[channel] = 0;

    M_UNLOCK
    return TRUE;
}




/****************************************************************************\
*
* Function:     MIDASmodule MIDASloadModule(char *fileName)
*
* Description:  Loads a module into memory
*
* Input:        char *fileName          module file name
*
* Returns:      Module handle if successful, NULL if failed
*
\****************************************************************************/

_FUNC(MIDASmodule) MIDASloadModule(char *fileName)
{
    static fileHandle  f;
    static char buf[64];
    int         error;
    static gmpModule *module;

    /* Check the module type and use the correct module loader
       (fixme, hardwired types and poor detection) */
    if ( (error = fileOpen(fileName, fileOpenRead, &f)) != OK )
    {
        mLastError = error;
        return FALSE;
    }
    if ( (error = fileRead(f, buf, 48)) != OK )
    {
        fileClose(f);
        mLastError = error;
        return FALSE;
    }

    if ( (error = fileClose(f)) != OK )
    {
        mLastError = error;
        return FALSE;
    }
	
    if ( mMemEqual(buf, "Extended Module:", 16) )
    {
        /* It's a FastTracker module: */
        error = gmpLoadXM(fileName, 1, NULL, &module);
    }
    else
    {
        if ( mMemEqual(buf+44, "SCRM", 4) )
        {
            /* It's a Scream Tracker 3 module */
            error = gmpLoadS3M(fileName, 1, NULL, &module);
        }
        else
        {
		if ( mMemEqual(buf, "IMPM", 4 ) )
		{
		    /* It's a Impulse Tracker module */
		    error = gmpLoadIT(fileName, 1, NULL, &module);
		}
		else
		{
			/* None of the above - we'll assume it's a Protracker module,
			   the loader will fail if this is not the case */
			error = gmpLoadMOD(fileName, 1, NULL, &module);
		}
	 }		    
    }

    if ( error != OK )
    {
        mLastError = error;
        return NULL;
    }

    return (MIDASmodule) module;
}




/****************************************************************************\
*
* Function:     MIDASmodulePlayHandle MIDASplayModuleSection(
*                   MIDASmodule module, unsigned startPos, unsigned endPos,
*                   unsigned restartPos, BOOL loopSong)
*
* Description:  Starts playing a section of a module loaded to memory
*
* Input:        MIDASmodule module      module handle for the loaded module
*               unsigned startPos       module section start position
*               unsigned endPos         module section end position
*               unsigned restartPos     module section restart position
*               BOOL loopSong           should the song be looped?
*
* Returns:      MIDAS module playing handle if successful, 0 if failed
*
* Notes:        If sound channels have been opened previously with
*               MIDASopenChannels(), MIDASplayModule() will allocate the
*               necessary channels, and MIDASstopModule() then deallocate
*               them.
*
*               Set startPos, endPos and restartPos to MIDAS_POSITION_DEFAULT
*               to play the whole module.
*
\****************************************************************************/

_FUNC(MIDASmodulePlayHandle) MIDASplayModuleSection(MIDASmodule module,
                                                    unsigned startPos,
                                                    unsigned endPos,
                                                    unsigned restartPos,
                                                    BOOL loopSong)
{
    unsigned    numChans;
    int         error;
    gmpModule   *gmpmod = (gmpModule*) module;
    unsigned    handleNum = 0xFFFF, i, u;
    ModulePlayHandle *playHandle;

    M_LOCK

    for ( i = 0; i < MAXPLAYHANDLES; i++ )
    {
        if ( mPlayHandles[i] == NULL )
        {
            handleNum = i;
            break;
        }
    }

    if ( handleNum == 0xFFFF )
    {
        mLastError = errOutOfResources;
        M_UNLOCK
        return 0;
    }

    numChans = gmpmod->numChannels;

    /* Open Sound Device channels if not already open: */
    if ( midasChannels == 0 )
    {
	if ( !MIDASopenChannels(numChans) )
	{
	    M_UNLOCK
            return 0;
	}
	playModuleOpenChannels = 1;
    }
    else
    {
        playModuleOpenChannels = 0;
    }

    /* Allocate play handle: */
    if ( (error = memAlloc(sizeof(ModulePlayHandle) +
                           numChans * sizeof(unsigned), (void**) &playHandle))
         != OK )
    {
        mLastError = error;
        M_UNLOCK
        return 0;
    }

    mPlayHandles[handleNum] = playHandle;
    playHandle->numChannels = numChans;

    /* Allocate the channels: */
    for ( i = 0; i < numChans; i++ )
    {
        if ( (playHandle->channels[i] = MIDASallocateChannel()) ==
             MIDAS_ILLEGAL_CHANNEL )
        {
            mLastError = errOutOfChannels;
                
            for ( u = 0; u < i; i++ )
                MIDASfreeChannel(playHandle->channels[u]);
            memFree(playHandle);

            M_UNLOCK
            return 0;
        }
    }

    /* Start playing the whole song: */
    if ( (error = gmpPlaySong(gmpmod, (int) startPos, (int) endPos,
                              (int) restartPos, (int) loopSong, -1,
                              playHandle->channels,
                              &playHandle->playHandle)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return 0;
    }
    midasGMPPlay = 1;

#if defined (__DOS__) && (!defined(NOTIMER))
    if ( !midasTMRMusic )
    {
        /* Start playing using the timer: */
        if ( (error = tmrPlayMusic(&gmpPlay, &midasPlayerNum)) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return 0;
        }
        if ( (error = gmpSetUpdRateFunct(&tmrSetUpdRate)) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return 0;
        }
        midasTMRMusic = 1;
        timerMusicCount = 1;
    }
    else
    {
        timerMusicCount++;
    }
#endif

    M_UNLOCK
    return handleNum + 1;
}




/****************************************************************************\
*
* Function:     MIDASmodulePlayHandle MIDASplayModule(MIDASmodule module,
*                   BOOL loopSong)
*
* Description:  Starts playing a module loaded to memory
*
* Input:        MIDASmodule module      module handle for the loaded module
*               BOOL loopSong           should the song be looped?
*
* Returns:      MIDAS module playing handle if successful, 0 if failed
*
* Notes:        If sound channels have been opened previously with
*               MIDASopenChannels(), MIDASplayModule() will allocate the
*               necessary channels, and MIDASstopModule() then deallocate
*               them.
*
\****************************************************************************/

_FUNC(MIDASmodulePlayHandle) MIDASplayModule(MIDASmodule module,
                                             BOOL loopSong)
{
    return MIDASplayModuleSection(module, (unsigned) MIDAS_POSITION_DEFAULT,
                                  (unsigned) MIDAS_POSITION_DEFAULT,
                                  (unsigned) MIDAS_POSITION_DEFAULT, loopSong);
}




/****************************************************************************\
*
* Description:  Stops playing a module and uninitializes the Module Player.
=======
* Function:     BOOL MIDASstopModule(MIDASmodulePlayHandle playHandle)
*
* Input:        MIDASmodulePlayHandle playHandle  playing handle
*
* Description:  Stops playing a module.
*               If sound channels were NOT opened through midasOpenChannels(),
*               but by letting midasPlayModule() open them, they will be
*               closed. Sound channels opened with midasOpenChannels() are NOT
*               closed and must be closed separately.
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASstopModule(MIDASmodulePlayHandle playHandle)
{
    int         error;
    unsigned    i;
    ModulePlayHandle *h = mPlayHandles[playHandle-1];

    M_LOCK

    /* Mark the playing handle free now to make sure that MIDASclose() won't
       try to stop the module again if we fail: */
    mPlayHandles[playHandle-1] = NULL;

#if defined (__DOS__) && (!defined(NOTIMER))
    timerMusicCount--;

    if ( timerMusicCount <= 0 )
    {
        /* Stop playing music with timer: */
        if ( (error = gmpSetUpdRateFunct(NULL)) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return FALSE;
        }
        if ( (error = tmrStopMusic(midasPlayerNum)) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return FALSE;
        }
        
        midasTMRMusic = 0;
    }
#endif

    /* Stop playing the module: */
    if ( (error = gmpStopSong(h->playHandle)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }
    midasGMPPlay = 0;
    midasPlayHandle = NULL;

    /* Deallocate channels and stop any sounds on them: */
    for ( i = 0; i < h->numChannels; i++ )
    {
        if ( (error = midasSD->StopSound(h->channels[i])) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return FALSE;
        }
        if ( (error = midasSD->SetVolume(h->channels[i], 0)) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return FALSE;
        }

        if ( !MIDASfreeChannel(h->channels[i]) )
        {
            M_UNLOCK
            return FALSE;
        }
    }

    /* If Sound Device channels were not opened with midasOpenChannels(),
       close them: */
    if ( playModuleOpenChannels )
    {
        if ( !MIDAScloseChannels() )
        {
            M_UNLOCK
            return FALSE;
        }
        playModuleOpenChannels = 0;
    }

    /* Deallocate the playing handle: */
    if ( (error = memFree(h)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASfreeModule(MIDASmodule module)
*
* Description:  Deallocates a module loaded with MIDASloadModule();
*
* Input:        DWORD module            module handle
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASfreeModule(MIDASmodule module)
{
    int         error;

    M_LOCK

    if ( (error = gmpFreeModule((gmpModule*) module)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASstartBackgroundPlay(DWORD pollRate)
*
* Description:  Starts playing music in the background
*
* Input:        DWORD pollRate          polling rate (in Hz - polls per
*                                       second)
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASstartBackgroundPlay(DWORD pollRate)
{
#if defined(__WIN32__) || defined(__UNIX__)
    DWORD       pollPeriod;

    /* Calculate delay between polls - default is 20ms, otherwise calculate
       delay based on rate, and divide by two to make sure polling is done
       at least often enough */
    if ( !pollRate )
        pollPeriod = 20;
    else
        pollPeriod = 1000 / pollRate / 2;

    StartPlayThread(pollPeriod);
    MIDASthread = 1;
#else
    pollRate = pollRate;
#endif
    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASstopBackgroundPlay(void)
*
* Description:  Stops playing music in the background
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASstopBackgroundPlay(void)
{
#if defined(__WIN32__) || defined(__UNIX__)
    if ( !MIDASthread )
        return TRUE;

    StopPlayThread();
    MIDASthread = 0;
#endif
    return TRUE;
}


#if defined(__WIN32__) || defined(__UNIX__)


/****************************************************************************\
*
* Function:     BOOL MIDASpoll(void)
*
* Description:  Polls the sound and music player manually
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASpoll(void)
{
    PollMIDAS();

    return TRUE;
}


#endif /* #if defined(__WIN32__) || defined(__UNIX__) */



#ifdef M_HAVE_THREADS


/****************************************************************************\
*
* Function:     void MIDASlock(void)
*
* Description:  Locks the MIDAS player thread by entering a critical section.
*
\****************************************************************************/

_FUNC(void) MIDASlock(void)
{
    LockPollThread();
}



/****************************************************************************\
*
* Function:     void MIDASunlock(void)
*
* Description:  Unlocks MIDAS player thread
*
\****************************************************************************/

_FUNC(void) MIDASunlock(void)
{
    UnlockPollThread();
}
    

#endif /*#ifdef M_HAVE_THREADS */

/****************************************************************************\
*
* Function:     BOOL MIDASsetOption(int option, DWORD value);
*
* Description:  Sets a MIDAS option
*
* Input:        int option              option (see enum MIDASoptions)
*               DWORD value             value for the option
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetOption(int option, DWORD value)
{
#define M_SET_OPT(opt, var) \
        case opt: \
            var = value; \
            return TRUE;

    switch ( option )
    {
        M_SET_OPT(MIDAS_OPTION_MIXRATE, midasMixRate)
        M_SET_OPT(MIDAS_OPTION_OUTPUTMODE, midasOutputMode)
        M_SET_OPT(MIDAS_OPTION_MIXBUFLEN, mBufferLength)
        M_SET_OPT(MIDAS_OPTION_MIXBUFBLOCKS, mBufferBlocks)
        M_SET_OPT(MIDAS_OPTION_DSOUND_MODE, mDSoundMode)
        M_SET_OPT(MIDAS_OPTION_DSOUND_BUFLEN, mDSoundBufferLength)
        M_SET_OPT(MIDAS_OPTION_16BIT_ULAW_AUTOCONVERT, m16bitUlawAutoConvert)
        M_SET_OPT(MIDAS_OPTION_FILTER_MODE, mOutputFilterMode)
        M_SET_OPT(MIDAS_OPTION_DEFAULT_STEREO_SEPARATION,
                  mDefaultStereoSeparation)
        M_SET_OPT(MIDAS_OPTION_FORCE_NO_SOUND, forceNoSound)

        case MIDAS_OPTION_MIXING_MODE:
            switch ( value )
            {
                case MIDAS_MIX_HIGH_QUALITY:
                    mOversampling = 1;
                    return TRUE;

                default:
                    mOversampling = 0;
                    return TRUE;
            }        

        case MIDAS_OPTION_DSOUND_HWND:
            mDSoundHwnd = (void*) value;
            return TRUE;

        case MIDAS_OPTION_DSOUND_OBJECT:
            mDSoundObject = (void*) value;
            return TRUE;
    }

    return TRUE;
}



/****************************************************************************\
*
* Function:     DWORD MIDASgetOption(int option);
*
* Description:  Sets a MIDAS option
*
* Input:        int option              option (see enum MIDASoptions)
*
* Returns:      Current option value
*
\****************************************************************************/

_FUNC(DWORD) MIDASgetOption(int option)
{
#define M_GET_OPT(opt, var) \
    case opt: \
        return (DWORD) var;

    switch ( option )
    {
        M_GET_OPT(MIDAS_OPTION_MIXRATE, midasMixRate)
        M_GET_OPT(MIDAS_OPTION_OUTPUTMODE, midasOutputMode)
        M_GET_OPT(MIDAS_OPTION_MIXBUFLEN, mBufferLength)
        M_GET_OPT(MIDAS_OPTION_MIXBUFBLOCKS, mBufferBlocks)
        M_GET_OPT(MIDAS_OPTION_DSOUND_MODE, mDSoundMode)
        M_GET_OPT(MIDAS_OPTION_DSOUND_BUFLEN, mDSoundBufferLength)
        M_GET_OPT(MIDAS_OPTION_16BIT_ULAW_AUTOCONVERT, m16bitUlawAutoConvert)
        M_GET_OPT(MIDAS_OPTION_FILTER_MODE, mOutputFilterMode)
        M_GET_OPT(MIDAS_OPTION_DEFAULT_STEREO_SEPARATION,
                  mDefaultStereoSeparation)
        M_GET_OPT(MIDAS_OPTION_DSOUND_HWND, mDSoundHwnd)
        M_GET_OPT(MIDAS_OPTION_DSOUND_OBJECT, mDSoundObject)
        M_GET_OPT(MIDAS_OPTION_FORCE_NO_SOUND, forceNoSound)

        case MIDAS_OPTION_MIXING_MODE:
            if ( mOversampling )
                return MIDAS_MIX_HIGH_QUALITY;
            return MIDAS_MIX_NORMAL_QUALITY;
    }

    return 0;
}



/****************************************************************************\
*
* Function:     MIDASsample MIDASloadRawSample(char *filename, int sampleType,
*                   int loopSample)
*
* Description:  Loads a raw sound effect sample
*
* Input:        char *filename          sample file name
*               int sampleType          sample type
*               int loopSample          1 if sample should be looped
*
* Returns:      MIDAS sample handle, NULL if failed
*
\****************************************************************************/

_FUNC(MIDASsample) MIDASloadRawSample(char *fileName, int sampleType,
    int loopSample)
{
    int         error;
    static unsigned sampleHandle;

    /* Load the sample: */
    if ( (error = fxLoadRawSample(fileName, sampleType, loopSample,
        &sampleHandle)) != OK )
    {
        mLastError = error;
        return 0;
    }

    return sampleHandle;
}




/****************************************************************************\
*
* Function:     MIDASsample MIDASloadWaveSample(char *filename,
*                   int loopSample)
*
* Description:  Loads a RIFF WAVE sound effect sample
*
* Input:        char *filename          sample file name
*               int loopSample          1 if sample should be looped
*
* Returns:      MIDAS sample handle, NULL if failed
*
\****************************************************************************/

_FUNC(MIDASsample) MIDASloadWaveSample(char *fileName, int loopSample)
{
    int         error;
    static unsigned sampleHandle;

    /* Load the sample: */
    if ( (error = fxLoadWaveSample(fileName, loopSample, &sampleHandle))
         != OK )
    {
        mLastError = error;
        return 0;
    }

    return sampleHandle;
}




/****************************************************************************\
*
* Function:     BOOL MIDASfreeSample(MIDASsample sample)
*
* Description:  Deallocates a sample
*
* Input:        MIDASsample sample      sample to be deallocated
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASfreeSample(MIDASsample sample)
{
    int         error;

    if ( (error = fxFreeSample(sample)) != OK )
    {
        mLastError = error;
        return FALSE;
    }

    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASallocAutoEffectChannels(unsigned numChannels)
*
* Description:  Allocates a number of channels for automatic sound effect
*               channels
*
* Input:        unsigned numChannels    number of channels to use
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASallocAutoEffectChannels(unsigned numChannels)
{
    int         error;
    unsigned    i;
    unsigned    n;

    /* If we have previous automatic effect channels, deallocate them: */
    if ( numAutoEffectChannels > 0 )
    {
        if ( !MIDASfreeAutoEffectChannels() )
            return FALSE;
    }

    /* Allocate memory for the channel numbers: */
    if ( (error = memAlloc(numChannels * sizeof(unsigned),
                           (void**) &autoEffectChannels)) != OK )
    {
        mLastError = error;
        return FALSE;
    }

    /* Allocate the channels: */
    for ( i = 0; i < numChannels; i++ )
    {
        if ( (autoEffectChannels[i] = MIDASallocateChannel()) ==
             MIDAS_ILLEGAL_CHANNEL )
        {
            for ( n = 0; n < i; n++ )
                MIDASfreeChannel(autoEffectChannels[i]);
            memFree(autoEffectChannels);
            return FALSE;
        }
    }

    /* Set the channels: */
    if ( (error = fxSetAutoChannels(numChannels, autoEffectChannels)) != OK )
    {
        for ( i = 0; i < numChannels; i++ )
            MIDASfreeChannel(autoEffectChannels[i]);
        memFree(autoEffectChannels);
        mLastError = error;
        return FALSE;
    }

    numAutoEffectChannels = numChannels;

    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASfreeAutoEffectChannels(void)
*
* Description:  Frees the channels allocated for automatic effect channels
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASfreeAutoEffectChannels(void)
{
    int error;
    unsigned i;

    if ( (numAutoEffectChannels == 0) || (autoEffectChannels == NULL) )
        return TRUE;    

    /* First, stop all sounds on the channels: */
    for ( i = 0; i < numAutoEffectChannels; i++ )
    {
        if ( (error = midasSD->StopSound(autoEffectChannels[i])) != OK )
        {
            mLastError = error;
            return FALSE;
        }

        if ( (error = midasSD->SetVolume(autoEffectChannels[i], 0)) != OK )
        {
            mLastError = error;
            return FALSE;
        }
    }

    numAutoEffectChannels = 0;

    if ( (error = memFree(autoEffectChannels)) != OK )
    {
        autoEffectChannels = NULL;
        mLastError = error;
        return FALSE;
    }

    autoEffectChannels = NULL;

    return TRUE;
}




/****************************************************************************\
*
* Function:     MIDASsamplePlayHandle MIDASplaySample(MIDASsample sample,
*                   unsigned channel, int priority, unsigned rate,
*                   unsigned volume, int panning)
*
* Description:  Plays a sound effect sample
*
* Input:        MIDASsample sample      sample to be played
*               unsigned channel        channel the sample should be played
*                                       on, MIDAS_CHANNEL_AUTO for automatic
*                                       selection
*               int priority            sample playing priority, the higher
*                                       the value the higher the priority
*               unsigned rate           initial sample rate
*               unsigned volume         initial volume
*               int panning             initial panning position
*
* Returns:      Sample playing handle or NULL if failed
*
\****************************************************************************/

_FUNC(MIDASsamplePlayHandle) MIDASplaySample(MIDASsample sample,
    unsigned channel, int priority, unsigned rate, unsigned volume,
    int panning)
{
    int         error;
    static unsigned playHandle;

    M_LOCK

    if ( (error = fxPlaySample(channel, sample, priority, rate, volume,
        panning, &playHandle)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return 0;
    }

    M_UNLOCK
    /* KLUGE! Add 1 to the handle to make sure NULL is an illegal handle: */
    return playHandle + 1;
}




/****************************************************************************\
*
* Function:     BOOL MIDASstopSample(MIDASsamplePlayHandle sample)
*
* Description:  Stops playing a sample
*
* Input:        MIDASsamplePlayHandle sample    sample playing handle
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASstopSample(MIDASsamplePlayHandle sample)
{
    int         error;

    M_LOCK

    if ( (error = fxStopSample(sample-1)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASsetSampleRate(MIDASsamplePlayHandle sample,
*                   unsigned rate)
*
* Description:  Changes the sample rate for a sound effect sample that is
*               being played
*
* Input:        MIDASsamplePlayHandle sample    sample to change
*               unsigned rate                   new rate
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetSampleRate(MIDASsamplePlayHandle sample, unsigned rate)
{
    int         error;

    M_LOCK

    if ( (error = fxSetSampleRate(sample-1, rate)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASsetSampleVolume(MIDASsamplePlayHandle sample,
*                   unsigned volume)
*
* Description:  Changes the volume for a sound effect sample that is being
*               played
*
* Input:        MIDASsamplePlayHandle sample    sample to change
*               unsigned volume                 new volume
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetSampleVolume(MIDASsamplePlayHandle sample,
    unsigned volume)
{
    int         error;

    M_LOCK
    
    if ( (error = fxSetSampleVolume(sample-1, volume)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASsetSamplePanning(MIDASsamplePlayHandle sample,
*                   int panning)
*
* Description:  Changes the panning position of a sound effect sample that is
*               being played
*
* Input:        MIDASsamplePlayHandle sample    sample to change
*               int panning                     new panning position
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetSamplePanning(MIDASsamplePlayHandle sample,
    int panning)
{
    int         error;

    M_LOCK

    if ( (error = fxSetSamplePanning(sample-1, panning)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASsetSamplePriority(MIDASsamplePlayHandle sample,
*                   int priority)
*
* Description:  Changes the playing priority of a sound effect sample that is
*               being played
*
* Input:        MIDASsamplePlayHandle sample    sample to change
*               int priority                    new playing priority
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetSamplePriority(MIDASsamplePlayHandle sample,
    int priority)
{
    int         error;

    M_LOCK

    if ( (error = fxSetSamplePriority(sample-1, priority)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}




/****************************************************************************\
*
* Function:     DWORD MIDASgetSamplePlayStatus(MIDASsamplePlayHandle sample)
*
* Description:  Gets the playing status of a sample
*
* Input:        MIDASsamplePlayHandle sample   sample playing handle
*
* Returns:      Sample playing status, see enum MIDASsamplePlayStatus
*
\****************************************************************************/

_FUNC(DWORD) MIDASgetSamplePlayStatus(MIDASsamplePlayHandle sample)
{
    static unsigned status;
    int error;

    M_LOCK

    if ( (error = fxGetSamplePlayStatus(sample-1, &status)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return MIDAS_SAMPLE_STOPPED;
    }

    M_UNLOCK
    return status;
}




/****************************************************************************\
*
* Function:     BOOL MIDASgetPlayStatus(MIDASplayStatus *status)
*
* Description:  Gets current module playing status
*
* Input:        MIDASplayStatus *status     pointer to status structure
*
* Returns:      TRUE if successful, FALSE if not.
*               Module playing status will be written to *status
*
\****************************************************************************/

_FUNC(BOOL) MIDASgetPlayStatus(MIDASmodulePlayHandle playHandle,
                               MIDASplayStatus *status)
{
    static gmpInformation *gmpInfo;
    int         error;

    /* Get information from GMPlayer: */
    if ( (error = gmpGetInformation(mPlayHandles[playHandle-1]->playHandle,
                                    &gmpInfo)) != OK )
    {
        mLastError = error;
        return FALSE;
    }

    /* Copy them: */
    status->position = gmpInfo->position;
    status->pattern = gmpInfo->pattern;
    status->row = gmpInfo->row;
    status->syncInfo = gmpInfo->syncInfo;
    status->songLoopCount = gmpInfo->songLoopCount;

    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASsetPosition(int newPosition)
*
* Description:  Sets module playback position
*
* Input:        int newPosition         new position
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetPosition(MIDASmodulePlayHandle playHandle, int newPosition)
{
    int         error;

    M_LOCK

    if ( (error = gmpSetPosition(mPlayHandles[playHandle-1]->playHandle,
                                 newPosition)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASsetMusicVolume
*
* Description:  Sets module playback volume
*
* Input:        unsigned volume     new volume
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetMusicVolume(MIDASmodulePlayHandle playHandle,
                                unsigned volume)
{
    int error;
    
    M_LOCK

    if ( (error = gmpSetMasterVolume(mPlayHandles[playHandle-1]->playHandle,
                                     volume)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK

    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASgetModuleInfo(MIDASmodule module,
*                   MIDASmoduleInfo *info)
*
* Description:  Gets information about a module
*
* Input:        MIDASmodule module      MIDAS module handle
*               MIDASmoduleInfo *info   pointer to module info structure
*
* Returns:      TRUE if successful, FALSE if not. Module information is
*               written to *info.
*
\****************************************************************************/

_FUNC(BOOL) MIDASgetModuleInfo(MIDASmodule module, MIDASmoduleInfo *info)
{
    gmpModule   *gmpMod = (gmpModule*) module;

    if ( module == NULL )
    {
        mLastError = errInvalidArguments;
        return FALSE;
    }

    mMemCopy(info->songName, gmpMod->name, 32);
    info->songName[31] = 0;
    info->songLength = gmpMod->songLength;
    info->numInstruments = gmpMod->numInsts;
    info->numPatterns = gmpMod->numPatts;
    info->numChannels = gmpMod->numChannels;

    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASfadeMusicChannel(unsigned channel, unsigned fade);
*
* Description:  Fades a music channel
*
* Input:        unsigned channel        channel number   
*               unsigned fade           fade: 64 is normal volume, 0 silence
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASfadeMusicChannel(MIDASmodulePlayHandle playHandle,
                                  unsigned channel, unsigned fade)
{
    int error;

    M_LOCK

    if ( (error = gmpFadeChannel(mPlayHandles[playHandle-1]->playHandle,
                                 channel, fade)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASgetInstrumentInfo(MIDASmodule module, int instNum,
*                   MIDASinstrumentInfo *info);
*
* Description:  Gets information about an instrument in a module
*
* Input:        MIDASmodule module      MIDAS module handle
*               int instNum             instrument number
*               MIDASinstrumentInfo *info   pointer to destination info struct
*
* Returns:      TRUE if successful, FALSE if not. Instrument information is
*               written to *info.
*
\****************************************************************************/

_FUNC(BOOL) MIDASgetInstrumentInfo(MIDASmodule module, int instNum,
    MIDASinstrumentInfo *info)
{
    gmpModule   *gmpMod = (gmpModule*) module;

    if ( (module == NULL) || (((unsigned) instNum) >= gmpMod->numInsts) )
    {
        mLastError = errInvalidArguments;
        return FALSE;
    }

    mMemCopy(info->instName, gmpMod->instruments[instNum]->name, 32);
    info->instName[31] = 0;

    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASsetMusicSyncCallback(
*                   void MIDAS_CALL (*callback)(DWORD syncInfo))
*
* Description:  Sets the music synchronization callback function. The callback
*               function is called each time a Wxx command is played (in FT2
*               or ST3 modules), and it gets as an argument the infobyte
*
* Input:        void MIDAS_CALL (*callback)(...)   pointer to callback
*                                                  function, NULL to disable
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetMusicSyncCallback(MIDASmodulePlayHandle playHandle, 
                                      void (MIDAS_CALL *callback)
                                      (unsigned syncInfo, unsigned position,
                                       unsigned row))
{
    int error;

    M_LOCK

    if ( (error = gmpSetSyncCallback(mPlayHandles[playHandle-1]->playHandle,
                                     callback)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}


#ifdef SUPPORTSTREAMS


#ifdef M_HAVE_THREADS

/****************************************************************************\
*
* Function:     MIDASstreamHandle MIDASplayStreamFile(
*                   char *fileName, unsigned sampleType, unsigned sampleRate,
*                   unsigned bufferLength, int loopStream)
*
* Description:  Starts playing a digital audio stream from a file
*
* Input:        char *fileName          stream file name
*               unsigned sampleType     stream sample type
*               unsigned sampleRate     stream sampling rate
*               unsigned bufferLength   stream buffer length in milliseconds
*               int loopStream          1 if stream should be looped
*
* Returns:      Stream handle or NULL if failed
*
\****************************************************************************/

_FUNC(MIDASstreamHandle) MIDASplayStreamFile(char *fileName,
    unsigned sampleType, unsigned sampleRate,
    unsigned bufferLength, int loopStream)
{
    int         error;
    static strStream *stream;
    unsigned    channel;

    M_LOCK

    /* First allocate a channel for the stream: */
    if ( (channel = MIDASallocateChannel()) == MIDAS_ILLEGAL_CHANNEL )
    {
        M_UNLOCK
        return NULL;
    }

    if ( (error = strPlayStreamFile(channel, fileName, sampleType, sampleRate,
        bufferLength, loopStream, &stream)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return NULL;
    }

    M_UNLOCK
    return (MIDASstreamHandle) stream;
}




/****************************************************************************\
*
* Function:     MIDASstreamHandle MIDASplayStreamWaveFile(
*                   char *fileName, unsigned bufferLength, int loopStream)
*
* Description:  Starts playing a digital audio stream from a RIFF WAVE file
*
* Input:        char *fileName          stream file name
*               unsigned bufferLength   stream buffer length in milliseconds
*               int loopStream          1 if stream should be looped
*
* Returns:      Stream handle or NULL if failed
*
\****************************************************************************/

_FUNC(MIDASstreamHandle) MIDASplayStreamWaveFile(char *fileName,
    unsigned bufferLength, int loopStream)
{
    int         error;
    static strStream *stream;
    unsigned    channel;

    M_LOCK

    /* First allocate a channel for the stream: */
    if ( (channel = MIDASallocateChannel()) == MIDAS_ILLEGAL_CHANNEL )
    {
        M_UNLOCK
        return NULL;
    }

    if ( (error = strPlayStreamWaveFile(channel, fileName, bufferLength,
                                        loopStream, &stream)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return NULL;
    }

    M_UNLOCK
    return (MIDASstreamHandle) stream;
}


#endif /* #ifdef M_HAVE_THREADS */



/****************************************************************************\
*
* Function:     BOOL MIDASstopStream(MIDASstreamHandle stream)
*
* Description:  Stops playing a stream
*
* Input:        MIDASstreamHandle stream    stream to be stopped
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASstopStream(MIDASstreamHandle stream)
{
    int         error;
    unsigned    channel;

    M_LOCK

    channel = ((strStream*) stream)->sdChannel;

    if ( (error = strStopStream((strStream*) stream)) != OK )
    {
        MIDASfreeChannel(channel);
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    if ( !MIDASfreeChannel(channel) )
    {
        M_UNLOCK
        return FALSE;
    }
    
    M_UNLOCK
    return TRUE;
}


/****************************************************************************\
*
* Function:     MIDASstreamHandle MIDASplayStreamPolling(
*                   unsigned sampleType, unsigned sampleRate,
*                   unsigned bufferLength);
*
* Description:  Starts playing a stream in polling mode. Use
*               MIDASfeedStreamData() to feed the stream data to the player
*
* Input:        unsigned sampleType     stream sample type
*               unsigned sampleRate     stream sampling rate
*               unsigned bufferLength   stream buffer length in milliseconds
*
* Returns:      Stream handle or NULL if failed
*
\****************************************************************************/

_FUNC(MIDASstreamHandle) MIDASplayStreamPolling(unsigned sampleType,
                                                unsigned sampleRate,
                                                unsigned bufferLength)
{
    int         error;
    static strStream *stream;
    unsigned    channel;

    M_LOCK

    /* First allocate a channel for the stream: */
    if ( (channel = MIDASallocateChannel()) == MIDAS_ILLEGAL_CHANNEL )
    {
        M_UNLOCK
        return NULL;
    }

    if ( (error = strPlayStreamPolling(channel, sampleType, sampleRate,
        bufferLength, &stream)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return NULL;
    }

    M_UNLOCK
    return (MIDASstreamHandle) stream;
}



/****************************************************************************\
*
* Function:     unsigned MIDASfeedStreamData(MIDASstreamHandle stream,
                    unsigned char *data, unsigned numBytes, BOOL feedAll)
* Description:  Feeds sample data to a stream that is being played in polling
*               mode.
*
* Input:        MIDASstreamHandle stream    Stream playing handle
*               uchar *data             pointer to stream data
*               unsigned numBytes       number of bytes of data to feed. Note!
*                                       This must be a multiple of the stream
*                                       sample size
*               BOOL feedAll            TRUE if all data should be fed in all
*                                       circumstances. The function will block
*                                       the current thread if this flag is 1
*                                       until all data is fed.
*
* Returns:      The number of bytes of sample data that was actually fed.
*
\****************************************************************************/

_FUNC(unsigned) MIDASfeedStreamData(MIDASstreamHandle stream,
    unsigned char *data, unsigned numBytes, BOOL feedAll)
{
    int         error;
    static unsigned numFed;
    int         iFeedAll;

    if ( feedAll )
        iFeedAll = 1;
    else
        iFeedAll = 0;

    if ( !feedAll )
        M_LOCK

    if ( (error = strFeedStreamData((strStream*) stream, data, numBytes,
        iFeedAll, &numFed)) != OK )
    {
        mLastError = error;
        if ( !feedAll )
            M_UNLOCK
        return 0;
    }

    if ( !feedAll )
        M_UNLOCK

    return numFed;
}



/****************************************************************************\
*
* Function:     BOOL MIDASsetStreamRate(MIDASstreamHandle stream,
*                   unsigned rate)
*
* Description:  Changes the playback rate of a stream
*
* Input:        MIDASstreamHandle stream    Stream playing handle
*               unsigned rate           New playback sample rate, in Hz
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetStreamRate(MIDASstreamHandle stream, unsigned rate)
{
    int         error;

    M_LOCK

    if ( (error = strSetStreamRate((strStream*) stream, rate)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASsetStreamVolume(MIDASstreamHandle stream,
*                   unsigned volume)
*
* Description:  Changes the playback volume of a stream
*
* Input:        MIDASstreamHandle stream    Stream playing handle
*               unsigned volume         New volume
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetStreamVolume(MIDASstreamHandle stream, unsigned volume)
{
    int         error;

    M_LOCK

    if ( (error = strSetStreamVolume((strStream*) stream, volume)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASsetStreamPanning(MIDASstreamHandle stream,
*                   int panning)
*
* Description:  Changes the panning position of a stream
*
* Input:        MIDASstreamHandle stream    Stream playing handle
*               int panning             New panning position
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASsetStreamPanning(MIDASstreamHandle stream, int panning)
{
    int         error;

    M_LOCK

    if ( (error = strSetStreamPanning((strStream*) stream, panning)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}




/****************************************************************************\
*
* Function:     DWORD MIDASgetStreamBytesBuffered(MIDASstreamHandle stream)
*
* Description:  Get the amount of data currently stored in the stream buffer
*
* Input:        MIDASstreamHandle stream stream handle
*
* Returns:      The number of bytes of data currently buffered.
*
\****************************************************************************/

_FUNC(DWORD) MIDASgetStreamBytesBuffered(MIDASstreamHandle stream)
{
    int error;
    static unsigned buffered;

    M_LOCK

    if ( (error = strGetStreamBytesBuffered((strStream*) stream, &buffered))
         != OK )
    {
        mLastError = error;
        M_UNLOCK
        return 0;
    }

    M_UNLOCK
    return buffered;
}




/****************************************************************************\
*
* Function:     BOOL MIDASpauseStream(MIDASstreamHandle stream)
*
* Description:  Pauses stream playback
*
* Input:        MIDASstreamHandle stream  stream to pause
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASpauseStream(MIDASstreamHandle stream)
{
    int error;

    M_LOCK

    if ( (error = strPauseStream((strStream*) stream, 1)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}

    


/****************************************************************************\
*
* Function:     BOOL MIDASresumeStream(MIDASstreamHandle stream)
*
* Description:  Resumes paused stream playback
*
* Input:        MIDASstreamHandle stream  stream to resume
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASresumeStream(MIDASstreamHandle stream)
{
    int error;

    M_LOCK

    if ( (error = strPauseStream((strStream*) stream, 0)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK
    return TRUE;
}



#endif /* #ifdef SUPPORTSTREAMS */


/****************************************************************************\
*
* Function:     MIDASechoHandle MIDASaddEchoEffect(MIDASechoSet *echoSet)
*
* Description:  Adds an echo effect to the output
*
* Input:        MIDASechoSet *echoSet   echo effect description
*
* Returns:      Echo effect handle if successful, NULL if failed
*
\****************************************************************************/

_FUNC(MIDASechoHandle) MIDASaddEchoEffect(MIDASechoSet *echoSet)
{
    static mEchoHandle h;
    int error;
    
    M_LOCK

    /* Initialize the echo effects engine if necessary: */
    if ( numEchoEffects == 0 )
    {
        if ( (error = mEchoInit()) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return NULL;
        }
    }

    numEchoEffects++;

    if ( (error = mEchoAddEffect((mEchoSet*) echoSet, &h)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return NULL;
    }

    if ( h == NULL )
        h = (mEchoSet*) 0x1742;

    M_UNLOCK

    return (MIDASechoHandle) h;
}



/****************************************************************************\
*
* Function:     BOOL MIDASremoveEchoEffect(MIDASechoHandle echoHandle);
*
* Description:  Removes an echo effect
*
* Input:        MIDASechoHandle echoHandle  echo effect handle
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASremoveEchoEffect(MIDASechoHandle echoHandle)
{
    int error;
    
    M_LOCK
    
    if ( !numEchoEffects )
    {
        M_UNLOCK
        return TRUE;
    }
    
    if ( (error = mEchoRemoveEffect((mEchoHandle) echoHandle)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    numEchoEffects--;

    if ( !numEchoEffects )
    {
        if ( (error = mEchoClose()) != OK )
        {
            mLastError = error;
            M_UNLOCK
            return FALSE;
        }
    }

    M_UNLOCK

    return TRUE;
}



/****************************************************************************\
*
* Function:     BOOL MIDASaddPostProcessor(MIDASpostProcessor *postProc,
*                   unsigned procPos, void *userData)
*
* Description:  Adds a post processor to the sound output
*
* Input:        MIDASpostProcessor *postProc  the post processor structure
*               unsigned procPos        post processing position, see enum
*                                       MIDASpostProcPosition
*               void *userData          user data for post-processing
*                                       functions
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASaddPostProcessor(MIDASpostProcessor *postProc,
                                  unsigned procPos, void *userData)
{
    int error;

    M_LOCK

    if ( (error = dsmAddPostProcessor((dsmPostProcessor*) postProc, procPos,
                                      userData)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK

    return TRUE;
}




/****************************************************************************\
*
* Function:     BOOL MIDASremovePostProcessor(MIDASpostProcessor *postProc)
*
* Description:  Removes a post processor from sound output
*
* Input:        MIDASpostProcessor *postProc  the post processor
*
* Returns:      TRUE if successful, FALSE if not
*
\****************************************************************************/

_FUNC(BOOL) MIDASremovePostProcessor(MIDASpostProcessor *postProc)
{
    int error;

    M_LOCK

    if ( (error = dsmRemovePostProcessor((dsmPostProcessor*) postProc)) != OK )
    {
        mLastError = error;
        M_UNLOCK
        return FALSE;
    }

    M_UNLOCK

    return TRUE;
}


#ifdef __WIN32__


int PASCAL WEP(short nParameter)
{
    /* The DLL is being unloaded by a process */

    /* Stop playing in a thread: */
    if ( MIDASthread )
    {
        MIDASstopBackgroundPlay();
    }

    /* Close MIDAS - this can safely be done many times: */
    MIDASclose();

    DLLinUse = 0;

    return 1;
}


/* The DLL main: */


int APIENTRY LibMain(HANDLE hdll, DWORD reason, LPVOID reserved)
{
    switch ( reason )
    {
        case DLL_PROCESS_ATTACH:
            /* The DLL is loaded by a process. Check that the DLL is not in
               use by some other process, and if not, mark that we are in use
               and set to default config: */
            if ( DLLinUse )
                return 0;               /* Only one program can use MIDAS */

            DLLinUse = 1;
            MIDASstartup();

            break;

        case DLL_PROCESS_DETACH:
            /* The DLL is being unloaded by a process */

            /* Stop playing in a thread: */
            if ( MIDASthread )
            {
                MIDASstopBackgroundPlay();
            }

            /* Close MIDAS - this can safely be done many times: */
            MIDASclose();

            DLLinUse = 0;

            break;

        /* We aren't interested in thread creation */
    }

    return 1;
}


#endif /* #ifdef __WIN32__ */


/*
 * $Log: midasdll.c,v $
 * Revision 1.42.2.4  1998/01/21 21:57:25  pekangas
 * Fixed changing amplification after it has been set already once
 *
 * Revision 1.42.2.3  1997/09/06 17:18:09  pekangas
 * Added optional debugging messages
 *
 * Revision 1.42.2.2  1997/08/21 22:30:58  pekangas
 * MIDAS_OPTION_FORCE_NOSOUND now works with Linux
 *
 * Revision 1.42.2.1  1997/08/20 12:48:05  pekangas
 * Manually merged fixes from the main trunk (oops)
 *
 * Revision 1.43  1997/08/19 17:21:35  pekangas
 * Added profiling support initialization/uninitialization
 *
 * Revision 1.42  1997/08/18 14:01:18  pekangas
 * Removed dsm.h from midas.h
 *
 * Revision 1.41  1997/08/18 13:35:11  pekangas
 * Fixed to compile for MS-DOS
 *
 * Revision 1.40  1997/08/17 12:34:29  pekangas
 * Fixed MIDASsetAmplification when channels were already open
 * Changed MIDASsetMusicVolume to use gmpSetMasterVolume
 *
 * Revision 1.39  1997/08/15 16:48:25  pekangas
 * Added user post-processing functions
 *
 * Revision 1.38  1997/07/31 14:30:57  pekangas
 * Added option MIDAS_OPTION_FORCE_NO_SOUND
 *
 * Revision 1.37  1997/07/31 10:56:48  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.36  1997/07/29 16:51:09  pekangas
 * Added sample playing status query
 *
 * Revision 1.35  1997/07/26 14:13:47  pekangas
 * Changed MIDASaddEchoEffect() to return a non-NULL "echo handle" if
 * the echo engine returned NULL without error (echoes are disabled)
 *
 * Revision 1.34  1997/07/25 13:48:24  pekangas
 * Changed MIDAS_OPTION_OVERSAMPLING setting to MIDAS_OPTION_MIXING_MODE
 * Added automatic filtering
 * Added MIDASgetOption()
 *
 * Revision 1.33  1997/07/10 18:40:23  pekangas
 * Added echo effect support
 *
 * Revision 1.32  1997/07/09 08:58:00  pekangas
 * Added default stereo separation
 *
 * Revision 1.31  1997/06/10 19:51:24  jpaana
 * Added IT-loader
 *
 * Revision 1.30  1997/06/05 20:18:48  pekangas
 * Added preliminary support for interpolating mixing (mono only at the
 * moment)
 *
 * Revision 1.29  1997/06/02 00:54:15  jpaana
 * Changed most __LINUX__ defines to __UNIX__ for generic *nix porting
 *
 * Revision 1.28  1997/05/23 15:12:19  jpaana
 * Fixed a typo
 *
 * Revision 1.27  1997/05/20 20:37:58  pekangas
 * Added WAVE support to both streams and samples
 *
 * Revision 1.26  1997/05/20 10:16:29  pekangas
 * Added preliminary tracker mode support
 *
 * Revision 1.25  1997/05/07 17:14:52  pekangas
 * Added a lot of new thread synchronization code, mainly to minimize the
 * cases where MIDAS state may be modified when the player thread is active
 * and vice versa. Added MIDASlock() and MIDASunlock() to the API.
 *
 * Revision 1.24  1997/05/04 12:22:59  pekangas
 * Removed references to the IT loader (not in CVS yet)
 *
 * Revision 1.23  1997/05/03 21:57:43  jpaana
 * Fixed for Linux
 *
 * Revision 1.22  1997/05/03 21:57:05  pekangas
 * Fixed MIDASsetPosition
 *
 * Revision 1.21  1997/05/03 17:54:01  pekangas
 * Added optional simple output filtering
 *
 * Revision 1.20  1997/05/03 15:10:50  pekangas
 * Added stream support for DOS, removed GUS Sound Device from non-Lite
 * build. M_HAVE_THREADS now defined in threaded environment.
 *
 * Revision 1.19  1997/05/02 13:19:49  pekangas
 * Several changes: Added support for non-looping module playback, added
 * support for playing several modules simultaneously, added module section
 * playback, added automatic channel allocation and deallocation to stream
 * playback, simplified automatic effect channel handling and added functions
 * for allocating and deallocating individiual channels.
 *
 * Revision 1.18  1997/04/08 15:48:07  pekangas
 * Added gmpFadeChannel / MIDASfadeMusicChannel functions
 *
 * Revision 1.17  1997/04/07 21:07:51  pekangas
 * Added the ability to pause/resume streams.
 * Added functions to query the number of stream bytes buffered
 *
 * Revision 1.16  1997/03/09 19:13:00  pekangas
 * Added the possibility to turn off u-law autoconvert
 *
 * Revision 1.15  1997/03/05 16:49:48  pekangas
 * Added timer functions to DLL, some other minor modifications
 *
 * Revision 1.14  1997/02/27 16:03:48  pekangas
 * Fixed to work in DOS
 *
 * Revision 1.13  1997/02/22 18:24:49  jpaana
 * Added #ifdef's for M_SD_HAVE_SUSPEND
 *
 * Revision 1.12  1997/02/19 20:45:09  pekangas
 * Added functions MIDASsuspend() and MIDASresume()
 *
 * Revision 1.11  1997/02/12 17:18:36  pekangas
 * Added MIDASsetAmplification()
 *
 * Revision 1.10  1997/02/08 17:27:17  jpaana
 * Fixed some typos and function headers
 *
 * Revision 1.9  1997/02/06 20:58:20  pekangas
 * Added DirectSound support - new files, errors, and global flags
 *
 * Revision 1.8  1997/02/06 12:44:28  pekangas
 * Now #includes windows.h properly with Visual C, without magic command
 * line defined macros
 *
 * Revision 1.7  1997/02/05 17:40:42  pekangas
 * Changed to new makefile structure. Removed old makefiles, library
 * command files and related junk. Fixed some double linefeeds caused
 * by RCS-CVS transition. lang.h now defined NOTIMER for Win32.
 *
 * Revision 1.6  1997/01/16 19:57:19  pekangas
 * Removed a couple of Visual C warnings
 *
 * Revision 1.5  1997/01/16 19:43:55  pekangas
 * Removed a warning
 *
 * Revision 1.4  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.3  1997/01/16 18:26:27  pekangas
 * Added numerous new functions
 *
 * Revision 1.2  1996/12/07 22:19:56  pekangas
 * No change
 *
 * Revision 1.1  1996/09/25 18:38:02  pekangas
 * Initial revision
 *
*/
