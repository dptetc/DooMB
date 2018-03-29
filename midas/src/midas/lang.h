/*      lang.h
 *
 * Destination language dependent macros and conditional compilation
 *
 * $Id: lang.h,v 1.20 1997/07/31 10:56:42 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __LANG_H
#define __LANG_H


#if defined(__WATCOMC__) || defined(__WC32__)
    #define __WC32__
    #define CALLING __cdecl
    #define GLOBALVAR
    #define __C__
    #define __PROTMODE__
    #define __32__
    #define NOEMS
    #define __DPMI__
    #define __FLATMODE__
    #define EMPTYARRAY
    #define __LITTLEENDIAN__

    #ifdef __cplusplus
        /* Throw a couple of Watcom C++ warnings out of our way to level 9
           - I have no idea how to work around these in the code while
           maintaining Visual C compatibility */
        #pragma warning 604 9
        #pragma warning 594 9
    #endif
#else
#if defined(_MSC_VER) || defined(__VC32__)
    #define __VC32__
    #define CALLING cdecl
    #define GLOBALVAR
    #define __C__
    #define __PROTMODE__
    #define __32__
    #define NOEMS
    #define __DPMI__
    #define __FLATMODE__
    #define __WIN32__
    #define __LITTLEENDIAN__

    /* Disable warning about 0-sized arrays in structures: */
    #pragma warning(disable:4200)

    #define EMPTYARRAY
#else
#if defined(__linux__) || defined(__LINUX__)
    #define __UNIX__
    #define __LINUX__
    #define CALLING
    #define GLOBALVAR
    #define __C__
    #define __PROTMODE__
    #define __32__
    #define NOEMS
    #define __FLATMODE__
    #define NOTIMER
    #define M_HAVE_THREADS
    #define __LITTLEENDIAN__

    #define EMPTYARRAY 0
#else
#if defined(__hpux__) || defined(__HPUX__)
    #define __HPUX__
    #define __UNIX__
    #define CALLING
    #define GLOBALVAR
    #define __C__
    #define __PROTMODE__
    #define __32__
    #define NOEMS
    #define __FLATMODE__
    #define NOTIMER
    #undef  M_HAVE_THREADS
    #define __BIBENDIAN__

    #define EMPTYARRAY 0
#else
#ifdef __DJGPP__
    #define CALLING
    #define GLOBALVAR
    #define __C__
    #define __PROTMODE__
    #define __32__
    #define NOEMS
    #define __DOS__
    #define __DPMI__
    #define __LITTLEENDIAN__

    #define EMPTYARRAY 0
#else
    #error NO COMPILER DEFINED!
#endif
#endif
#endif
#endif
#endif

#ifdef __16__
    typedef unsigned char U8;
    typedef signed char S8;
    typedef unsigned short U16;
    typedef signed short S16;
    typedef unsigned long U32;
    typedef signed long S32;
    typedef unsigned int UINT;
    typedef signed int SINT;
#else
    typedef unsigned char U8;
    typedef signed char S8;
    typedef unsigned short U16;
    typedef signed short S16;
    typedef unsigned long U32;
    typedef signed long S32;
    typedef unsigned int UINT;
    typedef signed int SINT;
#endif

#ifndef M_LITE
    #define SUPPORTSTREAMS
#endif

#if defined(__WINDOWS__) || defined(__NT__) || defined(__WIN32__)
    #define __WIN32__
    #define NOTIMER
    #define M_SD_HAVE_SUSPEND
    #define M_HAVE_THREADS
#endif

#define RCSID(x) x

#ifndef __HPUX__
#define M_X86_ASSEMBLER
#endif

/* Black magic */

#ifdef __LITTLEENDIAN__
#define LE16(x) x
#define LE32(x) x
#define BE16(x) ( ((x << 8) & 0xFF00) | ( (x >> 8) & 0x00FF) )
#define BE32(x) ( (((unsigned)x << 24) & 0xFF000000) | ( (x << 8) & 0x00FF0000 ) | ( (x >> 8) & 0x0000FF00) | ( ((unsigned)x >> 24) & 0x000000FF) )

#else  /* __BIGENDIAN__ */
#define LE16(x) ( (( x << 8) & 0xFF00) | ( ( x >> 8) & 0x00FF) ) 
#define LE32(x) ( (((unsigned)x << 24) & 0xFF000000) | ( (x << 8) & 0x00FF0000 ) | ( (x >> 8) & 0x0000FF00) | ( ((unsigned)x >> 24) & 0x000000FF) )
#define BE16(x) x
#define BE32(x) x
#endif


#endif


/*
 * $Log: lang.h,v $
 * Revision 1.20  1997/07/31 10:56:42  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.19  1997/06/10 23:01:37  jpaana
 * Added some (unsigned)'s
 *
 * Revision 1.18  1997/06/10 19:55:26  jpaana
 * Added HP-UX
 *
 * Revision 1.17  1997/06/04 15:24:22  pekangas
 * Added a #define M_X86_ASSEMBLER
 *
 * Revision 1.16  1997/06/02 00:54:14  jpaana
 * Changed most __LINUX__ defines to __UNIX__ for generic *nix porting
 *
 * Revision 1.15  1997/06/01 14:06:47  jpaana
 * Preliminary support for big-endian machines
 *
 * Revision 1.14  1997/05/03 15:10:49  pekangas
 * Added stream support for DOS, removed GUS Sound Device from non-Lite
 * build. M_HAVE_THREADS now defined in threaded environment.
 *
 * Revision 1.13  1997/02/27 16:01:48  pekangas
 * Added DJGPP support
 *
 * Revision 1.12  1997/02/19 20:43:40  pekangas
 * M_SD_HAVE_SUSPEND now defined for Win32
 *
 * Revision 1.11  1997/02/06 12:45:06  pekangas
 * Now figures out the compiler type and target without magic command line
 * defined macros
 *
 * Revision 1.10  1997/02/05 17:40:41  pekangas
 * Changed to new makefile structure. Removed old makefiles, library
 * command files and related junk. Fixed some double linefeeds caused
 * by RCS-CVS transition. lang.h now defined NOTIMER for Win32.
 *
 * Revision 1.9  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.8  1996/09/28 08:12:40  jpaana
 * Enabled SUPPORTSTREAMS for Linux
 *
 * Revision 1.7  1996/09/01 19:03:52  pekangas
 * Removed a couple of warnings from Watcom C++ in C++ mode
 *
 * Revision 1.6  1996/07/16 20:21:00  pekangas
 * Added support for Visual C
 *
 * Revision 1.5  1996/07/13 17:29:08  pekangas
 * Removed cdecl calling convention redefinition pragma
 *
 * Revision 1.4  1996/05/26 16:44:22  pekangas
 * Defined SUPPORTSTREAMS and __WIN32__ when compiling for NT
 *
 * Revision 1.3  1996/05/24 16:58:22  pekangas
 * Added #define EMPTYARRAY - used to declare empty arrays at the end of a structure: type array[EMPTYARRAY] to gain portability.
 *
 * Revision 1.2  1996/05/24 16:20:36  jpaana
 * Added __LINUX__
 *
 * Revision 1.1  1996/05/22 20:49:33  pekangas
 * Initial revision
 *
*/
