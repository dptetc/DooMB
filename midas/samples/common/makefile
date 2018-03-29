#*      makefile
#*
#* Make script, used just for cleanup
#*
#* $Id: makefile,v 1.2.2.1 1998/02/26 20:53:39 pekangas Exp $
#*
#* Copyright 1996,1997 Housemarque Inc.
#*
#* This file is part of MIDAS Digital Audio System, and may only be
#* used, modified and distributed under the terms of the MIDAS
#* Digital Audio System license, "license.txt". By continuing to use,
#* modify or distribute this file you indicate that you have
#* read the license and understand and accept it fully.
#*

# Include build options:
BLDOPTSDIR=../../src/bldopts
include $(BLDOPTSDIR)/bldopts.mak


# Default target:
all :
	$(REM) Targets available here: clean, distclean, cvsclean
	$(REM) To compile, use:
	$(REM)         make -f makedos.gcc      for DJGPP
	$(REM)         wmake -u -f makedos.wc   for Watcom C in DOS
	$(REM)         wmake -u -f makefile.wc  for Watcom C in Win32
	$(REM)         nmake -f makefile.vc     for Visual C in Win32
	$(REM)         make -f makelinx.gcc     for Linux

# Cleanup:
clean distclean cvsclean :
	$(CLEANALL)
	-$(RM) all
	-$(RM) effects
	-$(RM) module

.PHONY : clean distclean cvsclean all
