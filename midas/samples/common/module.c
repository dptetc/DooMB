/*      module.c
 *
 * A minimal module playing example with the DLL API
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#include <stdio.h>
#include <stdlib.h>
#include "midasdll.h"

#ifndef __linux__
#include <conio.h>
#endif


int main(void)
{
    MIDASmodule module;
    MIDASmodulePlayHandle playHandle;

    /* Error checking has been removed for clarity - see other API examples */

    /* Initialize MIDAS and start background playback: */
    MIDASstartup();
    MIDASinit();
    MIDASstartBackgroundPlay(0);

    /* Load the module and start playing: */
#ifdef __linux__    
    module = MIDASloadModule("../data/templsun.xm");
#else    
    module = MIDASloadModule("..\\data\\templsun.xm");
#endif
    playHandle = MIDASplayModule(module, TRUE);

    puts("Playing - press any key");

#ifdef __linux__
    getchar();
#else
    getch();
#endif

    /* Stop playing and deallocate module: */
    MIDASstopModule(playHandle);
    MIDASfreeModule(module);

    /* Stop background playback and uninitialize MIDAS: */
    MIDASstopBackgroundPlay();
    MIDASclose();

    return 0;
}
