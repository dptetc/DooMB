#*      makefile
#*
#* Make script, used just for cleanup
#*
#* $Id: makefile,v 1.2 1997/07/31 10:56:30 pekangas Exp $
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
	$(REM)         make -f makefile.gcc     for DJGPP
	$(REM)         wmake -u -f makefile.wc  for Watcom C

# Cleanup:
clean distclean cvsclean :
	$(CLEANALL)
	-$(RM) demo
	-$(RM) timer

.PHONY : clean distclean cvsclean all


