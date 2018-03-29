/*      DEMO.C
 *
 * MIDAS megademo
 *
 * Copyright 1996 Petteri Kangaslampi
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include <midasdll.h>
#include "vga.h"

#ifdef __DJGPP__
#include <dpmi.h>
#endif



/* To start the program right away, without a configuration screen,
   uncomment the following line. However, this is generally NOT a good
   idea, as the user might want to use a different sound card than the
   one detected, or might want better sound quality than the default. Also
   some sound cards can NOT be detected at all.

   If you uncomment the following line, at least provide a way for the
   user to get to the setup by giving a command line argument - this
   program does also this. */

/*#define ATTEMPT_DETECT*/


/* Music playing position information - updated by UpdateInfo: */
unsigned        position;               /* Current position */
unsigned        pattern;                /* Current pattern number */
unsigned        row;                    /* Current row number */
int             syncInfo;               /* Music synchronization info */

/* Music file name ;) */
char            *moduleName = "..\\data\\templsun.xm";


/* Frame counter variable - note VOLATILE! */
volatile unsigned   frameCount;

static int      border = 0;

MIDASmodule module;                     /* Der Module */
MIDASmodulePlayHandle playHandle;       /* Das Playing Handle */



#ifdef __DJGPP__
/* DJGPP mode set routine: */
void SetMode(unsigned mode)
{
    __dpmi_regs regs;

    regs.x.ss = regs.x.sp = 0;
    regs.d.eax = (mode & 0xFF);
    __dpmi_simulate_real_mode_interrupt(0x10, &regs);
}
#define vgaSetMode SetMode
#endif



/****************************************************************************\
*
* Function:     void CALLING prevr(void)
*
* Description:  Timer callback function - just increments the frame counter
*               and flashes the border
*
\****************************************************************************/

void MIDAS_CALL prevr(void)
{
    frameCount++;

    if ( border >= 0 )
    {
        vgaSetBorder(border);
        border -= 15;
    }
}




/****************************************************************************\
*
* Function:     void MIDASerror(void)
*
* Description:  Displays most recent MIDAS error and exits to DOS
*
\****************************************************************************/

void MIDASerror(void)
{
    vgaSetMode(3);
    printf("MIDAS error: %s\n", MIDASgetErrorMessage(MIDASgetLastError()));
#ifndef NOMUSIC
    MIDASclose(); /*!!!!!*/
#endif
    exit(EXIT_FAILURE);
}




/****************************************************************************\
*
* Function:     void WaitFrame(void)
*
* Description:  Waits for the next frame
*
\****************************************************************************/

void WaitFrame(void)
{
    unsigned    old;

#ifdef NOMUSIC
    vgaWaitNoVR();
    vgaWaitVR();
#else

    /* Playing music - wait for frame counter to change: */
    old = frameCount;
    while ( old == frameCount );
#endif
}




/****************************************************************************\
*
* Function:     void UpdateInfo(void)
*
* Description:  Updates song playing information (defined at the beginning of
*               this file)
*
* Note:         To use the position information from Assembler, do the
*               following (almost - get the idea?)
*
*               INCLUDE "midas.inc"
*               ...
*               GLOBAL  UpdateInfo : LANG
*               ...
*               EXTRN   position : dword
*               EXTRN   pattern : dword
*               EXTRN   row : dword
*               EXTRN   syncInfo : dword
*               ...
*               CODESEG
*               ...
*               PROC    SuperRoutine    NEAR
*               ...
*               call    UpdateInfo C
*               cmp     [position],17
*               je      @@design
*               ...
*
\****************************************************************************/

void MIDAS_CALL UpdateInfo(void)
{
    /* MIDAS_CALL is cdecl for Watcom, empty for DJGPP. Helps calling this
       from assembler, otherwise unnecessary */
    
    static MIDASplayStatus status;

    /* Get playback status: */
    if ( !MIDASgetPlayStatus(playHandle, &status) )
        MIDASerror();

    /* Store interesting information in easy-to-access variables: */
    position = status.position;
    pattern = status.pattern;
    row = status.row;
    syncInfo = status.syncInfo;
}



/****************************************************************************\
*
* Function:     void SyncCallback(unsigned syncNum, unsigned position,
*                   unsigned row);
*
* Description:  Music synchronization callback function. Called by MIDAS
*               whenever command 'W' is encountered (XMs and S3Ms).
*
* Input:        unsigned syncNum        synchronization command infobyte
*               unsigned position       current position
*               unsigned row            current row
*
* Notes:        This function is called from inside the music player timer
*               interrupt! Therefore it may not take very much time (more
*               than a few rasterlines maximum) and shouldn't do really
*               much anything...
*
\****************************************************************************/

void MIDAS_CALL SyncCallback(unsigned syncNum, unsigned position, unsigned row)
{
    /* Prevent warnings: */
    position = position;
    row = row;

    /* Check if the infobyte is interesting - do something only when command
       "W42" is encountered: */
    if ( syncNum == 0x42 )
    {
        /* Yeah, yeah, flash the border! */
        border = 15;
        /* The timer will set the border color */
    }
}





int main(int argc, char *argv[])
{
    static unsigned refreshRate;        /* Display refresh rate */

    /* Unbuffer stdout: */
    setbuf(stdout, NULL);

    /* Prevent warnings: */
    argc = argc;
    argv = argv;

#ifndef NOMUSIC
    /* Call this first: */
    MIDASstartup();

    /* Try to detect the sound card if we should, otherwise run config
       right away. Also, if something was on the commend line, run config. */
#ifdef ATTEMPT_DETECT
    if ( (argc > 1) || (!MIDASdetectSoundCard()) )
#endif
    {
        if ( !MIDASconfig() )
        {
            /* Configuration failed. Check if it is an error - if yes, report,
               otherwise exit normally: */
            if ( MIDASgetLastError() )
            {
                MIDASerror();
            }
            else
            {
                printf("User exit!\n");
                return 1;
            }
        }
    }
#endif /* #ifndef NOMUSIC */    

    /* Here we could set up a cool display mode */
    vgaSetMode(0x03);
    /* But we don't */

#ifndef NOMUSIC
    /* Get display refresh rate: */
    refreshRate = MIDASgetDisplayRefreshRate();

    /* Now we could return to text mode for setup and stuff */    

    /* Check if we got the refresh rate: */
    if ( refreshRate == 0 )
    {
        printf("Warning! Unable to synchronize to display refresh!\n"
               "This can lead to problems in music output and flickering.\n"
               "This usually happens only when running under Windows 95 - "
               "please consider\nrunning DOS programs in DOS instead.\n"
               "Press Esc to quit or any other key to continue\n");
        if ( getch() == 27 )
            return 0;
	refreshRate = 70000; /* default 70Hz */
    }

    /* Now initialize MIDAS Digital Audio System: */

    puts("Initializing MIDAS Digital Audio System");

    if ( !MIDASinit() )
        MIDASerror();

    puts("Loading music");

    if ( (module = MIDASloadModule(moduleName)) == NULL )
        MIDASerror();
#endif

    /* Now we could do all our initialization, set up a fancy display mode
       etc. */

#ifndef NOMUSIC
    /* Now set up our timer callback. If we got the display refresh rate
       properly, prevr() will get called just before each Vertical Retrace.
       If not, well, at least it gets called 70 times per second. */
    if ( !MIDASsetTimerCallbacks(refreshRate, TRUE, &prevr, NULL, NULL) )
        MIDASerror();

    /* Start playing the module: */
    if ( (playHandle = MIDASplayModule(module, TRUE)) == 0 )
        MIDASerror();

    /* Set the music synchronization callback function: */
    if ( !MIDASsetMusicSyncCallback(playHandle, &SyncCallback) )
        MIDASerror();
#endif

    /* Now run a super demo:

       InitCredits();
       do
       {
           RunCredits();
           UpdateInfo();
       } while ( position < 2 );

       InitVectors();
       do
           RunVectors();
           UpdateInfo();
       } while ( position < 13 );

       Or just wait for a keypress: */

    while ( !kbhit() )
    {
        UpdateInfo();
        WaitFrame();
        printf("Pos %02X, Patt %02X, Row %02X", position,
            pattern, row);
        if ( syncInfo != -1 )
            printf(", Sync %02X\r", syncInfo);
        else
            printf("\r");
    }

    getch();

    /* Uninitialization */

#ifndef NOMUSIC
    /* Remove music sync callback: */
    if ( !MIDASsetMusicSyncCallback(playHandle, NULL) )
        MIDASerror();
    
    /* Stop playing module: */
    if ( !MIDASstopModule(playHandle) )
        MIDASerror();

    /* Deallocate the module: */
    if ( !MIDASfreeModule(module) )
        MIDASerror();

    /* Remove timer callback: */
    if ( !MIDASremoveTimerCallbacks() )
        MIDASerror();

    /* And close MIDAS: */
    if ( !MIDASclose() )
        MIDASerror();
#endif

    /* End of DEMO - DOS! */

    return 0;
}

