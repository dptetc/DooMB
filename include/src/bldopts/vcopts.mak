#*      vcopts.mak
#*
#* Visual C -specific macros
#*
#* $Id: vcopts.mak,v 1.10.2.2 1997/10/05 12:33:04 pekangas Exp $
#*
#* Copyright 1997 Housemarque Inc.
#*
#* This file is part of MIDAS Digital Audio System, and may only be
#* used, modified and distributed under the terms of the MIDAS
#* Digital Audio System license, "license.txt". By continuing to use,
#* modify or distribute this file you indicate that you have
#* read the license and understand and accept it fully.
#*

# Make a final sanity check - you can't compile DOS programs with Visual C:
ifndef _WIN32_TARGET
error:
	echo Visual C only supports Win32 targets
endif

# Define the programs:
CC = cl
CPP = cl
LINK = link
RC = rc

_LIB = lib
_DLL = dll
_EXE = .exe

# Object file extension:
O = obj

# Generic compiler options:
CCOPTS = -c -W3 -WX -YX -Zp1 -MT -nologo -Ot -G5
        # -c            compile only, no ling
        # -W3           warning level 3
        # -WX           treat warnings as errors
        # -YX           automatic precompiled headers
        # -Zp1          pack structures to 1-byte boundaries (no packing)
        # -MT           link with multithreaded runtime library
        # -nologo       don't display the annoying logo
        # -Ot           favor code speed
        # -G5           optimize for Pentium

CPPOPTS = $(CCOPTS)

LINKOPTS = -nologo
DLLLINKOPTS = -nologo

LIBOPTS = -nologo

RCOPTS =


# If an include directory has been given, add it to the command line options:
ifdef FIXINCPATH
  _VCINCDIR = $(patsubst %,-I%, $(FIXINCPATH))
  CCOPTS += $(_VCINCDIR)
endif


# Add options for building DLLs:
ifdef _DLL_TARGET
  CCOPTS += -GD -DM_DLL
        # -GD		optimize for Windows DLL
  DLLLINKOPTS += -dll
else
  CCOPTS += -GA
	# -GA		optimize for Windows Application
endif


# Add optimization for retail build:
ifdef _RETAIL
  CCOPTS += -O2 -Ox -DNDEBUG
        # -O2           maximize speed
        # -Ox           maximum optimizations
        # -DNDEBUG      #define NDEBUG, removes assertions
endif


# Add debug information and debug code for debug build:
ifdef _DEBUG
  CCOPTS += -Zi -DDEBUG
        # -Zi           debug information
  LINKOPTS += -debug -debugtype:both
  DLLLINKOPTS += -debug -debugtype:both
  LIBOPTS += -debugtype:both
endif


# Add optimization and line number debug infos for profile build:
ifdef _PROF
  CCOPTS += -O2 -Ox -DNDEBUG -Zd -DM_PROF
        # -O2           maximize speed
        # -Ox           maximum optimizations
        # -DNDEBUG      #define NDEBUG, removes assertions
        # -Zd           line number debug info
        # -DM_PROF      #define M_PROF, enables profiling
  LINKOPTS += -debug -debugtype:both
  DLLLINKOPTS += -debug -debugtype:both
  LIBOPTS += -debugtype:both
endif


# If additional libraries have been given, add them to the linker directives:
ifdef FIXLIBS
  LINKOPTS += $(patsubst %,-defaultlib:%,$(FIXLIBS))
  DLLLINKOPTS += $(FIXLIBS)
endif

# Add additional library paths to the LIB environment variable:
ifdef FIXLIBPATH
  LIB+=;$(FIXLIBPATH)
  export LIB
endif


# Commands for compiling C and C++ files:
COMPILEC = $(CC) $(CCOPTS) $< -Fo$(subst /,\\,$@)
COMPILECPP = $(CPP) $(CPPOPTS) $< -Fo$(subst /,\\,$@)


# Commands for building a library:
BUILDLIBRARY = lib $(LIBOPTS) -out:$@ -nologo $(subst /,\\,$^)


# Commands for building a DLL and an import library:
BUILDDLLIMPLIB = $(LINK) $(DLLLINKOPTS) -out:$(subst /,\\,$@) -implib:$(IMPLIBNAME) $(subst /,\\,$^) winmm.lib


# Commands for building a simple command line executable from object modules:
BUILDEXE = $(LINK) $(LINKOPTS) /out:$(subst /,\\,$@) $(subst /,\\,$^)


# Commands for building a resource file from a resource script:
BUILDRES = $(RC) $(RCOPTS) -r -fo $@ $^


# If command line macro defines have been given, add them to the command line
# options:
ifdef DEFINES
  _DEFINES := $(patsubst %,-D%,$(DEFINES))
  CCOPTS += $(_DEFINES)
endif



#* $Log: vcopts.mak,v $
#* Revision 1.10.2.2  1997/10/05 12:33:04  pekangas
#* Fixed to work with Make 4.76
#*
#* Revision 1.10.2.1  1997/08/25 19:33:19  pekangas
#* Added profiling support
#*
#* Revision 1.10  1997/07/31 10:56:34  pekangas
#* Renamed from MIDAS Sound System to MIDAS Digital Audio System
#*
#* Revision 1.9  1997/07/28 13:43:47  pekangas
#* Fixed the handling of libraries and library paths
#*
#* Revision 1.8  1997/07/22 13:45:44  pekangas
#* Added support for keeping object modules in a different directory
#* from the source
#*
#* Revision 1.7  1997/05/30 18:30:42  pekangas
#* Added NDEBUG-#defines for retail builds to remove assertions
#*
#* Revision 1.6  1997/02/27 16:22:44  pekangas
#* Added DJGPP support. Changed INCDIR to INCPATH. Other minor fixes.
#*
#* Revision 1.5  1997/02/11 18:16:14  pekangas
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