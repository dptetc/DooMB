/*      mprof.h
 *
 * MIDAS internal profiling support
 *
 * $Id: mprof.h,v 1.2.2.3 1997/08/25 19:33:10 pekangas Exp $
 *
 * Copyright 1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __mprof_h
#define __mprof_h


/* Total number of sections: */
#define M_PROF_NUM_SECTIONS 0x200

/* Section number starts for different system areas: */
enum mProfSectionNumberStarts
{
    M_PROF_NONE = 0,
    M_PROF_COMMON = 0x10,
    M_PROF_GMPLAYER = 0x20,
    M_PROF_ADPCM = 0x40,
    M_PROF_DSM = 0x100
};



/****************************************************************************\
*       struct mProfSection
*       -------------------
* Description:  A profiling code section
\****************************************************************************/

typedef struct
{
    unsigned    numCalls;               /* number of calls for the section */
    unsigned    totalCyclesLow, totalCyclesHigh; /* total number of cycles */
    unsigned    totalRounds;            /* total number of rounds */
    unsigned    minRounds;              /* minimum number of rounds per call */
    unsigned    maxRounds;              /* maximum number of rounds per call */
    unsigned    minCyclesPerRound;      /* minimum number of cycles per round */
    unsigned    maxCyclesPerRound;      /* maximum number of cycles per round */
    unsigned    curRounds;
    unsigned    startCyclesLow, startCyclesHigh;
    unsigned    filler[5];              /* total size: 64 bytes */
} mProfSection;



extern mProfSection* GLOBALVAR mProfSections;




/****************************************************************************\
*       The profiling macros
\****************************************************************************/

#ifdef M_PROF
#  define M_PROF_START(num, rounds) mProfStartSection(num, rounds)
#  define M_PROF_END(num) mProfEndSection(num)
#  define M_PROF_END_ROUNDS(num, rounds) mProfEndSectionRounds(num, rounds)
#else
#  define M_PROF_START(num, rounds)
#  define M_PROF_END(num)
#  define M_PROF_END_ROUNDS(num, rounds)
#endif




#ifdef __cplusplus
extern "C" {
#endif



/****************************************************************************\
*
* Function:     int mProfInit(void)
*
* Description:  Initializes profiling support
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mProfInit(void);




/****************************************************************************\
*
* Function:     int mProfClose(void)
*
* Description:  Uninitializes profiling support
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING mProfClose(void);




/****************************************************************************\
*
* Function:     void mProfAsmStartSection(void)
*
* Description:  Starts a profiling section. WARNING! This function does NOT
*               C calling convention, and should not be called directly!
*
* Input:        eax    section number
*               ecx    number of rounds that will be executed
*
* Returns:      ecx    pointer to the address where the current cycle count
*                      should be stored, low dword first
*
\****************************************************************************/

void CALLING mProfAsmStartSection(void);



/****************************************************************************\
*
* Function:     void mProfAsmEndSection(void);
*
* Description:  Ends a profiling section. WARNING! This function does NOT
*               C calling convention, and should not be called directly!
*
* Input:        eax:edx current cycle count
*               ecx     section number
*
\****************************************************************************/

void CALLING mProfAsmEndSection(void);



/****************************************************************************\
*
* Function:     void mProfAsmEndSectionRounds(void);
*
* Description:  Ends a profiling section with a new number of rounds.
*
* Input:        eax:edx current cycle count
*               ecx     section number
*               ebx     new number of rounds
*
\****************************************************************************/

void CALLING mProfAsmEndSectionRounds(void);



/****************************************************************************\
*
* Function:     void mProfStartSection(unsigned sectionNumber,
*                   unsigned numRounds)
*
* Description:  Starts a profiling section. Normally implemented in inline
*               assembler below - assembler code should use the macros in
*               mprof.inc instead.
*
* Input:        unsigned sectionNumber  the section number
*               unsigned numRounds      number of rounds that will be executed
*
\****************************************************************************/

void mProfStartSection(unsigned sectionNumber, unsigned numRounds);



/****************************************************************************\
*
* Function:     void mProfEndSection(unsigned sectionNumber)
*
* Description:  Ends a profiling section. Normally implemented in inline
*               assembler below - assembler code should use the macros in
*               mprof.inc instead.
*
* Input:        unsigned sectionNumber  the section number
*
\****************************************************************************/

void mProfEndSection(unsigned sectionNumber);




/****************************************************************************\
*
* Function:     void mProfEndSectionRounds(unsigned sectionNumber,
*                   unsigned rounds)
*
* Description:  Ends a profiling section with a new number of rounds.
*
* Input:        unsigned sectionNumber  the section number
*               unsigned rounds         updated number of rounds
*
\****************************************************************************/

void mProfEndSectionRounds(unsigned sectionNumber, unsigned rounds);



#ifdef __cplusplus
}
#endif



/****************************************************************************\
*
*       Inline assembler core for starting/ending profiling sections
*
\****************************************************************************/

#ifdef __WATCOMC__

#pragma aux mProfStartSection = \
    ".586" \
    "call mProfAsmStartSection" \
    "rdtsc" \
    "mov [ecx],eax" \
    "mov [ecx+4],edx" \
    parm [eax] [ecx] \
    modify exact [eax ecx edx];

#pragma aux mProfEndSection = \
    ".586" \
    "rdtsc" \
    "call mProfAsmEndSection" \
    parm [ecx] \
    modify exact [eax ecx edx];

#pragma aux mProfEndSectionRounds = \
    ".586" \
    "rdtsc" \
    "call mProfAsmEndSectionRounds" \
    parm [ecx] [ebx] \
    modify exact [eax ebx ecx edx];

#endif


#ifdef _MSC_VER

__inline void mProfStartSection(unsigned sectionNumber, unsigned numRounds)
{
    __asm {
        mov eax,sectionNumber
        mov ecx,numRounds
        call mProfAsmStartSection
        __emit 0x0f
        __emit 0x31
        mov [ecx],eax
        mov [ecx+4],edx
    }
}


__inline void mProfEndSection(unsigned sectionNumber)
{
    __asm {
        __emit 0x0f
        __emit 0x31
        mov ecx,sectionNumber
        call mProfAsmEndSection
    }
}



__inline void mProfEndSectionRounds(unsigned sectionNumber, unsigned rounds)
{
    __asm {
        __emit 0x0f
        __emit 0x31
        mov ecx,sectionNumber
        mov ebx,rounds
        call mProfAsmEndSectionRounds
    }
}

#endif


/****************************************************************************\
*       enum mProfFunctIDs
*       ------------------
* Description:  ID numbers for profiling
\****************************************************************************/

enum mProfFunctIDs
{
    ID_mProfInit = ID_echo,
    ID_mProfClose
};



#endif


/*
 * $Log: mprof.h,v $
 * Revision 1.2.2.3  1997/08/25 19:33:10  pekangas
 * Added support for Visual C
 *
 * Revision 1.2.2.2  1997/08/21 18:00:54  pekangas
 * Added functions for setting the number of rounds at the end of the section
 *
 * Revision 1.2.2.1  1997/08/20 12:48:05  pekangas
 * Manually merged fixes from the main trunk (oops)
 *
 * Revision 1.3  1997/08/19 16:17:21  pekangas
 * Fixed a typo
 *
 * Revision 1.2  1997/07/31 10:56:53  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.1  1997/07/23 17:26:49  pekangas
 * Initial revision
 *
*/
