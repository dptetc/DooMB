/*      MTYPES.H
 *
 * MIDAS Digital Audio System common type definitions
 *
 * $Id: mtypes.h,v 1.7 1997/07/31 10:56:53 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#ifndef __MTYPES_H
#define __MTYPES_H

typedef unsigned char uchar;
#ifndef _LINUX_TYPES_H
#ifndef _SYS_TYPES_H
typedef unsigned short ushort;
typedef unsigned long ulong;
#endif
#endif

/* Get NULL safely: */
#ifndef NULL
#include <stdio.h>
#endif
/* (OK, so this is not really sensible, and will #include a lot of useless
   cruft, but apparently the only remotely portable solution) */

#endif


/*
 * $Log: mtypes.h,v $
 * Revision 1.7  1997/07/31 10:56:53  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.6  1997/05/23 15:08:13  jpaana
 * Fixed warnings with already defined types
 *
 * Revision 1.5  1997/02/27 16:05:07  pekangas
 * Finally fixed NULL problems?
 *
 * Revision 1.4  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.3  1996/09/21 17:40:45  jpaana
 * Fixed ushort and ulong warnings on Linux
 *
 * Revision 1.2  1996/05/25 15:49:57  jpaana
 * Nothing really
 *
 * Revision 1.1  1996/05/22 20:49:33  pekangas
 * Initial revision
 *
*/
