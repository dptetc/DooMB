/*      midasrc.h
 *
 * MIDAS Digital Audio System .midasrc-file configuration functions
 *
 * $Id: midasrc.h,v 1.3 1997/07/31 10:56:49 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

enum
{
    noSec = 0,
    optSec,
    effSec,
    modSec,
    echoDef,
    modDef
}secTypes;

typedef struct
{
    int         token;
    char        *id;
}section;

typedef enum
{
    optNone = 0,
    optMixRate,
    optStereo,
    optBits,
    optInterpolation,
    optSeparation,
    optLoop,
    optEcho
}optionDef;

typedef struct effect
{
    struct effect       *next;
    char                *id;
    MIDASechoSet        *echoset;
}effect;

typedef struct option
{
    struct option       *next;
    optionDef           opt;
    union               valas
    {
        int             value;
        char            *str;
    }val;
}option;

typedef struct optlist
{
    struct optlist      *next;
    char                *id;
    option              *options;
}optlist;


#ifdef __cplusplus
extern "C" {
#endif
	

/****************************************************************************\
*
* Function:     int mrcReadFile(char *fileName)
*
* Description:  Reads the file specified and parses it as a MIDAS RC file
*
* Input:        char *fileName          RC file name
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int mrcReadFile(char *fileName);


MIDASechoSet *mrcFindEcho(char *name);
MIDASechoSet *mrcMatchFilename(char *name);

#ifdef __cplusplus
}
#endif
/*
 * $Log: midasrc.h,v $
 * Revision 1.3  1997/07/31 10:56:49  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.2  1997/07/17 09:54:11  pekangas
 * Removed a about half a thousand empty lines from the end
 *
 * Revision 1.1  1997/07/15 18:23:14  jpaana
 * Initial revision
 *
 *
*/
