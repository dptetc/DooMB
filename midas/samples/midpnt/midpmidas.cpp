/*
 *      MidpMIDAS.cpp
 *
 * MIDAS Module Player for Windows NT MIDAS interface module
 *
 * $Id: midpmidas.cpp,v 1.12 1997/07/31 14:26:09 pekangas Exp $
 *
 * Copyright 1996 Petteri Kangaslampi
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <ddeml.h>
#include <process.h>
#include <stdio.h>
#include "midasdll.h"
#include "MidpNT.h"

#define POLLRATE 50

int             mixRate = 44100, stereo = 1, force8bit = 0;
static volatile int backgroundPlay = 0;




/****************************************************************************\
*
* Function:     void MIDASerror(void)
*
* Description:  Gets the last MIDAS error, displays it, and exits
*
\****************************************************************************/

void MIDASerror(void)
{
    char        *errorMessage;

    errorMessage = MIDASgetErrorMessage(MIDASgetLastError());
    MessageBox(NULL, errorMessage, "MIDAS Error!",
        MB_APPLMODAL | MB_ICONSTOP | MB_OK);
    if ( ddeInit )
        DdeUninitialize(ddeInstance);
    MIDASclose();
    ExitProcess(1);
    while(1);                           // we should NEVER end up here
}




/****************************************************************************\
*
* Function:     void StopFreeModule(MIDASmodulePlayHandle playHandle,
*                                   MIDASmodule module);
*
* Description:  Stops playing a module and deallocates it
*
* Input:        MIDASmodulePlayHandle playHandle  playing handle for the
*                                                 module if it's being played
*               MIDASmodule module      the module to deallocate
*
\****************************************************************************/

void StopFreeModule(MIDASmodulePlayHandle playHandle, MIDASmodule module)
{
    if ( module == NULL )
        return;

    if ( playHandle )
    {
        AddTextLine("Stopping module");
        if ( !MIDASstopModule(playHandle) )
            MIDASerror();
    }

    AddTextLine("Deallocating module");
    if ( !MIDASfreeModule(module) )
        MIDASerror();

    SendMessage(mainWinHandle, WM_SETTEXT, 0,
        (LPARAM) "MIDAS Module Player for Windows NT");
}




/****************************************************************************\
*
* Function:     MIDASmodule LoadModule(char *fileName);
*
* Description:  Loads a module file
*
* Input:        char *fileName          module file name
*
* Returns:      Pointer to gmpModule structure for the loaded module,
*               NULL if loading failed.
*
\****************************************************************************/

MIDASmodule LoadModule(char *fileName)
{
    MIDASmodule module;
    char        *errorMessage;

    AddTextLine("Loading Module");

    SendMessage(mainWinHandle, WM_SETTEXT, 0, (LPARAM) "MidpNT - Loading...");

    if ( (module = MIDASloadModule(fileName)) == NULL )
    {
        errorMessage = MIDASgetErrorMessage(MIDASgetLastError());
        MessageBox(NULL, errorMessage, "Module load failure",
            MB_APPLMODAL | MB_ICONSTOP | MB_OK);
        return NULL;
    }

    return module;
}




/****************************************************************************\
*
* Function:     MIDASmodulePlayHandle PlayModule(MIDASmodule module);
*
* Description:  Starts playing a module
*
* Input:        MIDASmodule module      the module
*
* Returns:      MIDAS module playing handle for the module.
*
\****************************************************************************/

MIDASmodulePlayHandle PlayModule(MIDASmodule module)
{
    char        buf[64];
    MIDASmoduleInfo moduleInfo;
    MIDASmodulePlayHandle playHandle;

    if ( (playHandle = MIDASplayModule(module, TRUE)) == 0 )
        MIDASerror();

    if ( !MIDASgetModuleInfo(module, &moduleInfo) )
        MIDASerror();

    sprintf(buf, "Playing \"%s\" - %i channels", moduleInfo.songName,
        moduleInfo.numChannels);
    AddTextLine(buf);

    sprintf(buf, "MidpNT - %s", moduleInfo.songName);
    SendMessage(mainWinHandle, WM_SETTEXT, 0, (LPARAM) buf);

    return playHandle;
}




/****************************************************************************\
*
* Function:     void InitMIDAS(void)
*
* Description:  Initializes MIDAS Digital Audio System
*
\****************************************************************************/

void InitMIDAS(void)
{
    char message[512];
    
    AddTextLine("Initializing MIDAS Digital Audio System");

    MIDASsetOption(MIDAS_OPTION_FORCE_NO_SOUND, FALSE);

    while ( !MIDASinit() )
    {
        sprintf(message,
                "MIDAS Digital Audio System initialization failed:\n"
                "\"%s\"\n"
                "This usually means that the sound card is being used\n"
                "by another program, or that it is incorrectly configured.\n"
                "Click OK to try again, or Cancel to continue with no sound",
                MIDASgetErrorMessage(MIDASgetLastError()));

        if ( MessageBox(NULL, message, "MidpNT error",
                        MB_APPLMODAL | MB_ICONSTOP | MB_OKCANCEL) == IDCANCEL)
        {
            MIDASsetOption(MIDAS_OPTION_FORCE_NO_SOUND, TRUE);
            if ( !MIDASinit() )
            {
                if ( ddeInit )
                    DdeUninitialize(ddeInstance);
                MIDASerror();
            }
        }
    }

    /* Start polling MIDAS in a thread: */
    if ( !MIDASstartBackgroundPlay(POLLRATE) )
        MIDASerror();

    backgroundPlay = 1;
}




/****************************************************************************\
*
* Function:     void CloseMIDAS(void)
*
* Description:  Uninitializes MIDAS Digital Audio System
*
\****************************************************************************/

void CloseMIDAS(void)
{
    if ( backgroundPlay )
        if ( !MIDASstopBackgroundPlay() )
            MIDASerror();

    if ( !MIDASclose() )
        MIDASerror();
}




/****************************************************************************\
*
* Function:     void StartupMIDAS(void)
*
* Description:  Start up interface to MIDAS Digital Audio System
*
\****************************************************************************/

void StartupMIDAS(void)
{
    MIDASstartup();
}


/*
 * $Log: midpmidas.cpp,v $
 * Revision 1.12  1997/07/31 14:26:09  pekangas
 * Changed InitMIDAS to ask the user to retry or continue with no sound
 * if MIDAS initialization fails
 *
 * Revision 1.11  1997/07/31 10:56:31  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.10  1997/07/12 12:04:47  pekangas
 * Changed references to "MIDAS Digital Audio System" to "MIDAS Digital Audio System"
 *
 * Revision 1.9  1997/05/20 20:44:50  pekangas
 * Added the looping flag to MIDASplayModule()
 *
 * Revision 1.8  1997/05/02 13:21:57  pekangas
 * Changed to work with new module playback functions and keep track of
 * module playing handles
 *
 * Revision 1.7  1997/01/25 13:17:44  pekangas
 * Rewrote archive support
 *
 * Revision 1.6  1997/01/17 00:08:51  pekangas
 * Now puts song name and loading/decompressing messages in main window
 * caption
 *
 * Revision 1.5  1997/01/14 17:42:08  pekangas
 * Changed to use MIDAS DLL API
 *
 * Revision 1.4  1996/08/13 20:22:15  pekangas
 * #included stdio.h as MIDAS rawfile.h no longer does that
 *
 * Revision 1.3  1996/08/02  17:53:13  pekangas
 * Fixed to compile with Watcom C again
 *
 * Revision 1.2  1996/07/16  19:19:37  pekangas
 * Fixed to compile with Visual C, added RCS keywords, changed to LFs
 *
*/
