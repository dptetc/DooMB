#*      gcopts.mak
#*
#* Gnu C -specific macros
#*
#* $Id: gcopts.mak,v 1.12 1997/07/31 10:56:33 pekangas Exp $
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
CC = gcc
ifdef _DOS
  CPP = gxx
else
  CPP = g++
endif
LINK = gcc
RC = 

# Object file extension:
O = o

# Library file extension:
_LIB = a

# Dynamic Link Library extension:
_DLL = so

# Executable extension:
ifdef _DOS
  _EXE = .exe
else
  _EXE =
endif

# Generic compiler options:
CCOPTS = -c -Wall -Werror -fpack-struct -D_REENTRANT
        # -c            compile only
        # -Wall         maximum warnings
        # -Werror       warnings are errors
        # -fpack-struct pack all structure members together
        # -shared       produce a shared object
        # -mpentium     optimize for pentium
	# -D_REENTRANT  use thread-safe libc

CPPOPTS = $(CCOPTS)

LINKOPTS = -static
DLLLINKOPTS = -shared 
RCOPTS = 



# Add necessary options for building DLLs:
ifdef _DLL_TARGET
  CCOPTS += -fpic
        # -fpic		generate position-independent code
endif


# Add optimization for retail build:
ifdef _RETAIL
  CCOPTS += -O2 -fomit-frame-pointer -ffast-math -DNDEBUG
        # -O2		reasonable speed optimizations
        # -fomit-frame-pointer don't keep the frame pointer in a register for functions that don't use it
        # -ffast-math   faster, possibly slightly incorrect fp math
        # -DNDEBUG      #define NDEBUG, removes assertions
endif


# Add debug information and debug code for debug build:
ifdef _DEBUG
  CCOPTS += -g -gstabs+ -DDEBUG
        # -g            generate debugging information
	# -gstabs+      generate local symbols debug info etc.
  LINKOPTS += -g -gstabs+
  DLLLINKOPTS += -g -gstabs+
endif


# If additional libraries have been given, add them to the linker directives:
ifdef FIXLIBS
  LINKOPTS += $(patsubst %,-l%,$(FIXLIBS))
  DLLLINKOPTS += $(patsubst %,-l%,$(FIXLIBS))
endif

ifdef FIXLIBPATH
  LINKOPTS += $(patsubst %,-L%,$(FIXLIBPATH))
  DLLLINKOPTS += $(patsubst %,-L%,$(FIXLIBPATH))
endif



# Commands for compiling C and C++ files:
COMPILEC = $(CC) $(CCOPTS) $< -o $@
COMPILECPP = $(CPP) $(CPPOPTS) $< -o $@


# Commands for building a library and a DLL:
BUILDLIBRARY = ar r $@ $^


# Commands for building a simple command line executable from object modules:
BUILDEXE = $(LINK) -o $@ $^ $(LINKOPTS)


# Although DOS has a 126(?) character command line length limit, programs
# compiled with DJGPP (such as make and gcc) can use longer command lines
# between each other, so a fix is not necessary.


# If an include directory has been given, add it to the command line options:
ifdef FIXINCPATH
  CCOPTS += $(patsubst %,-I%,$(FIXINCPATH))
endif


# If command line macro defines have been given, add them to the command line
# options:
ifdef DEFINES
  _DEFINES := $(patsubst %,-D%,$(DEFINES))
  CCOPTS += $(_DEFINES)
endif


#* $Log: gcopts.mak,v $
#* Revision 1.12  1997/07/31 10:56:33  pekangas
#* Renamed from MIDAS Sound System to MIDAS Digital Audio System
#*
#* Revision 1.11  1997/07/28 13:24:32  pekangas
#* Fixed library paths, added BUILDEXE macro
#*
#* Revision 1.10  1997/07/28 11:16:57  pekangas
#* Fixed to compile TASM files for DJGPP properly
#* Some changes to DJGPP options
#*
#* Revision 1.9  1997/07/22 13:45:44  pekangas
#* Added support for keeping object modules in a different directory
#* from the source
#*
#* Revision 1.8  1997/06/10 20:01:32  jpaana
#* Misc option tweaking
#*
#* Revision 1.7  1997/05/30 18:30:42  pekangas
#* Added NDEBUG-#defines for retail builds to remove assertions
#*
#* Revision 1.6  1997/03/22 12:00:32  pekangas
#* Removed optimizations from DJGPP - they seemed to cause failures
#*
#* Revision 1.5  1997/02/27 16:22:37  pekangas
#* Added DJGPP support. Changed INCDIR to INCPATH. Other minor fixes.
#*
#* Revision 1.4  1997/02/22 18:44:42  jpaana
#* Changed the command line parameters to the "normal" gcc
#*
#* Revision 1.3  1997/02/11 18:15:44  pekangas
#* Added executable extension
#*
#* Revision 1.2  1997/02/08 13:35:53  jpaana
#* Added some explanations to options
#*
#* Revision 1.1  1997/02/07 15:16:17  pekangas
#* Initial revision
#*
