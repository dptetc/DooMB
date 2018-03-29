/*      dsmod.c
 *
 * A simple example module player using DirectSound
 *
 * Copyright 1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "midasdll.h"


HWND        consoleHwnd = NULL;


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    char txt[256];

    if ( GetWindowText(hwnd, txt, 255) )
    {
        if ( !strcmp(txt, "DirectSound TestiKala") )
        {
            consoleHwnd = hwnd;
            return FALSE;
        }
    }

    return TRUE;
}


void MIDASerror(void)
{
    printf("MIDAS error: %s\n", MIDASgetErrorMessage(MIDASgetLastError()));
    MIDASclose();
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    char        oldTitle[256];
    MIDASmodule module;
    MIDASmodulePlayHandle playHandle;
    char        c;
    int         suspended = 0;

    MIDASstartup();

    printf("\nThe Ultimate MIDAS DirectSound Module Player\n\n");

    if ( argc != 3 )
    {
        printf("Usage: dsmod filename DirectSoundMode\n"
               "\n"
               "DirectSoundMode:   0 - disabled\n"
               "                   1 - stream\n"
               "                   2 - primary\n"
               "                   3 - force stream");
        return 0;
    }

    /* Hack - try to figure out our console window's window handle: */
    GetConsoleTitle(oldTitle, 255);
    SetConsoleTitle("DirectSound TestiKala");
    Sleep(250);
    EnumWindows(&EnumWindowsProc, (LPARAM) NULL);
    SetConsoleTitle(oldTitle);

    printf("%P, %u\n", consoleHwnd, sizeof(HWND));

    if ( consoleHwnd == NULL )
    {
        printf("Panic! Couldn't find console window window handle\n");
        return 0;
    }

    MIDASsetOption(MIDAS_OPTION_DSOUND_HWND, (DWORD) consoleHwnd);
    MIDASsetOption(MIDAS_OPTION_DSOUND_MODE, atoi(argv[2]));

    if ( !MIDASconfig() )
        return 1;

    /* Initialize MIDAS: */
    printf("Initializing MIDAS\n");
    if ( !MIDASinit() )
        MIDASerror();

    printf("Loading module\n");
    if ( (module = MIDASloadModule(argv[1])) == NULL )
        MIDASerror();

    printf("Playing module - space to suspend/resume, esc to exit\n");
    if ( (playHandle = MIDASplayModule(module, TRUE)) == 0 )
        MIDASerror();

    c = 0;
    while ( c != 27 )
    {
	if ( kbhit() )
	{
	    c = getch();
	    if ( c == ' ' )
	    {
		if ( suspended )
		{
		    if ( !MIDASresume() )
			MIDASerror();
		    printf("Resumed\n");
		}
		else
		{
		    if ( !MIDASsuspend() )
			MIDASerror();
		    printf("Suspended\n");
		}
		suspended ^= 1;
	    }
	}
        if ( !MIDASpoll() )
            MIDASerror();
        Sleep(20);
    }

    printf("Closing MIDAS\n");
    if ( !MIDASstopModule(playHandle) )
        MIDASerror();
    if ( !MIDASfreeModule(module) )
        MIDASerror();
    if ( !MIDASclose() )
        MIDASerror();

    return 0;
}
