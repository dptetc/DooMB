#*      wcopts.mak
#*
#* Watcom C -specific macros
#*
#* $Id: wcopts.mak,v 1.12 1997/07/31 10:56:34 pekangas Exp $
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
CC = wcc386
CPP = wpp386
LINK = wlink
RC = wrc

# Object file extension:
O = obj

# Library file extension:
_LIB = lib

# Dynamic Link Library extension:
_DLL = dll

# Executable extension: 
_EXE = .exe

# Generic compiler options:
CCOPTS = -w3 -we -5r -fh -fhq -mf -zq -fpi87 -fp5
        # -w3           warning level 3
        # -we           treat warnings as errors
        # -5r           optimize for Pentium, use register calling convention
        # -fh           use precompiled headers
        # -fhq          do not display precompiled header information
        # -zq           operate quietly
        # -mf           flat model
	# -fpi87	generate inline 80x87 code
	# -fp5		optimize FPU code for Pentium

CPPOPTS = $(CCOPTS)

LINKOPTS = option caseexact
DLLLINKOPTS = option caseexact
RCOPTS = 



# Add necessary options for building DLLs:
ifdef _DLL_TARGET
  CCOPTS += -bd -dM_DLL
        # -bd		build target is a DLL
endif


# Add optimization for retail build:
ifdef _RETAIL
  CCOPTS += -oneatx -DNDEBUG
        # -oneatx       maximum speed optimizations
        # -DNDEBUG      #define NDEBUG, removes assertions
endif


# Add debug information and debug code for debug build:
ifdef _DEBUG
  CCOPTS += -d2 -dDEBUG
        # -d2           full debug information
  LINKOPTS += debug all
  DLLLINKOPTS += debug all
endif


# Add optimization and line number debug infos for profile build:
ifdef _PROF
  CCOPTS += -oneatx -d1 -DNDEBUG -DM_PROF
        # -oneatx       maximum speed optimizations
        # -d1           line number debug information
        # -DNDEBUG      #define NDEBUG, removes assertions
        # -DM_PROF      #define M_PROF, enables profiling
  # Use Codeview format debug information for profiling - vtune understands
  # it:
  LINKOPTS += debug codeview
  DLLLINKOPTS += debug codeview
endif


# Add DOS-specific flags for MS-DOS:
ifdef _DOS_TARGET
  CCOPTS += -bt=dos -zdp -zu
        # -bt=dos       target DOS
        # -zdp          DS is pegged to DGROUP
        # -zu           disable SS == DGROUP assumption
  LINKOPTS += system dos4g
endif


# Add Win32-specific flags for Win32:
ifdef _WIN32_TARGET
  CCOPTS += -bm -bt=nt
        # -bm		build for a multithreaded environment
        # -bt=nt        target NT
  ifdef _WIN_TARGET
    LINKOPTS += system nt_win
  else
    LINKOPTS += system nt
  endif
  DLLLINKOPTS += system nt dll
  RCOPTS += -bt=nt
endif


# If additional libraries have been given, add them to the linker directives:
ifdef FIXLIBS
  LINKOPTS += $(patsubst %,library %,$(FIXLIBS))
  DLLLINKOPTS += $(patsubst %,library %,$(FIXLIBS))
endif

ifdef FIXLIBPATH
  LINKOPTS += $(patsubst %,libpath %,$(FIXLIBPATH))
endif


# Commands for compiling C and C++ files:
COMPILEC = $(CC) $(CCOPTS) $< -fo=$(subst /,\\,$@)
COMPILECPP = $(CPP) $(CPPOPTS) $< -fo=$(subst /,\\,$@)


# Commands for building a library and a DLL:
ifdef _WIN32
  BUILDLIBRARY = wlib -c -n $(subst /,\\,$@) $(patsubst %,+-%,$(subst /,\\,$^))
  define BUILDDLLIMPLIB
	$(LINK) $(DLLLINKOPTS) name $(subst /,\\,$@) $(patsubst %,file %, $(subst /,\\,$^))
	wlib -c $(subst /,\\,$(IMPLIBNAME)) +-$(subst /,\\,$@)
  endef
endif

ifdef _DOS
  # Due to DOS command line limit, we need to do this the hard way:
  define BUILDLIBRARY
	@$(REM) $(shell if exist libtemp.tmp del libtemp.tmp)
	@$(REM) $(foreach _I,$(subst /,\\,$^), $(shell echo +-$(_I) >> libtemp.tmp)))
	wlib -c -n $(subst /,\\,$@) @libtemp.tmp
	$(RM) libtemp.tmp
  endef
endif


# Commands for building a simple command line executable from object modules:
ifdef _PROF
  define BUILDEXE
	$(LINK) $(LINKOPTS) name $(subst /,\\,$@) $(patsubst %, file %, $(subst /,\\,$^))
	cvpack $(subst /,\\,$@)
  endef
else
  BUILDEXE = $(LINK) $(LINKOPTS) name $(subst /,\\,$@) $(patsubst %, file %, $(subst /,\\,$^))
endif


# Commands for building a resource file from a resource script:
BUILDRES = $(RC) $(RCOPTS) -r $^ -fo=$@ 


# If an include directory has been given, add it to the command line options:
ifdef FIXINCPATH
  _WCINCDIR = $(patsubst %,-i%, $(FIXINCPATH))
  CCOPTS += $(_WCINCDIR)
endif


# If command line macro defines have been given, add them to the command line
# options:
ifdef DEFINES
  _DEFINES := $(patsubst %,-d%,$(DEFINES))
  CCOPTS += $(_DEFINES)
endif


#* $Log: wcopts.mak,v $
#* Revision 1.12  1997/07/31 10:56:34  pekangas
#* Renamed from MIDAS Sound System to MIDAS Digital Audio System
#*
#* Revision 1.11  1997/07/29 11:44:21  pekangas
#* Added cvpack to exe building for profile builds
#*
#* Revision 1.10  1997/07/23 17:28:11  pekangas
#* Added profiling build options
#*
#* Revision 1.9  1997/07/22 13:45:45  pekangas
#* Added support for keeping object modules in a different directory
#* from the source
#*
#* Revision 1.8  1997/05/30 18:30:43  pekangas
#* Added NDEBUG-#defines for retail builds to remove assertions
#*
#* Revision 1.7  1997/02/27 16:22:44  pekangas
#* Added DJGPP support. Changed INCDIR to INCPATH. Other minor fixes.
#*
#* Revision 1.6  1997/02/12 20:46:59  pekangas
#* Watcom C now generates inline 80x87 code
#*
#* Revision 1.5  1997/02/11 18:16:15  pekangas
#* Added executable extension
#* Moved handling of multiple directories in INCDIR here
#* Added a command to link a command-line program
#*
#* Revision 1.4  1997/02/06 20:57:30  pekangas
#* Added support for defining command line macros and adding libraries to
#* link. Other minor changes - build system is still a bit under construction.
#*
#* Revision 1.3  1997/02/06 12:46:30  pekangas
#* MIDAS include directory no longer added to compiler command lines -
#* instead, if INCDIRS is defined before bldopts is included, it get converted
#* to the correct slash type and will be used.
#* Magic target type macros aren't defined on command line any more - lang.h
#* determines the compiler type without them
#*
#* Revision 1.2  1997/02/05 22:53:55  pekangas
#* Several small changes to get the new make script system work better
#*
#* Revision 1.1  1997/02/05 17:33:42  pekangas
#* Initial revision
#*