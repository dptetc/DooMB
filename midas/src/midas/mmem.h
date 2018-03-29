/*      MMEM.H
 *
 * MIDAS Digital Audio System memory handling routines
 *
 * $Id: mmem.h,v 1.4.2.1 1997/09/22 14:48:41 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/


#ifndef __MEM_H
#define __MEM_H


#ifdef __cplusplus
extern "C" {
#endif



/****************************************************************************\
*
* Function:     int memAlloc(unsigned len, void **blk);
*
* Description:  Allocates a block of conventional memory
*
* Input:        unsigned len            Memory block length in bytes
*               void **blk              Pointer to memory block pointer
*
* Returns:      MIDAS error code.
*               Pointer to allocated block stored in *blk, NULL if error.
*
\****************************************************************************/

#ifdef DEBUG

#define memAlloc(len, blk) _memAlloc(len, blk, __FILE__, __LINE__)
int CALLING _memAlloc(unsigned len, void **blk, char *file, unsigned line);

#else
int CALLING memAlloc(unsigned len, void **blk);
#endif



/****************************************************************************\
*
* Function:     int memFree(void *blk);
*
* Description:  Deallocates a memory block allocated with memAlloc()
*
* Input:        void *blk               Memory block pointer
*
* Returns:      MIDAS error code.
*
\****************************************************************************/

int CALLING memFree(void *blk);




/****************************************************************************\
*       enum memFunctIDs
*       ----------------
* Description:  ID numbers for memory handling functions
\****************************************************************************/

enum memFunctIDs
{
    ID_memAlloc = ID_mem,
    ID_memFree
};




#ifdef __cplusplus
}
#endif

#endif


/*
 * $Log: mmem.h,v $
 * Revision 1.4.2.1  1997/09/22 14:48:41  pekangas
 * Added debug-allocation routines
 *
 * Revision 1.4  1997/07/31 10:56:52  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.3  1997/05/23 15:07:48  jpaana
 * Fixed a warning with -pedantic
 *
 * Revision 1.2  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.1  1996/05/22 20:49:33  pekangas
 * Initial revision
 *
*/
