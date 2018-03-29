#*      tasmopts.mak
#*
#* Turbo Assembler -specific macros
#*
#* $Id: tasmopts.mak,v 1.6.2.1 1997/10/05 12:33:04 pekangas Exp $
#*
#* Copyright 1997 Housemarque Inc.
#*
#* This file is part of MIDAS Digital Audio System, and may only be
#* used, modified and distributed under the terms of the MIDAS
#* Digital Audio System license, "license.txt". By continuing to use,
#* modify or distribute this file you indicate that you have
#* read the license and understand and accept it fully.
#*


# Make a final sanity check - TASM only runs in DOS and Win32:
ifndef _DOS
ifndef _WIN32
error:
	echo Invalid system for TASM!
endif
endif


# Define the programs:
ASM = tasm

# Generic compiler options:
ASMOPTS = -UT310 -p -m9 -ml -t
        # -UT310        emulate TASM 3.10 syntax
        # -p            check for code segment overrides
        # -m9           multiple passes (max 9)
        # -ml           case sensitivity in all symbols
        # -t            no messages for successful assembly


# Add debug information and debug code for debug build:
ifdef _DEBUG
  ASMOPTS += -zi -dDEBUG
        # -zi           full debug information
endif

# Add line number debug information for profilind build:
ifdef _PROF
  ASMOPTS += -zd -dM_PROF
endif


# Add compiler-dependent macro definitions:
#ifdef _WC
#  ASMOPTS += -d__WC32__
#endif
#ifdef _VC
#  ASMOPTS += -d__VC32__
#endif
#ifdef _GCC
#  ASMOPTS += -d__WC32__
#endif


# Commands for compiling assembler source files:
COMPILEASM = $(ASM) $(ASMOPTS) $<, $(subst /,\\,$@)




#* $Log: tasmopts.mak,v $
#* Revision 1.6.2.1  1997/10/05 12:33:04  pekangas
#* Fixed to work with Make 4.76
#*
#* Revision 1.6  1997/07/31 10:56:34  pekangas
#* Renamed from MIDAS Sound System to MIDAS Digital Audio System
#*
#* Revision 1.5  1997/07/23 17:28:11  pekangas
#* Added profiling build options
#*
#* Revision 1.4  1997/07/22 13:45:44  pekangas
#* Added support for keeping object modules in a different directory
#* from the source
#*
#* Revision 1.3  1997/05/20 20:39:51  pekangas
#* Target language no longer defined - it didn't make any difference
#*
#* Revision 1.2  1997/02/27 16:22:44  pekangas
#* Added DJGPP support. Changed INCDIR to INCPATH. Other minor fixes.
#*
#* Revision 1.1  1997/02/05 17:33:42  pekangas
#* Initial revision
#*


