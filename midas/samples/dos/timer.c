/*      timer.c
 *
 * MIDAS Timer example
 *
 * Copyright 1997 Housemarque Inc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include "midasdll.h"
#include "vga.h"

#ifdef __DJGPP__
#include <dpmi.h>
#include <sys/movedata.h>
#endif


/* Frame counter variable - note VOLATILE! */
volatile unsigned   frameCount;



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
* Function:     void MIDAS_CALL TimerCallback(void)
*
* Description:  Timer callback function - just increments the frame counter
*
\****************************************************************************/

void MIDAS_CALL TimerCallback(void)
{
    frameCount++;
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




int main(void)
{
    unsigned char *buffer;
    unsigned    i;
    int         x, y, xx, yy;
    unsigned char *b;
    unsigned    prevCount, moveNow;
    float       xa, ya, xaa, yaa;
    
    /* Initialize MIDAS: */
    MIDASstartup();
    if ( !MIDASinit() )
        MIDASerror();

    /* Go to mode 13h: (320x200 256-color) */
    vgaSetMode(0x13);

    buffer = malloc(320*200);

    /* Create a cool (ha) palette: */
    outp(0x03C8, 0);
    for ( i = 0; i < 64; i++ )
    {
        outp(0x3C9, i); outp(0x3C9, 0); outp(0x3C9, 0);
    }
    for ( i = 64; i < 128; i++ )
    {
        outp(0x3C9, 0); outp(0x3C9, 0); outp(0x3C9, i-64);
    }
    for ( i = 128; i < 192; i++ )
    {
        outp(0x3C9, 0); outp(0x3C9, i-128); outp(0x3C9, 0);
    }
    for ( i = 192; i < 256; i++ )
    {
        outp(0x3C9, i-192); outp(0x3C9, 0); outp(0x3C9, i-192);
    }

    /* Set timer callback, run it at 200Hz, without display sync: */
    if ( !MIDASsetTimerCallbacks(200000, FALSE, &TimerCallback, NULL, NULL) )
        MIDASerror();

    /* Run the cool effect: */
    prevCount = frameCount; xa = ya = 0.0;
    while ( !kbhit() )
    {
        /* Make sure we won't go past our framerate: */
        while ( prevCount == frameCount );        

        /* Store the frame counter, and check how many frames worth of movement
           we should do this time around: */
        moveNow = frameCount - prevCount;
        prevCount = frameCount;
        /* (it might actually be a good idea to disable the interrupts for
           the two lines above, but I'm too lazy here) */

        /* Render it first: */
        b = buffer; yaa = ya;
        for ( y = 0; y < 200; y++ )
        {
            xaa = xa + sin(yaa);
            xx = (int) (3.0 * sin (yaa));
            for ( x = 0; x < 320; x++ )
            {
                yy = (int) (2.5 * cos(xaa));
                *(b++) = ((x + xx) | (y + yy)) & 255;
                xaa += 0.17;
            }
            yaa += 0.17;
        }

        /* Blit to screen: */
#ifdef __DJGPP__
        dosmemput(buffer, 320*200, 0xA0000);
#else
        memcpy((void*) 0xA0000, buffer, 320*200);
#endif

        /* Do the movement: */
        while ( moveNow )
        {
            xa += 0.017;
            ya += 0.017;
            moveNow--;
        }
    }

    getch();

    /* Close MIDAS: */
    if ( !MIDASclose() )
        MIDASerror();

    /* Get lost: */
    vgaSetMode(3);
    printf("You expected something? :)\n");
    return 0;
}

