#*      win32cmd.mak
#*
#* Win32-specific command definitions
#*
#* $Id: win32cmd.mak,v 1.4.2.2 1997/11/18 19:38:39 pekangas Exp $
#*
#* Copyright 1997 Housemarque Inc.
#*
#* This file is part of MIDAS Digital Audio System, and may only be
#* used, modified and distributed under the terms of the MIDAS
#* Digital Audio System license, "license.txt". By continuing to use,
#* modify or distribute this file you indicate that you have
#* read the license and understand and accept it fully.
#*


ifdef _CYGWIN32
  RM = rm
  CP = cp
  REM = true
  MKDIR = mkdir
  RMDIR = rmdir
else
  RM = del
  CP = copy
  REM = rem
  MKDIR = md
  RMDIR = rd
endif

ifdef _SHELL_BKSLASH
COPYFILE = $(CP) $(subst /,\,$^)  $(subst /,\,$@)
else
COPYFILE = $(CP) $^  $@
endif


#* $Log: win32cmd.mak,v $
#* Revision 1.4.2.2  1997/11/18 19:38:39  pekangas
#* Added support for cygwin32 cp
#*
#* Revision 1.4.2.1  1997/10/05 13:16:33  pekangas
#* Fixed to work with Cygnus Win32 make
#*
#* Revision 1.4  1997/07/31 10:56:34  pekangas
#* Renamed from MIDAS Sound System to MIDAS Digital Audio System
#*
#* Revision 1.3  1997/07/29 11:57:31  pekangas
#* Fixed COPYFILE command
#*
#* Revision 1.2  1997/02/05 22:53:55  pekangas
#* Several small changes to get the new make script system work better
#*
#* Revision 1.1  1997/02/05 17:33:43  pekangas
#* Initial revision
#*