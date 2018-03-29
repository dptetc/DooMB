/*
 *      MidpNT.h
 *
 * MIDAS Module Player for Windows NT global definitions
 *
 * $Id: midpnt.h,v 1.17 1997/07/31 20:03:56 pekangas Exp $
 *
 * Copyright 1996 Petteri Kangaslampi
*/

#ifndef __MIDPNT_H
#define __MIDPNT_H

#define VERSION 1.0
#define FULLVERSION 1.0.2
#define VERSTR "1.0"
#define FULLVERSTR "1.0.2"

extern HANDLE   instance;
extern int      mixRate, stereo, force8bit;
extern MIDASmodule module;
extern MIDASmodulePlayHandle playHandle;
extern HWND     mainWinHandle;
extern DWORD    ddeInstance;
extern int      ddeInit;
extern int      viewsChildren;
extern int      minimalUI;
extern char     unzipDir[];
extern unsigned filterMode;




/* Custom window messages: */
enum midpWindowMessages
{
    MIDPMSG_SONGCHANGED = WM_USER+200
};



/* An echo effect: */




/****************************************************************************\
*
* Function:     void StartupMIDAS(void)
*
* Description:  Start up interface to MIDAS Digital Audio System
*
\****************************************************************************/

void StartupMIDAS(void);




/****************************************************************************\
*
* Function:     void InitMIDAS(void)
*
* Description:  Initializes MIDAS Digital Audio System
*
\****************************************************************************/

void InitMIDAS(void);




/****************************************************************************\
*
* Function:     void CloseMIDAS(void)
*
* Description:  Uninitializes MIDAS Digital Audio System
*
\****************************************************************************/

void CloseMIDAS(void);



/****************************************************************************\
*
* Function:     MIDASmodule *LoadModule(char *fileName);
*
* Description:  Loads a module file
*
* Input:        char *fileName          module file name
*
* Returns:      MIDAS module handle, NULL if loading failed.
*
\****************************************************************************/

MIDASmodule LoadModule(char *fileName);




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

MIDASmodulePlayHandle PlayModule(MIDASmodule module);




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

void StopFreeModule(MIDASmodulePlayHandle playHandle, MIDASmodule module);




/****************************************************************************\
*
* Function:     void AddTextLine(char *textLine)
*
* Description:  Adds a line of text to main window edit control
*
* Input:        char *textLine          text line
*
\****************************************************************************/

void AddTextLine(char *textLine);



/****************************************************************************\
*
* Function:     void Panic(char *message)
*
* Description:  Exits MidpNT with a "MIDP Panic" message
*
* Input:        char *message           message string
*
\****************************************************************************/

void Panic(char *message);




/****************************************************************************\
*
* Function:     void MIDASerror(void)
*
* Description:  Gets the last MIDAS error, displays it, and exits
*
\****************************************************************************/

void MIDASerror(void);



#endif

/*
 * $Log: midpnt.h,v $
 * Revision 1.17  1997/07/31 20:03:56  pekangas
 * Updated for 1.0.2
 *
 * Revision 1.16  1997/07/31 10:56:31  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.15  1997/07/14 17:51:19  pekangas
 * Changed version to 1.0.1
 *
 * Revision 1.14  1997/07/11 13:37:42  pekangas
 * Boosted version to 1.0.0
 *
 * Revision 1.13  1997/07/10 18:41:27  pekangas
 * Added echo effect editor and echo effect support
 *
 * Revision 1.12  1997/07/08 19:19:50  pekangas
 * Added new MIDAS setup, changed version to 0.3.0 and added a possibility
 * to compile a console version
 *
 * Revision 1.11  1997/05/20 20:45:00  pekangas
 * Added filter toggle
 *
 * Revision 1.10  1997/05/02 13:21:57  pekangas
 * Changed to work with new module playback functions and keep track of
 * module playing handles
 *
 * Revision 1.9  1997/02/19 10:09:33  pekangas
 * Updated version number
 *
 * Revision 1.8  1997/01/25 13:17:01  pekangas
 * Rewrote archive support, now much better
 *
 * Revision 1.7  1997/01/17 00:08:39  pekangas
 * Added minimalUI flag, changed version to 0.2.1
 *
 * Revision 1.6  1997/01/14 17:42:08  pekangas
 * Changed to use MIDAS DLL API
 *
 * Revision 1.5  1996/10/14 20:42:38  pekangas
 * Changed version to 0.1.7
 *
 * Revision 1.4  1996/09/25  18:43:46  pekangas
 * Changed version number to 0.1.6
 *
 * Revision 1.3  1996/09/13  06:48:38  pekangas
 * Changed version number to 0.1.5
 *
 * Revision 1.2  1996/09/01  20:57:40  pekangas
 * Changed MidpNT revision number, converted to LFs, added RCS keywords
 *
*/
