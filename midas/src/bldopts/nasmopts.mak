#*      nasmopts.mak
#*
#* Netwide Assembler -specific macros
#*
#* $Id: nasmopts.mak,v 1.6 1997/07/31 10:56:34 pekangas Exp $
#*
#* Copyright 1997 Housemarque Inc.
#*
#* This file is part of MIDAS Digital Audio System, and may only be
#* used, modified and distributed under the terms of the MIDAS
#* Digital Audio System license, "license.txt". By continuing to use,
#* modify or distribute this file you indicate that you have
#* read the license and understand and accept it fully.
#*


# Define the programs:
NASM = nasm


# Set target format and target define:

ifdef _WIN32_TARGET
  ifdef _WC
    NASMOPTS = -f obj -d M_TARGET_WATCOM
  else
    NASMOPTS = -f win32 -d M_TARGET_VISUALC
  endif
endif

ifdef _DOS_TARGET
  ifdef _DJGPP_TARGET
    NASMOPTS = -f coff -d M_TARGET_DJGPP
  else
    NASMOPTS = -f obj -d M_TARGET_WATCOM
  endif
endif

ifdef _LINUX_TARGET
  NASMOPTS = -f elf -d M_TARGET_LINUX
endif


# Add M_PROF %define for profiling build to enable profiling support:
ifdef _PROF
  NASMOPTS += -d M_PROF
endif



# Commands for compiling nasm source files:
COMPILENASM = $(NASM) -o $(FIXOBJDIR)$@ $(NASMOPTS) $<



#* $Log: nasmopts.mak,v $
#* Revision 1.6  1997/07/31 10:56:34  pekangas
#* Renamed from MIDAS Sound System to MIDAS Digital Audio System
#*
#* Revision 1.5  1997/07/23 17:28:10  pekangas
#* Added profiling build options
#*
#* Revision 1.4  1997/07/22 13:45:44  pekangas
#* Added support for keeping object modules in a different directory
#* from the source
#*
#* Revision 1.3  1997/07/17 19:34:52  pekangas
#* Added target type defines to nasm command line
#*
#* Revision 1.2  1997/06/10 19:58:02  jpaana
#* We build elf on Linux, a.out is obsolete
#*
#* Revision 1.1  1997/06/04 15:27:17  pekangas
#* Initial revision
#*


