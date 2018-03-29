#*      bldopts.mak
#*
#* Build options common for all make scripts
#*
#* $Id: bldopts.mak,v 1.18.2.2 1997/10/05 13:16:33 pekangas Exp $
#*
#* Copyright 1997 Housemarque Inc.
#*
#* This file is part of MIDAS Digital Audio System, and may only be
#* used, modified and distributed under the terms of the MIDAS
#* Digital Audio System license, "license.txt". By continuing to use,
#* modify or distribute this file you indicate that you have
#* read the license and understand and accept it fully.
#*

# Make sure that BLDOPTSDIR is defined:
ifndef BLDOPTSDIR
error:
	echo BLDOTPSDIR undefined!
endif


# If system hasn't been set, try to figure it out: (this might not work in
# 100% of the cases, but at least an educated guess should be better than
# nothing)
ifndef SYSTEM
  ifdef OS
    ifeq ($(OS), Windows_NT)
      ifeq ($(SHELL), /bin/sh)
        _CYGWIN32=1
      endif
      SYSTEM=win32
    else
      SYSTEM=dos
    endif
  else
    ifdef OSTYPE
      ifeq ($(OSTYPE), linux)
        SYSTEM=linux
      else
        SYSTEM=dos
      endif
    else
      SYSTEM=dos
    endif
  endif
endif

# (We'll detect Win95 as DOS - this is a feature, not a bug, as the Win32
# make doesn't seem to work well there)


# Define system macros:
ifeq ($(SYSTEM), dos)
_DOS = 1
_SYS = dos
_X86 = 1
endif
ifeq ($(SYSTEM), win32)
_WIN32 = 1
_SYS = win32
_X86 = 1
endif
ifeq ($(SYSTEM), linux)
_LINUX = 1
_SYS = linux
_X86 = 1
endif


# If no build type has been set, build debug:
ifndef BUILD
  BUILD = debug
endif


# Define build type macros:
ifeq ($(BUILD), debug)
  _DEBUG = 1
  _BUILD = debug
endif
ifeq ($(BUILD), retail)
  _RETAIL = 1
  _BUILD = retail
endif
ifeq ($(BUILD), prof)
  _PROF = 1
  _BUILD = prof
endif
ifeq ($(BUILD), profile)
  _PROF = 1
  _BUILD = prof
  BUILD = prof
endif


# If target system hasn't been set, set the default:
ifndef TARGET
  ifdef _DOS
    TARGET = dos
  endif
  ifdef _WIN32
    TARGET = win32
  endif
  ifdef _LINUX
    TARGET = linux
  endif
endif


# Define target macros:
ifeq ($(TARGET), dos)
  _DOS_TARGET = 1
  _TARGET = dos
endif
ifeq ($(TARGET), win32)
  _WIN32_TARGET = 1
  _TARGET = win32
endif
ifeq ($(TARGET), win32dll)
  _WIN32_TARGET = 1
  _TARGET = win32
  _DLL_TARGET = 1
endif
ifeq ($(TARGET), win32win)
  _WIN32_TARGET = 1
  _TARGET = win32
  _WIN_TARGET = 1
endif
ifeq ($(TARGET), linux)
  _LINUX_TARGET = 1
  _TARGET = linux
endif


# If compiler hasn't been set, try to figure the correct one:
ifndef COMPILER
  ifdef _LINUX_TARGET
    COMPILER=gcc
  else
    ifdef _WIN32_TARGET
      ifdef MSDevDir
        COMPILER = visualc
      else
        COMPILER = watcom
      endif
    else
      ifdef DJGPP
        COMPILER=gcc
      else
        COMPILER=watcom
      endif
    endif
  endif
endif


# Define compiler macros:
ifeq ($(COMPILER), watcom)
  _WC = 1
  _COMP = wc
endif
ifeq ($(COMPILER), visualc)
  _VC = 1
  _COMP = vc
endif
ifeq ($(COMPILER), gcc)
  _GCC = 1
  _COMP = gc
  ifdef _DOS_TARGET
    _DJGPP = 1
    _DJGPP_TARGET = 1
  endif
endif


# Check if the system uses backslashes instead of slashes:
ifdef _DOS
  ifndef _GCC
    _BKSLASH = 1
    _SHELL_BKSLASH = 1
  endif
endif
ifdef _WIN32
  _BKSLASH = 1
  ifndef _CYGWIN32
    _SHELL_BKSLASH = 1
  endif
endif



# Now, make sure we have a valid compiler, system, build and target type:
ifndef _SYS
error:
	echo Invalid system $(SYSTEM)!
endif
ifndef _COMP
error:
	echo Invalid compiler!
endif
ifndef _BUILD
error:
	echo Invalid build type!
endif
ifndef _TARGET
error:
	echo Invalid target type!
endif


# Get possible target-dependent command line defines and include directories:
ifdef _WIN32_TARGET
  ifdef WIN32INCPATH
    INCPATH += $(WIN32INCPATH)
  endif
  ifdef WIN32DEFINES
    DEFINES += $(WIN32DEFINES)
  endif
  ifdef WIN32LIBS
    LIBS += $(WIN32LIBS)
  endif
endif

ifdef _DJGPP
  ifdef DOSGCCINCPATH
    INCPATH += $(DOSGCCINCPATH)
  endif
  ifdef DOSGCCDEFINES
    DEFINES += $(DOSGCCDEFINES)
  endif
  ifdef DOSGCCLIBPATH
    LIBPATH += $(DOSGCCLIBPATH)
  endif
  ifdef DOSGCCLIBS
    LIBS += $(DOSGCCLIBS)
  endif
endif


ifdef _LINUX
  ifdef LINUXINCPATH
    INCPATH += $(LINUXINCPATH)
  endif
  ifdef LINUXDEFINES
    INCPATH += $(LINUXDEFINES)
  endif
  ifdef LINUXLIBPATH
    LIBPATH += $(LINUXLIBPATH)
  endif
  ifdef LINUXLIBS
    LIBS += $(LINUXLIBS)
  endif
endif


# If we have been given an include directory or libraries, fix the slashes:
ifdef INCPATH
  ifdef _BKSLASH
    FIXINCPATH := $(subst /,\\,$(INCPATH))
  else
    FIXINCPATH := $(INCPATH)
  endif
endif

ifdef LIBS
  ifdef _BKSLASH
    FIXLIBS := $(subst /,\\,$(LIBS))
  else
    FIXLIBS := $(LIBS)
  endif
endif

ifdef LIBPATH
  ifdef _BKSLASH
    FIXLIBPATH := $(subst /,\\,$(LIBPATH))
  else
    FIXLIBPATH := $(LIBPATH)
  endif
endif 


# Make sure we have a valid object target directory: (can be changed later)
ifndef OTARGETDIR
  OTARGETDIR = ./
endif


# Include system-specific commands:
include $(BLDOPTSDIR)/$(_SYS)cmd.mak

# Include compiler-specific macros:
include $(BLDOPTSDIR)/$(_COMP)opts.mak

# Include TASM-specific macros:
ifdef _DOS
include $(BLDOPTSDIR)/tasmopts.mak
endif
ifdef _WIN32
include $(BLDOPTSDIR)/tasmopts.mak
endif

# Include NASM-specific macros:
include $(BLDOPTSDIR)/nasmopts.mak


# Cleanup commands - everything but binaries/libraries:
ifndef _DOS
  CLEAN = -$(RM) *.$(O) *.obj *.o *.bak *~ core *.pch *.pdb *.ilk *.exp \
                 *.sym *.map *.res
else
  CLEAN = -$(RM) *.$(O); $(RM) *.obj; $(RM) *.o; $(RM) *.bak; \
           $(RM) core; $(RM) *.pch; $(RM) *.pdb; $(RM) *.ilk; $(RM) *.exp; \
           $(RM) *.sym; $(RM) *.map; $(RM) *.res
endif

# Note! Under DOS *~ also matches files like makefile! Don't use!

# Cleanup commands - everything:
ifndef _DOS
  CLEANALL = -$(RM) *.obj *.o *.exe *.dll *.lib *.bak *~ core *.pch *.pdb \
                    *.ilk *.exp *.sym *.map *.res *.a *.so
else
  CLEANALL = -$(RM) *.obj; $(RM) *.o; $(RM) *.exe; $(RM) *.dll; $(RM) *.lib; \
              $(RM) *.bak; $(RM) core; $(RM) *.pch; $(RM) *.pdb; \
              $(RM) *.ilk; $(RM) *.exp; $(RM) *.sym; $(RM) *.map; \
              $(RM) *.res; $(RM) *.a; $(RM) *.so
endif


# Flag that we have been included OK:
_BLDOPTS_OK = 1



#* $Log: bldopts.mak,v $
#* Revision 1.18.2.2  1997/10/05 13:16:33  pekangas
#* Fixed to work with Cygnus Win32 make
#*
#* Revision 1.18.2.1  1997/10/05 12:33:03  pekangas
#* Fixed to work with Make 4.76
#*
#* Revision 1.18  1997/07/31 10:56:33  pekangas
#* Renamed from MIDAS Sound System to MIDAS Digital Audio System
#*
#* Revision 1.17  1997/07/28 13:24:02  pekangas
#* Added linux libraries, paths and defines
#*
#* Revision 1.16  1997/07/23 17:28:10  pekangas
#* Added profiling build options
#*
#* Revision 1.15  1997/07/22 13:45:43  pekangas
#* Added support for keeping object modules in a different directory
#* from the source
#*
#* Revision 1.14  1997/07/16 12:34:49  pekangas
#* Added _X86 macro definitions for 80x86 targets
#*
#* Revision 1.13  1997/06/04 15:27:25  pekangas
#* Added NASM support
#*
#* Revision 1.12  1997/05/20 20:40:27  pekangas
#* Fixed library and library path settings
#*
#* Revision 1.11  1997/02/27 16:20:37  pekangas
#* Added DJGPP support
#* Changed INCDIR to INCPATH
#*
#* Revision 1.10  1997/02/11 18:15:17  pekangas
#* Moved INCDIR fix to compiler-specific files. Multiple include files are
#* now simply stored as a list in INCDIR, and the compiler-specific file is
#* responsible for building a suitable list of command line arguments
#*
#* Revision 1.9  1997/02/11 17:19:57  pekangas
#* Fixed building INCDIRS
#*
#* Revision 1.8  1997/02/07 15:16:32  pekangas
#* Added GCC/Linux support
#*
#* Revision 1.7  1997/02/06 21:10:03  pekangas
#* Added *.res to cleanup file list
#*
#* Revision 1.6  1997/02/06 20:57:29  pekangas
#* Added support for defining command line macros and adding libraries to
#* link. Other minor changes - build system is still a bit under construction.
#*
#* Revision 1.5  1997/02/06 12:52:56  pekangas
#* If MSDevDir is defined, compiler now defaults to Visual C under Win32
#* Added preliminary GCC support checks
#*
#* Revision 1.4  1997/02/06 12:46:29  pekangas
#* MIDAS include directory no longer added to compiler command lines -
#* instead, if INCDIRS is defined before bldopts is included, it get converted
#* to the correct slash type and will be used.
#* Magic target type macros aren't defined on command line any more - lang.h
#* determines the compiler type without them
#*
#* Revision 1.3  1997/02/05 22:57:06  pekangas
#* Clean now deletes map files
#*
#* Revision 1.2  1997/02/05 22:53:54  pekangas
#* Several small changes to get the new make script system work better
#*
#* Revision 1.1  1997/02/05 17:25:54  pekangas
#* Initial revision
#*