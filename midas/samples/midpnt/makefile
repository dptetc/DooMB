#* 	makefile
#*
#* Make script for MIDAS Module Player for Windows NT
#*
#* $Id: makefile,v 1.9.2.1 1997/10/05 13:17:13 pekangas Exp $
#*
#* Copyright 1996 Petteri Kangaslampi

# Force target:
ifndef TARGET
  TARGET = win32win
endif

# MIDAS directories:
MIDASDIR=../..
FIXMIDASDIR=..\\..

# Include build options:
INCPATH=$(MIDASDIR)/include
BLDOPTSDIR=$(MIDASDIR)/src/bldopts
include $(BLDOPTSDIR)/bldopts.mak

# Use the import library by default, to link with the MIDAS DLL, and the
# static link library only if STATIC is defined on the command line:
ifdef STATIC
  MIDASLIB = $(FIXMIDASDIR)\\lib\\$(_TARGET)\\$(_COMP)$(BUILD)\\midas.lib
else
  MIDASLIB = $(FIXMIDASDIR)\\lib\\$(_TARGET)\\$(_COMP)$(BUILD)\\midasdll.lib
endif


# Default target:
all :		midpnt.exe


# Get build pattern rules:
include $(BLDOPTSDIR)/bldrules.mak


# Linker commands for building the executable:
ifdef _VC
  LINKCMD = link $(LINKOPTS) @MidpNT.lk $(MIDASLIB)
endif
ifdef _WC
  LINKCMD = wlink $(LINKOPTS) library $(MIDASLIB) @midpnt.wlk
endif


midpnt.exe :    midpnt.obj midpmidas.obj midplist.obj midpview.obj \
                songinfo.obj viewlist.obj instlist.obj archivers.obj \
                registry.obj midpecho.obj echoedit.obj $(MIDASLIB) \
		midpnt.res
	$(LINKCMD)
ifndef _VC
	$(RC) $(RCOPTS) MidpNT.res MidpNT.exe
endif

midpnt.res :	temp.rc
	$(BUILDRES)

temp.rc :	midpnt.rc
ifdef _CYGWIN32
	-rm temp.rc midpnt.res
	-cat incwin.rc midpnt.rc > temp.rc
else
	-del temp.rc midpnt.res
	-copy incwin.rc+midpnt.rc temp.rc
endif


# Cleanup:
clean distclean cvsclean :
	$(CLEANALL)
	-del temp.rc

midpnt.obj :    midpnt.cpp midpres.h midpnt.h midplist.h midpview.h \
                songinfo.h midpmodeless.h viewlist.h instlist.h \
                archivers.h registry.h midpecho.h echoedit.h

midpmidas.obj : midpmidas.cpp midpnt.h

midplist.obj :  midplist.cpp midplist.h

midpview.obj :  midpview.cpp midpnt.h midpview.h

songinfo.obj :  songinfo.cpp songinfo.h midpview.h midpnt.h midpres.h \
                midpmodeless.h viewlist.h

viewlist.obj :  viewlist.cpp viewlist.h midpview.h midplist.h

instlist.obj :  instlist.cpp instlist.h midpview.h midpnt.h midpres.h \
                midpmodeless.h viewlist.h

archivers.obj : archivers.cpp archivers.h midpnt.h

registry.obj :  registry.cpp registry.h midpnt.h

midpecho.obj :	midpecho.cpp midpnt.h midplist.h registry.h midpecho.h

echoedit.obj :	echoedit.cpp midpview.h midpnt.h midpmodeless.h \
		echoedit.h midpecho.h midpres.h viewlist.h \
		registry.h


.PHONY : clean cvsclean distclean


#* $Log: makefile,v $
#* Revision 1.9.2.1  1997/10/05 13:17:13  pekangas
#* Fixed to work with Cygnus Win32 make
#*
#* Revision 1.9  1997/07/10 18:41:26  pekangas
#* Added echo effect editor and echo effect support
#*
#* Revision 1.8  1997/07/08 19:19:50  pekangas
#* Added new MIDAS setup, changed version to 0.3.0 and added a possibility
#* to compile a console version
#*
#* Revision 1.7  1997/05/20 20:44:31  pekangas
#* Removed linking the DirectSound import library
#*
#* Revision 1.6  1997/02/27 16:27:57  pekangas
#* Changed INCDIR to INCPATH, made cleanup targets .PHONY
#*
#* Revision 1.5  1997/02/06 12:56:34  pekangas
#* Fixed to work with updated bldopts
#*
#* Revision 1.4  1997/02/05 22:57:53  pekangas
#* Fixed small problems caused by RCS->CVS move.
#* Changed to use new build system - GNU Make now required.
#*
#* Revision 1.3  1997/01/14 17:41:19  pekangas
#* ?
#*
# Revision 1.2  1996/08/02  20:14:53  pekangas
# Added Visual C support
#
