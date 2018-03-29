#* 	makefile
#*
#* Make script for MIDAS Module Player for Linux
#*
#* $Id: makefile,v 1.9 1997/08/18 16:39:14 pekangas Exp $ 
#*
#* Copyright 1996, 1997 Jarno Paananen

# Force target:
TARGET = linux

# MIDAS directories:
MIDASPATH=../..
FIXMIDASPATH=../..

# Include build options:
INCPATH=$(MIDASPATH)/include $(MIDASPATH)/src/midas
BLDOPTSDIR=$(MIDASPATH)/src/bldopts
include $(BLDOPTSDIR)/bldopts.mak

# Use the import library by default, to link with the MIDAS DLL, and the
# static link library only if STATIC is defined on the command line:
MIDASLIB = $(FIXMIDASPATH)/lib/$(_TARGET)/$(_COMP)$(BUILD)/libmidas.a



# Default target:
all :		midp xmidp

install :       midp xmidp
	install -s -m 4555 midp /usr/local/bin
	install -s -m 4555 xmidp /usr/local/bin

web:	midp
	strip midp
	cp midp /home/jpaana/public_html/midp-linux-ELF
	gzip -9 -f /home/jpaana/public_html/midp-linux-ELF


# Get build pattern rules:
include $(BLDOPTSDIR)/bldrules.mak


XMIDPOBJS = xmidp.o xmidpecho.o xmidpmenu.o xmidpabout.o gtk--.o

# Linker commands for building the executable:
LINKCMD = gcc $(LINKOPTS) -o midp midplx.o $(MIDASLIB) -lncurses -lpthread
LINKCMD2 = g++ $(LINKOPTS) -L/usr/X11/lib -o xmidp $(XMIDPOBJS) $(MIDASLIB) -lgtk -lgdk -lglib -lX11 -lXext -lpthread

midp :          midplx.o $(MIDASLIB)
	$(LINKCMD)

xmidp :		$(XMIDPOBJS)
	$(LINKCMD2)

# Cleanup:
clean distclean cvsclean :
	$(CLEANALL) midp xmidp

midplx.o :      midplx.c 

xmidp.o :       xmidp.cc xmidp.hh

xmidpecho.o :   xmidpecho.cc xmidp.hh

xmidpmenu.o :   xmidpmenu.cc xmidp.hh

xmidpabout.o :  xmidpabout.cc xmidp.hh


.PHONY : clean distclean cvsclean


#* $Log: makefile,v $
#* Revision 1.9  1997/08/18 16:39:14  pekangas
#* Fixed include directories
#*
#* Revision 1.8  1997/08/04 01:20:58  jpaana
#* Split xmidp to parts and converted to C++. Also added gtk-- for C++ wrappers
#*
#* Revision 1.7  1997/08/03 03:06:15  jpaana
#* Removed xfmidp (obsolete XForms version of xmidp)
#*
#* Revision 1.6  1997/08/03 03:04:58  jpaana
#* Added initial version of X versio of Midp/Linux
#*
#* Revision 1.5  1997/05/31 21:16:32  jpaana
#* Renamed MIDASDIR to MIDASPATH
#*
#* Revision 1.4  1997/02/27 12:09:39  pekangas
#* Make cleanup targets .PHONY, changed INCDIR to INCPATH
#*
#* Revision 1.3  1997/02/22 18:46:33  jpaana
#* Cosmetic changes
#*
#* Revision 1.2  1997/02/08 13:38:55  pekangas
#* Fixed a couple of typos
#*
#* Revision 1.1  1997/02/08 13:35:31  jpaana
#* Added MIDP/Linux to CVS-tree

