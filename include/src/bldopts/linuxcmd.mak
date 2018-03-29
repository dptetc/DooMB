#*      doscmd.mak
#*
#* Linux-specific command definitions
#*
#* $Id: linuxcmd.mak,v 1.2 1997/07/31 10:56:34 pekangas Exp $
#*
#* Copyright 1997 Housemarque Inc.
#*
#* This file is part of MIDAS Digital Audio System, and may only be
#* used, modified and distributed under the terms of the MIDAS
#* Digital Audio System license, "license.txt". By continuing to use,
#* modify or distribute this file you indicate that you have
#* read the license and understand and accept it fully.
#*

RM = rm
CP = cp
REM = true
MKDIR = mkdir
RMDIR = rmdir

COPYFILE = $(CP) $^ $@


#* $Log: linuxcmd.mak,v $
#* Revision 1.2  1997/07/31 10:56:34  pekangas
#* Renamed from MIDAS Sound System to MIDAS Digital Audio System
#*
#* Revision 1.1  1997/02/07 15:16:17  pekangas
#* Initial revision
#*

